#include "../../include/api/gateway.h"

#include "../../include/api/request.h"
#include "../../include/api/jsonutils.h"
#include "../../include/token.h"

#include <thread>
#include <zlib.h>

namespace Api {

Gateway::Gateway()
{
    //Member initialization
    seq = 0;
    connected = false;
    resuming = false;

    //Getting the websocket URL
    MemoryStruct response;
    request(
        "https://discord.com/api/v9/gateway",
        "",
        &response,
        "",
        ""
    );
    url = json::parse(response.memory).value("url", "");
}

//Connect to the gateway
void Gateway::connect()
{
    client.connect(url + "?v=9&encoding=json");
}

//Set the handlers and connect
void Gateway::start()
{
    client.set_message_handler([&](web::websockets::client::websocket_incoming_message msg){
        try {
            onMessage(msg); //Process messages
        }
        catch (const std::exception& e) {}
    });
    client.set_close_handler([&](web::websockets::client::websocket_close_status/* status*/, const utility::string_t &/*reason*/, const std::error_code &/*code*/)
    {
        //Reconnect when the connection is closed
        if (connected) {
            connected = false;
            //connect();
        }
    });
    connect();
}

//Set the callback function called when the gateway recieve events
void Gateway::onDispatch(std::function<void(std::string&, json&)> callback)
{
    onDispatchHandler = callback;
}


//Send data through the gateway
void Gateway::send(int op, const std::string& data)
{
    std::string payload = "{\"op\":" + std::to_string(op) + ",\"d\":" + data + "}"; //Build the payload string

    //Creating the message
    web::websockets::client::websocket_outgoing_message message;
    message.set_utf8_message(payload);

    //Send it
    client.send(message);
}

//Process the messages that the gateway recieve
void Gateway::onMessage(const web::websockets::client::websocket_incoming_message message)
{
    std::string messageStr;

    //Convert the message to string if it is binary
    if (message.message_type() == web::websockets::client::websocket_message_type::binary_message) {
        Concurrency::streams::container_buffer<std::string> strbuf;

        auto task = message.body().read_to_end(strbuf).then([strbuf](size_t) {
            return strbuf.collection();
        });

        z_stream zs;
        memset(&zs, 0, sizeof(zs));

        if (inflateInit(&zs) != Z_OK) {
            return;
        }

        std::string compressed = task.get();

        zs.next_in = reinterpret_cast<Bytef *>(const_cast<char *>(compressed.data()));
        zs.avail_in = static_cast<uInt>(compressed.size());

        int ret;
        char buffer[32768];

        do {
            zs.next_out = reinterpret_cast<Bytef *>(buffer);
            zs.avail_out = sizeof(buffer);

            ret = inflate(&zs, 0);

            if (messageStr.size() < zs.total_out) {
                messageStr.append(buffer, zs.total_out - messageStr.size());
            }
        } while (ret == Z_OK);

        inflateEnd(&zs);

        if (ret != Z_STREAM_END) {
            return;
        }
    } else {
        messageStr = message.extract_string().get();
    }

    json payload = json::parse(messageStr);
    json& data = payload.at("d");

    //https://discord.com/developers/docs/topics/opcodes-and-status-codes#gateway-opcodes
    switch (payload.value("op", -1)) {
        case 0: //Dispacth, event recieved
            seq = payload.at("s");
            dispatch(payload.at("t"), data);
            break;
        case 7: //Reconnect
            resume();
            break;
        case 9: //Invalid Session
            identify();
            break;
        case 10: //Hello
            connected = true;

            //Start Heartbeating
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

//Identifying to the gateway
void Gateway::identify()
{
    send(2, "{\"token\":\"" + token +"\","
        "\"capabilities\":125,"
        "\"properties\":{"
            "\"os\":\"Linux\","
            "\"browser\":\"Chrome\","
            "\"device\":\"\","
            "\"system_locale\":\"fr\","
            "\"browser_user_agent\":\"Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/93.0.4577.63 \"," // Randomised User-Agent
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

//Resume connection
void Gateway::resume()
{
    send(6, "{\"token\":\""+ token +"\","
        "\"session_id\":,"
        "\"seq\":" + std::to_string(seq) + "}");
}

//Function Launched in a thread to send heartbeat messages
void Gateway::heartbeatLoop()
{
    while (connected) {
        send(1, std::to_string(seq));

        std::this_thread::sleep_for(std::chrono::milliseconds(heartbeatInterval));
    }
}

//Internal function used to process some messages
void Gateway::dispatch(std::string eventName, json& data)
{
    if (eventName == "READY") {
        sessionId = data.at("session_id");
    }
    onDispatchHandler(eventName, data);
}

} // namespace Api
