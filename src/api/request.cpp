#include "../../include/api/request.h"

#include "../../include/api/jsonutils.h"

#include <curl/curl.h>
#include <boost/filesystem.hpp>

#include <stdlib.h>

namespace Api {

std::string Request::token;

size_t Request::writeMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    size_t realsize = size * nmemb;
    MemoryStruct *mem = (MemoryStruct *)userp;

    char *ptr = static_cast<char *>(realloc(mem->memory, mem->size + realsize + 1));
    if (ptr == nullptr) {
        /* out of memory! */
        printf("Not enough memory (realloc returned NULL)\nMemory size : %lu\nMemory to allocate : %lu\n", mem->size, realsize);
        return 0;
    }

    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}

size_t Request::writeFileCallback(void *contents, size_t size, size_t nmemb, FILE *stream)
{
    size_t written = fwrite(contents, size, nmemb, stream);
    return written;
}

size_t Request::noOutputCallback(void *, size_t size, size_t nmemb, void *)
{
    return size * nmemb;
}

void Request::requestApi(const std::string& url, const std::string& postDatas, MemoryStruct *callbackStruct, const std::string& customRequest, const std::string& fileName, const std::string& outputFile, bool json)
{
    CURL *curl;
    CURLcode res;

    /* init the curl session */
    curl = curl_easy_init();
    if (curl) {
        curl_mime *form = nullptr;

        /* set our custom set of headers */
        curl_slist *headers = curl_slist_append(nullptr, ("Authorization: " + token).c_str());
        if (json) {
            headers = curl_slist_append(headers, "content-type: application/json");
        }
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        /* specify URL to get */
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

        if (outputFile != "") {
            if (!boost::filesystem::exists(boost::filesystem::path("cache/"))) boost::filesystem::create_directory(boost::filesystem::path("cache"));
            FILE *fp = fopen(outputFile.c_str(), "wb");
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeFileCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        } else {
            if (callbackStruct != nullptr) {
                callbackStruct->memory = static_cast<char *>(malloc(1));  /* will be grown as needed by the realloc above */
                callbackStruct->size = 0;    /* no data at this point */

                /* send all data to this function  */
                curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeMemoryCallback);

                /* we pass our 'chunk' struct to the callback function */
                curl_easy_setopt(curl, CURLOPT_WRITEDATA, static_cast<void *>(callbackStruct));
            } else {
                curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, noOutputCallback);
            }
        }

        if (postDatas != "") {
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postDatas.c_str());
        }

        if (customRequest != "") {
            curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, customRequest.c_str());
        }

        if (fileName != "") {
            /* Create the form */
            form = curl_mime_init(curl);

            /* Fill in the file upload field */
            curl_mimepart *field = curl_mime_addpart(form);
            curl_mime_name(field, "sendfile");
            curl_mime_filedata(field, fileName.c_str());

            /* Fill in the filename field */
            field = curl_mime_addpart(form);
            curl_mime_name(field, "filename");
            curl_mime_data(field, fileName.c_str(), CURL_ZERO_TERMINATED);

            /* Fill in the submit field too*/
            field = curl_mime_addpart(form);
            curl_mime_name(field, "submit");
            curl_mime_data(field, "send", CURL_ZERO_TERMINATED);

            curl_easy_setopt(curl, CURLOPT_MIMEPOST, form);
        }

        /* get it! */
        res = curl_easy_perform(curl);

        /* check for errors */
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }

        /* cleanup curl stuff */
        curl_easy_cleanup(curl);

        if (fileName != "") {
            curl_mime_free(form);
        }
    }
}

void Request::requestJson(const std::string& url, const std::string& postDatas, MemoryStruct *callbackStruct, const std::string& customRequest, const std::string& fileName)
{
    requestApi(url, postDatas, callbackStruct, customRequest, fileName, "", true);
}

void Request::requestFile(const std::string& url, const std::string& fileName)
{
    requestApi(url, "", nullptr, "", "", fileName, false);
}


std::vector<Channel *> *Request::getPrivateChannels()
{
    MemoryStruct response;

    requestApi(
        "https://discord.com/api/v9/users/@me/channels",
        "",
        &response,
        "",
        "",
        "",
        false);

    return getChannelsFromJson(json::parse(response.memory), "");
}

std::vector<Guild *> *Request::getGuilds()
{
    MemoryStruct response;

    requestApi(
        "https://discord.com/api/v9/users/@me/guilds",
        "",
        &response,
        "",
        "",
        "",
        false);

    return getGuildsFromJson(json::parse(response.memory), "");
}

std::vector<Channel *> *Request::getGuildChannels(const std::string& id)
{
    MemoryStruct response;

    requestApi(
        "https://discord.com/api/v9/guilds/" + id + "/channels",
        "",
        &response,
        "",
        "",
        "",
        false);

    return getChannelsFromJson(json::parse(response.memory), "");
}

std::vector<Message *> *Request::getMessages(const std::string& channelId, unsigned int limit)
{
    MemoryStruct response;

    limit = limit >= 100 ? 100 : limit;

    requestApi(
        "https://discord.com/api/v9/channels/" + channelId + "/messages?limit=" + std::to_string(limit),
        "",
        &response,
        "",
        "",
        "",
        false);

    return getMessagesFromJson(json::parse(response.memory), "");
}

void Request::setStatus(const std::string& status)
{
    requestApi(
        "https://discord.com/api/v9/users/@me/settings",
        "{\"status\":\"" + status + "\"}",
        nullptr,
        "PATCH",
        "",
        "",
        false);
}

void Request::sendTyping(const std::string& channelId)
{
    requestApi("https://discord.com/api/v9/channels/" + channelId + "/typing",
            " ",
            nullptr,
            "POST",
            "",
            "",
            false);
}

void Request::sendMessage(const std::string& content, const std::string& channelId)
{
    requestJson("https://discord.com/api/v9/channels/" + channelId + "/messages",
            "{\"content\":\"" + content + "\"}",
            nullptr,
            "",
            "");
}

void Request::sendMessageWithFile(const std::string& content, const std::string& channelId, const std::string& filePath)
{
    requestApi("https://discord.com/api/v9/channels/" + channelId + "/messages",
            "{\"content\":\"" + content + "\"}",
            nullptr,
            "",
            filePath,
            "",
            false);
}

void Request::deleteMessage(const std::string& channelId, const std::string& messageId)
{
    requestApi(
        "https://discord.com/api/v9/channels/" + channelId + "/messages/" + messageId,
        "",
        nullptr,
        "DELETE",
        "",
        "",
        false);
}

void Request::pinMessage(const std::string& channelId, const std::string& messageId)
{
    requestApi(
        "https://discord.com/api/v9/channels/" + channelId + "/pins/" + messageId,
        "",
        nullptr,
        "PUT",
        "",
        "",
        false);
}

void Request::unpinMessage(const std::string& channelId, const std::string& messageId)
{
    requestApi(
        "https://discord.com/api/v9/channels/" + channelId + "/pins/" + messageId,
        "",
        nullptr,
        "PUT",
        "",
        "",
        false);
}

} // namespace Api
