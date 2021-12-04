#pragma once

#include "channel.h"
#include "guild.h"
#include "message.h"
#include "client.h"
#include "ui/roundedimage.h"

#include <QBoxLayout>

#include <string>
#include <vector>
#include <queue>
#include <condition_variable>
#include <mutex>
#include <functional>
#include <thread>
#include <cstddef>

namespace Api {

enum RequestTypes {
    // We need the response data
    GetGuilds,
    GetGuildChannels,
    GetPrivateChannels,
    GetMessages,
    GetClient,
    GetClientSettings,
    GetImage,
    GetWsUrl,
    GetUser,

    // We don't care about the response
    SetStatus,
    SendTyping,
    SendMessage,
    SendMessageWithFile,
    DeleteMessage,
    PinMessage,
    UnpinMessage
};

struct MemoryStruct
{
    char *memory;
    size_t size;
};

struct RequestParameters
{
    std::function<void(void *)> callback;
    const std::string url;
    const std::string postDatas;
    const std::string customRequest;
    const std::string fileName;
    const std::string outputFile;
    int type;
    bool json;
};

//Class to request to the API
class Request
{
public:
    static std::string token; // Authorization token

    // Start and stop the loop for the requests
    static void startLoop();
    static void stopLoop();

    // Function that request the API
    static void requestApi(const RequestParameters& parameters);

    // Functions that request the API to retrieve data
    static void getGuilds(std::function<void(void *)> callback);
    static void getGuildChannels(std::function<void(void *)> callback, const std::string& id);
    static void getPrivateChannels(std::function<void(void *)> callback);
    static void getMessages(std::function<void(void *)> callback, const std::string& channelId, unsigned int limit);
    static void getClient(std::function<void(void *)> callback);
    static void getClientSettings(std::function<void(void *)> callback);
    static void getImage(std::function<void(void *)> callback, const std::string& url, const std::string& fileName);
    static void getUser(std::function<void(void *)> callback, const std::string& userId);

    // Functions that request the API to send data
    static void setStatus(const std::string& status);
    static void sendTyping(const std::string& channelId);
    static void sendMessage(const std::string& content, const std::string& channelId);
    static void sendMessageWithFile(const std::string& content, const std::string& channelId, const std::string& filePath);
    static void deleteMessage(const std::string& channelId, const std::string& messageId);
    static void pinMessage(const std::string& channelId, const std::string& messageId);
    static void unpinMessage(const std::string& channelId, const std::string& messageId);

private:
    static std::queue<RequestParameters> requestQueue;
                                // The queue of request parameters
    static std::mutex lock;
    static std::condition_variable requestWaiter;
                                // The loop waits when there is no request
    static std::thread loop;    // The request loop
    static double rateLimitEnd; // Unix time that represents the moment of the end of the rate limit
    static bool stopped;        // Used to stop the request loop

    Request(); // You can't create objects of this class

    // The function that contains the request loop
    static void RequestLoop();

    // Functions used by libcurl to process recieved data
    static size_t writeMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp);
    static size_t writeFileCallback(void *contents, size_t size, size_t nmemb, FILE *stream);
    static size_t noOutputCallback(void *, size_t size, size_t nmemb, void *);
};

} // namespace Api
