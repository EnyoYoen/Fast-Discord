#pragma once

#include "api/objects/snowflake.h"

#include <QObject>
#include <QString>
#include <QtWebSockets/QtWebSockets>
#include <QUdpSocket>

namespace Api {

class VoiceSocket : public QObject
{
    Q_OBJECT
public:
    VoiceSocket();
    
    void start(const QString& endpoint, const Snowflake& serverId, const Snowflake& userId, const QString& sessionId, const QString& token);

private slots:
    void closeHandler();
    void identify();
    void processTextMessage(const QString& message);
    void readPendingDatagrams();

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
        ClientDisconnect = 13
    };

    void const send(Opcodes op, const QString& data);

    QWebSocket client;
    QUdpSocket udp;
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
    quint32 ssrc;
    quint16 port;
    quint16 hostPort;
    bool connected;
};

} // namespace Api