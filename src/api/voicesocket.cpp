#include "api/voicesocket.h"

#include <QHostAddress>

namespace Api {

VoiceSocket::VoiceSocket()
{
    connected = false;

    QObject::connect(&udp, &QUdpSocket::readyRead, this, &VoiceSocket::readPendingDatagrams);
}

void VoiceSocket::start(const QString& endpointp, const Snowflake& serverIdp, const Snowflake& userIdp, const QString& sessionIdp, const QString& tokenp)
{
    endpoint = endpointp;
    serverId = serverIdp;
    userId = userIdp;
    sessionId = sessionIdp;
    token = tokenp;

    QObject::connect(&client, &QWebSocket::aboutToClose, this, &VoiceSocket::closeHandler);
    QObject::connect(&client, &QWebSocket::connected, this, &VoiceSocket::identify);
    QObject::connect(&client, &QWebSocket::textMessageReceived, this, &VoiceSocket::processTextMessage);
    //QObject::connect(&client, &QWebSocket::binaryMessageReceived, this, &VoiceSocket::processBinaryMessage);

    client.open(QUrl("wss://" + endpoint + "/?v=4"));
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
         "\"token\":\"" + token + "\""
        "}");
}

void VoiceSocket::processTextMessage(const QString& message)
{
    QJsonDocument payload = QJsonDocument::fromJson(message.toUtf8());
    QJsonValue data = payload["d"];

    //qDebug() << "⇩" << message;

    switch (payload["op"].toInt(-1)) {
        case Opcodes::Ready:
        {
            ip = data["ip"].toString();
            port = data["port"].toInt();
            modes = data["modes"].toArray().toVariantList();
            ssrc = data["ssrc"].toVariant().toULongLong();

            unsigned char discovery[74] = {0};
            (*(quint16*)(discovery)) = qToBigEndian(quint16(0x01));
            (*(quint16*)(discovery + 2)) = qToBigEndian(quint16(70));
            (*(quint32*)(discovery + 4)) = qToBigEndian(ssrc);
            
            udp.connectToHost(QHostAddress(ip),port);
            udp.waitForConnected();
            udp.write(QByteArray((const char *)discovery, 74));
            break;
        }

        case Opcodes::SessionDescription:
        {
            QJsonArray secretKeyJson = data["secret_key"].toArray();
            for (unsigned int i = 0 ; i < 32 ; i++) {
                secretKey[i] = (quint8)secretKeyJson[i].toInt();
            }
            break;
        }

        case Opcodes::HeartbeatACK:
        {
            nonce = data.toVariant().toULongLong();
            break;
        }

        case Opcodes::Hello:
        {
            connected = true;

            // Start Heartbeating
            int heartbeatInterval = data["heartbeat_interval"].toInt(45000);
            send(Opcodes::Heartbeat, "0");
            QThread *heartbeatThread = QThread::create([this, heartbeatInterval](){
                while (connected) {
                    send(Opcodes::Heartbeat, QString::number(nonce));

                    QThread::msleep(heartbeatInterval);
                }
            });
            heartbeatThread->start();
            break;
        }
    }
}

void VoiceSocket::readPendingDatagrams()
{
    while (udp.hasPendingDatagrams()) {
        QNetworkDatagram datagram = udp.receiveDatagram(1000);
        if (externalIp.isNull()) {
            externalIp = QString(datagram.data().mid(8, 64));
            QByteArray portBytes = datagram.data().mid(72, 2);
            hostPort = qFromBigEndian(portBytes[0] * 16 + portBytes[1]);
            send(Opcodes::SelectProtocol, 
                "{\"protocol\":\"udp\","
                "\"data\":{"
                    "\"address\":\"" + externalIp + "\","
                    "\"port\":" + QString::number(hostPort) + ","
                    "\"mode\":\"xsalsa20_poly1305\""
                "}}");
        }
    }
}

void const VoiceSocket::send(Opcodes op, const QString& data)
{
    // Build the payload string
    QString payload = "{\"op\":" + QString::number(op) + ",\"d\":" + data + "}";
    //qDebug() << "⇧" << payload;
    // Send the message
    client.sendTextMessage(payload);
}

} // namespace APi