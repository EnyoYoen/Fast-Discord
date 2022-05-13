#include "api/gateway.h"

#include "api/jsonutils.h"

#include <QThread>

namespace Api {

Gateway::Gateway(Api::Requester *requester, const QString& tokenp)
    : QObject()
{
    // Member initialization
    seq = 0;
    waitVoiceInfos = 0;
    connected = false;
    resuming = false;
    token = tokenp;

    // Getting the websocket URL
    requester->requestApi({
        [this](void *urlp) {
            url = *static_cast<QString *>(urlp);
            this->start();
        },
        "https://discord.com/api/v9/gateway",
        "",
        "",
        "",
        "",
        GetWsUrl,
        false
    });
}

// Set the handlers and connect
void const Gateway::start()
{
    QObject::connect(&client, &QWebSocket::aboutToClose, this, &Gateway::closeHandler);
    QObject::connect(&client, &QWebSocket::connected, this, &Gateway::identify);
    QObject::connect(&client, &QWebSocket::binaryMessageReceived, this, &Gateway::processBinaryMessage);
    QObject::connect(&client, &QWebSocket::textMessageReceived, this, &Gateway::processTextMessage);

    client.open(QUrl(url + "/?v=9&encoding=json"));
}

void const Gateway::closeHandler()
{
    // Reconnect when the connection is closed
    if (connected) {
        connected = false;
        client.open(QUrl(url + "?v=9&encoding=json"));
    }
}

// Set the callback function called when the gateway recieve events
void Gateway::onDispatch(const DispatchCallback& callback)
{
    onDispatchHandler = callback;
}

void const Gateway::sendGuildChannelOpened(const QMap<Snowflake, QVector<QVector<int>>>& channels, const Snowflake& guildId, bool activities, bool threads, bool typing)
{
    QString data = "{\"guild_id\":\"" + guildId + "\"" + (typing ? ",\"typing\":true" : "") + (activities ? ",\"activities\":true" : "") + (threads ? ",\"threads\":true" : "") + ",\"channels\":{";

    unsigned int counter = 0;
    for (auto it = channels.begin() ; it != channels.end() ; it++, counter++) {
        data += QString(counter > 0 ? "," : "") + "\"" + it.key() + "\":[";
        QVector<QVector<int>> messagesNumbers = it.value();
        for (unsigned int i = 0 ; i < messagesNumbers.size() ; i++) {
            data += "[";
            for (unsigned int j = 0 ; j < messagesNumbers[i].size() ; j++) {
                data += QString::number(messagesNumbers[i][j]) + (j + 1 < messagesNumbers[i].size() ? "," : "");
            }
            data += "]" + QString(i + 1 < messagesNumbers.size() ? "," : "");
        }
        data += "]";
    }

    send(GuildChannelOpened, data + "}}");
}

void const Gateway::sendDMChannelOpened(const Snowflake& channelId)
{
    send(DMChannelOpened, "{\"channel_id\":\"" + channelId + "\"}");
}

void const Gateway::sendVoiceStateUpdate(const Snowflake& guildId, const Snowflake& channelId, bool selfMute, bool selfDeaf)
{
    send(VoiceStateUpdate, "{\"guild_id\":" + (guildId == 0 ? "null" : "\"" + guildId + "\"") + ","
                            "\"channel_id\":" + (channelId == 0 ? "null" : "\"" + channelId + "\"") + ","
                            "\"self_mute\":" + (selfMute ? "true" : "false") + ","
                            "\"self_deaf\":" + (selfDeaf ? "true" : "false") + "}");
}

void const Gateway::sendCall(VoiceCallback callback, const Snowflake& guildId, const Snowflake& channelId, bool selfMute, bool selfDeaf)
{
    if (!waitVoiceInfos) {
        voiceSessionId.clear();
        voiceEndpoint.clear();
        voiceToken.clear();
        voiceCallback = callback;
        waitVoiceInfos = true;
        send(VoiceStateUpdate, "{\"guild_id\":\"" + guildId + "\","
                                "\"channel_id\":\"" + channelId + "\","
                                "\"self_mute\":" + (selfMute ? "true" : "false") + ","
                                "\"self_deaf\":" + (selfDeaf ? "true" : "false") + "}");
    }
}

// Send data through the gateway
void const Gateway::send(int op, const QString& data)
{
    // Build the payload string
    QString payload = "{\"op\":" + QString::number(op) + ",\"d\":" + data + "}";
    //qDebug() << "⇧" << payload.mid(0, 198);
    // Send the message
    client.sendTextMessage(payload);
}

// Process a binary message that the gateway recieves
void const Gateway::processBinaryMessage(const QByteArray& message)
{
    //qDebug() << "⇩" << qUncompress(message).mid(0, 198);
    QJsonDocument payload = QJsonDocument::fromJson(qUncompress(message));
    QJsonValue data = payload["d"];

    switch (payload["op"].toInt(-1)) {
        case Dispatch: //Event recieved
            seq = payload["s"].toInt();
            dispatch(payload["t"].toString(), data);
            break;
        case Reconnect:
            resume();
            break;
        case InvalidSession:
            identify();
            break;
        case Hello:
            connected = true;

            // Start Heartbeating
            heartbeatInterval = data["heartbeat_interval"].toInt(45000);
            QThread *heartbeatThread = QThread::create([this](){
                while (connected) {
                    send(Heartbeat, QString::number(seq));

                    QThread::msleep(heartbeatInterval);
                }
            });
            heartbeatThread->start();

            if (resuming) {
                resume();
            } else {
                //identify();
                resuming = true;
            }
            break;
    }
}

// Process a text message that the gateway recieves
void const Gateway::processTextMessage(const QString& message)
{
    //qDebug() << "⇩" << message.mid(0, 198);
    QJsonDocument payload = QJsonDocument::fromJson(message.toUtf8());
    QJsonValue data = payload["d"];

    switch (payload["op"].toInt(-1)) {
        case Dispatch: //Event recieved
            seq = payload["s"].toInt();
            dispatch(payload["t"].toString(), data);
            break;
        case Reconnect:
            resume();
            break;
        case InvalidSession:
            identify();
            break;
        case Hello:
            connected = true;

            // Start Heartbeating
            heartbeatInterval = data["heartbeat_interval"].toInt(45000);
            QThread *heartbeatThread = QThread::create([this](){
                while (connected) {
                    send(1, QString::number(seq));

                    QThread::msleep(heartbeatInterval);
                }
            });
            heartbeatThread->start();

            if (resuming) {
                resume();
            } else {
                //identify();
                resuming = true;
            }
            break;
    }
}

// Identifying to the gateway
void const Gateway::identify()
{
    send(Identify, "{\"token\":\"" + token +"\","
        "\"capabilities\":125,"
        "\"properties\":{"
            "\"os\":\"Linux\","
            "\"browser\":\"Chrome\","
            "\"device\":\"\","
            "\"system_locale\":\"fr\","
            "\"browser_user_agent\":\"Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/93.0.4577.63 \"," // Random generated User-Agent
            "\"browser_version\":\"93.0\","
            "\"os_version\":\"\","
            "\"referrer\":\"\","
            "\"referring_domain\":\"\","
            "\"referrer_current\":\"\","
            "\"referring_domain_current\":\"\","
            "\"release_channel\":\"stable\","
            "\"client_event_source\":null"
        "},"
        "\"presence\":{"
            "\"status\":\"online\","
            "\"since\":0,"
            "\"activities\":[],"
            "\"afk\":false"
        "},"
        "\"compress\":false,"
        "\"client_state\":{"
            "\"guild_hashes\":{},"
            "\"highest_last_message_id\":\"0\","
            "\"read_state_version\":0,"
            "\"user_guild_settings_version\":-1"
        "}}");
}

// Resume connection
void const Gateway::resume()
{
    send(Resume, "{\"token\":\""+ token +"\","
                 "\"session_id\":,"
                 "\"seq\":" + QString::number(seq) + "}");
}

// Internal function used to process some messages
void Gateway::dispatch(QString eventName, json& data)
{
    if (waitVoiceInfos) {
        if (eventName == "VOICE_STATE_UPDATE") {
            voiceSessionId = data["session_id"].toString();
            if (!voiceEndpoint.isNull() && !voiceToken.isNull()) {
                waitVoiceInfos = false;
                voiceCallback(voiceSessionId, voiceEndpoint, voiceToken);
            }
        } else if (eventName == "VOICE_SERVER_UPDATE") {
            voiceEndpoint = data["endpoint"].toString();
            voiceToken = data["token"].toString();
            if (!voiceEndpoint.isNull()) {
                waitVoiceInfos = false;
                voiceCallback(voiceSessionId, voiceEndpoint, voiceToken);
            }
        }
    } else if (eventName == "READY") {
        sessionId = data["session_id"].toString();
    }
    onDispatchHandler(eventName, data);
}

} // namespace Api
