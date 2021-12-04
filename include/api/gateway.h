#pragma once

#include "../../lib/nlohmann/json.hpp"

#include <cpprest/ws_client.h>

#include <string>

using json = nlohmann::json;

namespace Api {

//https://discord.com/developers/docs/topics/opcodes-and-status-codes#gateway-opcodes
enum Opcodes {
    Dispatch,
    Heartbeat,
    Identify,
    PresenceUpdate,
    VoiceStateUpdate,
    Resume,
    Reconnect,
    RequestGuildMembers,
    InvalidSession,
    Hello,
    HeartbeatACK
};

//https://discord.com/developers/docs/topics/gateway
class Gateway
{
public:
    Gateway();
    void onDispatch(std::function<void(std::string&, json&)> callback);
        //Sets the callback function called when the gateway recieve events

private:
    void start();
    void send(int op, const std::string& data);
                          //Send data through the gateway
    void onMessage(web::websockets::client::websocket_incoming_message message);
                          //Process the messages that the gateway recieve
    void resume();        //Resume connection
    void identify();      //Identifying to the gateway
    void heartbeat();     //Send Heartbeat message to stay connected
    void heartbeatLoop(); //Function Launched in a thread to send heartbeat messages
    void dispatch(std::string eventName, json& data);
                          //Internal function used to process some messages
    void connect();       //Connect to the gateway

    web::websockets::client::websocket_callback_client client; //websocket client
    std::function<void(std::string&, json&)> onDispatchHandler;
         //Function called when when the gateway recieve events
    std::string url; //websocket URL
    std::string sessionId;
    std::string token;
    int heartbeatInterval;
    int seq;
    bool connected;
    bool resuming;
};

} // namespace Api
