#pragma once

#include "api/request.h"

#include <QtWebSockets/QtWebSockets>
#include <QJsonValue>
#include <QString>
#include <QMap>

using json = QJsonValue;
typedef std::function<void(QString&, json&)> DispatchCallback;
typedef std::function<void(QString&, QString&, QString&)> VoiceCallback;

namespace Api {

//https://discord.com/developers/docs/topics/opcodes-and-status-codes#gateway-opcodes
enum Opcodes {
    Dispatch,
    Heartbeat,
    Identify,
    PresenceUpdate,
    VoiceStateUpdate,
    Resume = 6,
    Reconnect,
    RequestGuildMembers,
    InvalidSession,
    Hello,
    HeartbeatACK,
    DMChannelOpened = 13,
    GuildChannelOpened
};

//https://discord.com/developers/docs/topics/gateway
class Gateway : public QObject
{
    Q_OBJECT
public:
    Gateway(Api::Requester *requester, const QString& token);
    void onDispatch(const DispatchCallback& callback);
        //Sets the callback function called when the gateway recieve events

    void const sendGuildChannelOpened(const QMap<Snowflake, QVector<QVector<int>>>& channels, const Snowflake& guildId, bool activities, bool threads, bool typing);
    void const sendDMChannelOpened(const Snowflake& channelId);
    void const sendVoiceStateUpdate(const Snowflake& guildId, const Snowflake& channelId, bool selfMute, bool selfDeaf);
    void const sendCall(VoiceCallback callback, const Snowflake& guildId, const Snowflake& channelId, bool selfMute, bool selfDeaf);

private slots:
    void const closeHandler();
    void const identify();
    void const processBinaryMessage(const QByteArray& message);
                          // Process a binary message that the gateway recieves
    void const processTextMessage(const QString& message);
                          // Process a text message that the gateway recieves

private:
    void const start();
    void const send(int op, const QString& data);
                                //Send data through the gateway
    void const resume();        //Resume connection
    void const heartbeat();     //Send Heartbeat message to stay connected
    void dispatch(QString eventName, json& data);
                                //Internal function used to process some messages

    QWebSocket client;    // websocket client
    DispatchCallback onDispatchHandler;
         //Function called when when the gateway recieve events
    VoiceCallback voiceCallback;
    QString url;      //websocket URL
    QString sessionId;
    QString token;
    QString voiceSessionId;
    QString voiceEndpoint;
    QString voiceToken;
    int heartbeatInterval;
    int seq;
    bool connected;
    bool resuming;
    bool waitVoiceInfos;
};

} // namespace Api
