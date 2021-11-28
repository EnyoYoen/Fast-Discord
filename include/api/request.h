#pragma once

#include "channel.h"
#include "guild.h"
#include "message.h"
#include "client.h"
#include "ui/roundedimage.h"

#include <QBoxLayout>

#include <string>
#include <vector>
#include <cstddef>

namespace Api {

typedef struct
{
    char *memory;
    size_t size;
} MemoryStruct;

//Class to request to the API
class Request
{
public:
    static std::string token; // Authorization token

    // Function that request the API
    static void requestApi(const std::string& url, const std::string& postDatas, MemoryStruct *callbackStruct, const std::string& customRequest, const std::string& fileName, const std::string& outputFile, bool json);

    // Functions that use requestApi withh less parameters
    static void requestJson(const std::string& url, const std::string& postDatas, MemoryStruct *callbackStruct, const std::string& customRequest, const std::string& fileName);
    static void requestFile(const std::string& url, const std::string& fileName);
    static void requestImage(const std::string& url, const std::string& fileName, Ui::RoundedImage *imageWidget);

    // Functions that request the API to retrieve data
    static std::vector<Guild *>   *getGuilds();
    static std::vector<Channel *> *getGuildChannels(const std::string& id);
    static std::vector<Channel *> *getPrivateChannels();
    static std::vector<Message *> *getMessages(const std::string& channelId, unsigned int limit);
    static Client                 *getClient();
    static ClientSettings         *getClientSettings();

    // Functions that request the API to send data
    static void setStatus(const std::string& status);
    static void sendTyping(const std::string& channelId);
    static void sendMessage(const std::string& content, const std::string& channelId);
    static void sendMessageWithFile(const std::string& content, const std::string& channelId, const std::string& filePath);
    static void deleteMessage(const std::string& channelId, const std::string& messageId);
    static void pinMessage(const std::string& channelId, const std::string& messageId);
    static void unpinMessage(const std::string& channelId, const std::string& messageId);

private:
    static double rateLimitEnd; // Unix time that represents the moment of the end of the rate limit

    Request(); // You can't create objects of this class

    // Functions used by libcurl to process recieved data
    static size_t writeMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp);
    static size_t writeFileCallback(void *contents, size_t size, size_t nmemb, FILE *stream);
    static size_t noOutputCallback(void *, size_t size, size_t nmemb, void *);
};

} // namespace Api
