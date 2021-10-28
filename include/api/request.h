#pragma once

#include "channel.h"
#include "guild.h"
#include "message.h"

#include <string>
#include <vector>
#include <cstddef>

namespace Api {

typedef struct
{
    char *memory;
    size_t size;
} MemoryStruct;

//Class to request to the api
class Request
{
public:
    static std::string token;

    static void requestApi(const std::string& url, const std::string& postDatas, MemoryStruct *callbackStruct, const std::string& customRequest, const std::string& fileName, const std::string& outputFile, bool json);
    static void requestJson(const std::string& url, const std::string& postDatas, MemoryStruct *callbackStruct, const std::string& customRequest, const std::string& fileName);
    static void requestFile(const std::string& url, const std::string& fileName);

    static std::vector<Guild *>   *getGuilds();
    static std::vector<Channel *> *getGuildChannels(const std::string& id);
    static std::vector<Channel *> *getPrivateChannels();
    static std::vector<Message>   *getMessages(const std::string& channelId, unsigned int limit);

    static void setStatus(const std::string& status);
    static void sendTyping(const std::string& channelId);
    static void sendMessage(const std::string& content, const std::string& channelId);
    static void sendMessageWithFile(const std::string& content, const std::string& channelId, const std::string& filePath);
    static void deleteMessage(const std::string& channelId, const std::string& messageId);
    static void pinMessage(const std::string& channelId, const std::string& messageId);
    static void unpinMessage(const std::string& channelId, const std::string& messageId);

private:
    Request();

    static size_t writeMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp);
    static size_t writeFileCallback(void *contents, size_t size, size_t nmemb, FILE *stream);
    static size_t noOutputCallback(void *, size_t size, size_t nmemb, void *);
};

} // namespace Api
