#pragma once

#include "api/objects/snowflake.h"

#include <QObject>
#include <QString>
#include <QBuffer>
#include <QTimer>
#include <QtWebSockets/QtWebSockets>
#include <QUdpSocket>
#include <QAudioInput>
#include <QAudioOutput>
#include <QAudioFormat>
#include <QAudioDeviceInfo>
#include <opus/opus.h>

#define FRAME_SIZE 960
//#define FRAME_SIZE 2880
#define SAMPLE_RATE 48000
#define CHANNELS 2
#define APPLICATION OPUS_APPLICATION_VOIP
#define BITRATE 64000
#define MAX_FRAME_SIZE (6*FRAME_SIZE)
//#define MAX_FRAME_SIZE 11520
#define MAX_PACKET_SIZE 16384

namespace Api {

class VoiceSocket : public QObject
{
    Q_OBJECT
public:
    VoiceSocket();
    
    void start(const QString& endpoint, const Snowflake& serverId, const Snowflake& userId, const QString& sessionId, const QString& token);
    void stop();

    bool mute;
    bool deaf;

private slots:
    void closeHandler();
    void identify();
    void processTextMessage(const QString& message);
    void readPendingDatagrams();
    void test(qint64);

private:
    enum Opcodes {
        Identify,
        SelectProtocol,
        Ready,
        Heartbeat,
        SessionDescription,
        Speaking,
        HeartbeatACK,
        Resume,
        Hello,
        Resumed,
        ClientConnect = 12,
        ClientDisconnect,
        Unknown = 16
    };

    void const send(Opcodes op, const QString& data);
    void const sendAudio(QByteArray data, qint64 length);
    void const sendSilence();
    int const encode(quint8 *rawData, int rawDataSize, quint8 *encodedData);
    void resume();

    quint8 encodeBuffer[MAX_PACKET_SIZE];
    QWebSocket client;
    QUdpSocket udp;
    QTimer timer;
    QThread *heartbeatThread;
    QAudioInput *input;
    QAudioOutput *output;
    QIODevice *mic;
    QIODevice *speaker;
    OpusEncoder *encoder;
    OpusDecoder *decoder;
    OpusRepacketizer *repacketizer;
    QBuffer rdBuff;
    QBuffer wrBuff;
    QAudioFormat inFormat;
    QAudioFormat outFormat;
    QVariantList modes;
    quint8 secretKey[32];
    QString endpoint;
    QString ip;
    QString externalIp;
    QString sessionId;
    QString token;
    Snowflake serverId;
    Snowflake userId;
    quint64 nonce;
    quint32 timestamp;
    quint32 ssrc;
    quint16 port;
    quint16 hostPort;
    quint16 sequence;
    bool speaking;
    bool connected;
    bool udpConnected;
};

} // namespace Api