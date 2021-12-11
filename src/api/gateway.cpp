#include "api/gateway.h"

#include "api/jsonutils.h"

#include <thread>

namespace Api {

Gateway::Gateway(Api::Requester *requester, const std::string& tokenp)
    : QObject()
{
    // Member initialization
    seq = 0;
    connected = false;
    resuming = false;
    token = tokenp;

    // Getting the websocket URL
    requester->requestApi({
        [this](void *urlp) {
            url = *static_cast<std::string *>(urlp);
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
void Gateway::start()
{
    QObject::connect(&client, SIGNAL(aboutToClose()), this, SLOT(closeHandler()));
    QObject::connect(&client, SIGNAL(connected()), this, SLOT(identify()));
    QObject::connect(&client, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(errored(QAbstractSocket::SocketError)));
    QObject::connect(&client, SIGNAL(binaryMessageReceived(const QByteArray&)), this, SLOT(processBinaryMessage(const QByteArray&)));
    QObject::connect(&client, SIGNAL(textMessageReceived(const QString&)), this, SLOT(processTextMessage(const QString&)));

    client.open(QUrl(QString(url.c_str()) + "/" + QString("?v=9&encoding=json")));
}

void Gateway::errored(QAbstractSocket::SocketError err)
{
    qDebug() << err;
}

void Gateway::closeHandler()
{
    // Reconnect when the connection is closed
    if (connected) {
        connected = false;
        client.open(QUrl(QString(url.c_str()) + QString("?v=9&encoding=json")));
    }
}

// Set the callback function called when the gateway recieve events
void Gateway::onDispatch(std::function<void(std::string&, json&)> callback)
{
    onDispatchHandler = callback;
}


// Send data through the gateway
void Gateway::send(int op, const std::string& data)
{
    // Build the payload string
    std::string payload = "{\"op\":" + std::to_string(op) + ",\"d\":" + data + "}";

    // Send the message
    client.sendTextMessage(QString(payload.c_str()));
}

// Process a binary message that the gateway recieves
void Gateway::processBinaryMessage(const QByteArray& message)
{
    json payload = json::parse(QString(qUncompress(message)).toUtf8().constData());
    json& data = payload.at("d");

    switch (payload.value("op", -1)) {
        case Dispatch: //Event recieved
            seq = payload.at("s");
            dispatch(payload.at("t"), data);
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
            heartbeatInterval = data.value("heartbeat_interval", 45000);
            std::thread heartbeatThread = std::thread(&Gateway::heartbeatLoop, this);
            heartbeatThread.detach();

            if (resuming) {
                resume();
            } else {
                identify();
                resuming = true;
            }
            break;
    }
}

// Process a text message that the gateway recieves
void Gateway::processTextMessage(const QString& message)
{
    json payload = json::parse(message.toUtf8().constData());
    json& data = payload.at("d");

    switch (payload.value("op", -1)) {
        case Dispatch: //Event recieved
            seq = payload.at("s");
            dispatch(payload.at("t"), data);
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
            heartbeatInterval = data.value("heartbeat_interval", 45000);
            std::thread heartbeatThread = std::thread(&Gateway::heartbeatLoop, this);
            heartbeatThread.detach();

            if (resuming) {
                resume();
            } else {
                identify();
                resuming = true;
            }
            break;
    }
}

// Identifying to the gateway
void Gateway::identify()
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
void Gateway::resume()
{
    send(Resume, "{\"token\":\""+ token +"\","
                 "\"session_id\":,"
                 "\"seq\":" + std::to_string(seq) + "}");
}

// Function Launched in a thread to send heartbeat messages
void Gateway::heartbeatLoop()
{
    while (connected) {
        send(1, std::to_string(seq));

        std::this_thread::sleep_for(std::chrono::milliseconds(heartbeatInterval));
    }
}

// Internal function used to process some messages
void Gateway::dispatch(std::string eventName, json& data)
{
    if (eventName == "READY") {
        sessionId = data.at("session_id");
    }
    onDispatchHandler(eventName, data);
}

} // namespace Api
