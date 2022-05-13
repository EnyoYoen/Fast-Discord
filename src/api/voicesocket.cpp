#include "api/voicesocket.h"

#include <QHostAddress>
#include <sodium.h>

namespace Api {

struct RtpHeader
{
    quint16 constant;
	quint16 sequence;
	quint32 timestamp;
	quint32 ssrc;
};
    

VoiceSocket::VoiceSocket()
{
    heartbeatThread = nullptr;
    sequence = 0;
    timestamp = 0;
    speaking = false;
    connected = false;
    udpConnected = false;
    mute = false;
    deaf = false;

    timer.setInterval(1000);
    timer.callOnTimeout([this](){
        speaking = false;
        timer.stop();
        send(Opcodes::Speaking, "{\"speaking\":0,\"delay\":5,\"ssrc\":" + QString::number(ssrc) + "}");
    });

    if (sodium_init() < 0)
		fprintf(stderr, "Failed to init sodium\n");

    int err;
    encoder = opus_encoder_create(SAMPLE_RATE, CHANNELS, APPLICATION, &err);
    if (err < 0)
        fprintf(stderr, "Failed to create an encoder: %s\n", opus_strerror(err));

    err = opus_encoder_ctl(encoder, OPUS_SET_BITRATE(BITRATE));
    if (err < 0)
        fprintf(stderr, "Failed to set bitrate: %s\n", opus_strerror(err));

    decoder = opus_decoder_create(SAMPLE_RATE, CHANNELS, &err);
    if (err < 0)
        fprintf(stderr, "Failed to create decoder: %s\n", opus_strerror(err));

    repacketizer = opus_repacketizer_create();

    QObject::connect(&client, &QWebSocket::aboutToClose, this, &VoiceSocket::closeHandler);
    QObject::connect(&client, &QWebSocket::textMessageReceived, this, &VoiceSocket::processTextMessage);
    QObject::connect(&udp, &QUdpSocket::readyRead, this, &VoiceSocket::readPendingDatagrams);
    QObject::connect(&udp, &QUdpSocket::connected, [this](){udpConnected = true;});
    QObject::connect(&udp, &QUdpSocket::disconnected, [this](){udpConnected = false;});
    //QObject::connect(&wrBuff, &QIODevice::bytesWritten, this, &VoiceSocket::test);
}

void VoiceSocket::start(const QString& endpointp, const Snowflake& serverIdp, const Snowflake& userIdp, const QString& sessionIdp, const QString& tokenp)
{
    if (connected) return;
    endpoint = endpointp;
    serverId = serverIdp;
    userId = userIdp;
    sessionId = sessionIdp;
    token = tokenp;

    client.open(QUrl("wss://" + endpoint + "/?v=6"));


    wrBuff.open(QBuffer::WriteOnly);
    rdBuff.open(QBuffer::ReadOnly);

    inFormat.setSampleRate(48000);
    inFormat.setChannelCount(2);
    inFormat.setCodec("audio/pcm");

    outFormat.setSampleRate(48000);
    outFormat.setChannelCount(2);
    outFormat.setCodec("audio/pcm");

    QAudioDeviceInfo info = QAudioDeviceInfo::defaultInputDevice();
    if(!info.isFormatSupported(inFormat))
        inFormat = info.nearestFormat(inFormat);

    info = QAudioDeviceInfo::defaultOutputDevice();
    if(!info.isFormatSupported(outFormat))
        outFormat = info.nearestFormat(outFormat);

    input = new QAudioInput(inFormat);
    output = new QAudioOutput(outFormat);

    input->start(&wrBuff);
    output->start(&rdBuff);
    input->resume();
}

void VoiceSocket::stop()
{
    connected = false;
    udpConnected = false;

    client.close();
    udp.close();

    input->stop();
    output->stop();
}

void VoiceSocket::closeHandler()
{
    connected = false;
}

void VoiceSocket::identify()
{
    send(Opcodes::Identify, 
        "{\"server_id\":\"" + serverId + "\","
         "\"user_id\":\"" + userId + "\","
         "\"session_id\":\"" + sessionId + "\","
         "\"token\":\"" + token + "\","
         "\"video\":true,"
         "\"streams\":[{\"type\":\"video\",\"rid\":\"100\",\"quality\":100}]"
        "}");
}

void VoiceSocket::resume()
{
    send(Opcodes::Resume, "{\"server_id\":\"" + serverId + "\","
                           "\"session_id\":\"" + sessionId + "\","
                           "\"token\":\"" + token + "\"}");
}

void VoiceSocket::processTextMessage(const QString& message)
{
    QJsonDocument payload = QJsonDocument::fromJson(message.toUtf8());
    QJsonValue data = payload["d"];

    qDebug() << "⇩" << message;

    switch (payload["op"].toInt(-1)) {
        case Opcodes::Ready:
        {
            send(Opcodes::Unknown, "{}");

            ip = data["ip"].toString();
            port = data["port"].toInt();
            modes = data["modes"].toArray().toVariantList();
            ssrc = data["ssrc"].toVariant().toULongLong();

            unsigned char discovery[74] = {0};
            (*(quint16*)(discovery)) = qToBigEndian(quint16(0x01));
            (*(quint16*)(discovery + 2)) = qToBigEndian(quint16(70));
            (*(quint32*)(discovery + 4)) = qToBigEndian(ssrc);
            
            udp.bind();
            hostPort = udp.localPort();
            udp.connectToHost(QHostAddress(ip),port);
            udp.waitForConnected();
            udp.write(QByteArray((const char *)discovery, 74));
            break;
        }

        case Opcodes::SessionDescription:
        {
            send(Opcodes::ClientConnect, "{\"audio_ssrc\":0,\"video_ssrc\":0,\"rtx_ssrc\":0,\"streams\":[{\"type\":\"video\",\"rid\":\"100\",\"ssrc\":0,\"active\":false,\"quality\":100,\"rtx_ssrc\":0,\"max_bitrate\":2500000,\"max_framerate\":30,\"max_resolution\":{\"type\":\"fixed\",\"width\":1280,\"height\":720}}]}");
            QJsonArray secretKeyJson = data["secret_key"].toArray();
            for (unsigned int i = 0 ; i < 32 ; i++)
                secretKey[i] = (quint8)secretKeyJson[i].toInt();
            break;
        }

        case Opcodes::Speaking:
            break;

        case Opcodes::HeartbeatACK:
        {
            nonce = data.toVariant().toULongLong();
            break;
        }

        case Opcodes::Resume:
            break;

        case Opcodes::Hello:
        {
            if (!modes.empty())
                resume();
            else
                identify();
            connected = true;

            // Start Heartbeating
            int heartbeatInterval = data["heartbeat_interval"].toInt(15000);

            if (heartbeatThread == nullptr) {
                send(Opcodes::Heartbeat, "0");
                heartbeatThread = QThread::create([this, heartbeatInterval](){
                    while (connected) {
                        QThread::msleep(heartbeatInterval);
                        send(Opcodes::Heartbeat, QString::number(nonce));
                    }
                });
                heartbeatThread->start();
            }
            break;
        }

        case Opcodes::Unknown:
            send(Opcodes::ClientConnect, "{\"audio_ssrc\":0,\"video_ssrc\":0,\"rtx_ssrc\":0,\"streams\":[{\"type\":\"video\",\"rid\":\"100\",\"ssrc\":0,\"active\":false,\"quality\":100,\"rtx_ssrc\":0,\"max_bitrate\":2500000,\"max_framerate\":30,\"max_resolution\":{\"type\":\"fixed\",\"width\":1280,\"height\":720}}]}");
            break;
    }
}

void VoiceSocket::readPendingDatagrams()
{
    while (udp.hasPendingDatagrams()) {
        QNetworkDatagram datagram = udp.receiveDatagram(16384);
        if (externalIp.isNull()) {
            externalIp = QString(datagram.data().mid(8, 64));
            send(Opcodes::SelectProtocol, 
                "{\"protocol\":\"udp\","
                "\"data\":{"
                    "\"address\":\"" + externalIp + "\","
                    "\"port\":" + QString::number(hostPort) + ","
                    "\"mode\":\"xsalsa20_poly1305\""
                "}}");
            continue;
        }

        if (!deaf) {
            int size = datagram.data().size();
            if (size < 12) continue;
            unsigned char *buffer = (unsigned char *)datagram.data().data();

            quint8 nonce[24] = {0};
            memcpy(nonce, buffer, 12);

            quint8 *packet = buffer + 12;
            if (crypto_secretbox_open_easy(packet, packet, size-12, nonce, secretKey))
                continue;

            int packetSize = size-12-crypto_box_MACBYTES;

            if (!(packet[0] == 0xbe && packet[1] == 0xde && packetSize > 4))
                continue;
            
            quint16 extensionSize;
            memcpy(&extensionSize, packet+2, sizeof(quint16));
            extensionSize = qFromBigEndian(extensionSize);

            int offset = 4;
            for (int i = 0 ; i < extensionSize ; i++) {
                qint8 byte = packet[offset];
                offset++;
                if (byte == 0) 
                    continue;
                offset += 1 + (byte >> 4);
            }
            qint8 byte = packet[offset];
            if (byte == 0x00 || byte == 0x02) 
                offset++;

            packet = packet + offset;
            packetSize -= offset;

            opus_int16 pcm[23040];
            int samples = opus_decode(decoder, packet, packetSize, pcm, 5760, 0);
            if (samples < 0)
                continue;
            
            rdBuff.buffer().append((const char *)pcm, 2*samples*CHANNELS);
        }
    }
}

void const VoiceSocket::send(Opcodes op, const QString& data)
{
    // Build the payload string
    QString payload = "{\"op\":" + QString::number(op) + ",\"d\":" + data + "}";
    qDebug() << "⇧" << payload;
    // Send the message
    client.sendTextMessage(payload);
}

int const VoiceSocket::encode(quint8 *rawData, int rawDataSize, quint8 *encodedData)
{
    int encodedDataSize = 0;
    memset(encodeBuffer, 0, MAX_PACKET_SIZE);
    quint8 *out = (quint8 *)encodeBuffer;

    repacketizer = opus_repacketizer_init(repacketizer);
    if (repacketizer == nullptr)
        return encodedDataSize;
    
    for (int i = 0; i < (rawDataSize / MAX_FRAME_SIZE); ++ i) {
        const opus_int16* pcm = (opus_int16*)(rawData + i * MAX_FRAME_SIZE);
        int ret = opus_encode(encoder, pcm, FRAME_SIZE, out, MAX_PACKET_SIZE);
        if (ret > 0) {
            int retval = opus_repacketizer_cat(repacketizer, out, ret);
            if (retval != OPUS_OK)
                return encodedDataSize;
            out += ret;
        } else {
            return encodedDataSize;
        }
    }

    int ret = opus_repacketizer_out(repacketizer, encodedData, MAX_PACKET_SIZE);
    if (ret > 0)
        encodedDataSize = ret;

    return encodedDataSize;
}

void const VoiceSocket::sendAudio(QByteArray data, qint64 length)
{
	if (length > MAX_FRAME_SIZE) {
		while (data.size() > MAX_FRAME_SIZE) {
			QByteArray packet = data.mid(0, MAX_FRAME_SIZE);
            data.remove(0, MAX_FRAME_SIZE);
			if (packet.size() < MAX_FRAME_SIZE)
				packet.resize(MAX_FRAME_SIZE);
			sendAudio(packet, MAX_FRAME_SIZE);
		}
	} else {
        QVector<quint8> encodedAudioData(length);
        data.resize(MAX_FRAME_SIZE);
        length = encode((quint8*)data.data(), length, encodedAudioData.data());
        int samples = opus_packet_get_nb_samples(encodedAudioData.data(), (opus_int32)length, 48000);

        QVector<uint8_t> packet(12 + length + crypto_secretbox_MACBYTES);
        sequence++;
        RtpHeader rtp{qToBigEndian<quint16>(0x8078), sequence, timestamp, ssrc};
        qint8 nonce[24] = {0};
        memcpy(nonce, &rtp, 12);
        memcpy(packet.data(), &rtp, 12);
        crypto_secretbox_easy(packet.data() + 12, encodedAudioData.data(), length, (const unsigned char*)nonce, secretKey);
    
        if (!speaking) {
            speaking = true;
            send(Opcodes::Speaking, "{\"speaking\":1,\"delay\":5,\"ssrc\":" + QString::number(ssrc) + "}");
        }
        udp.write(QByteArray((const char *)packet.data(), packet.size()));
	    timestamp += samples;
    }
}

void const VoiceSocket::sendSilence()
{
    const char silence[] = {(char)0xf8, (char)0xff, (char)0xfe};
    sendAudio(QByteArray(silence, 3), 3);
}

void VoiceSocket::test(qint64 length)
{
    if (connected && udpConnected && !mute) {
        sendAudio(wrBuff.buffer(), length);

        wrBuff.buffer().clear();
        wrBuff.seek(0);
    }
}

} // namespace Api