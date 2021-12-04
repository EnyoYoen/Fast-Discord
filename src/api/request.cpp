#include "api/request.h"

#include "api/jsonutils.h"

#include <curl/curl.h>
#include <boost/filesystem.hpp>

#include <stdlib.h>
#include <ctime>
#include <chrono>
#include <thread>

namespace Api {

std::queue<RequestParameters> Request::requestQueue;
std::mutex Request::lock;
std::condition_variable Request::requestWaiter;
std::thread Request::loop;
std::string Request::token;       // Authorization token
double Request::rateLimitEnd = 0; // No rate limit for now
bool Request::stopped = false;

// Store the data recieved
size_t Request::writeMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    size_t realsize = size * nmemb;
    MemoryStruct *mem = (MemoryStruct *)userp; // MemoryStruct that we passed, to store the data

    char *ptr = static_cast<char *>(realloc(mem->memory, mem->size + realsize + 1));
    if (ptr == nullptr) {
        /* out of memory */
        printf("Not enough memory (realloc returned NULL)\nMemory size : %lu\nMemory to allocate : %lu\n", mem->size, realsize);
        return 0;
    }

    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}

// Store data in a file
size_t Request::writeFileCallback(void *contents, size_t size, size_t nmemb, FILE *stream)
{
    size_t written = fwrite(contents, size, nmemb, stream);
    return written;
}

// If we don't need the response
size_t Request::noOutputCallback(void *, size_t size, size_t nmemb, void *)
{
    return size * nmemb;
}

void Request::RequestLoop()
{
    while (!stopped) {
        if (requestQueue.size() > 0) {
            do {
                RequestParameters parameters = requestQueue.front();

                // Very basic rate limit checker
                if (rateLimitEnd != 0) { // We were rate limited
                    double sleepDuration = rateLimitEnd - std::time(0); // Time to sleep

                    // We are currently rate limited
                    if (sleepDuration > 0) std::this_thread::sleep_for(std::chrono::duration<double>(sleepDuration));
                    rateLimitEnd = 0; // Reset rate limit
                }

                CURL *curl;
                CURLcode res;

                /* Init the curl session */
                curl = curl_easy_init();
                if (curl) {
                    MemoryStruct callbackStruct;
                    curl_mime *form = nullptr;

                    /* Set our custom set of headers */
                    curl_slist *headers = curl_slist_append(nullptr, ("Authorization: " + token).c_str());
                    if (parameters.json) {
                        headers = curl_slist_append(headers, "content-type: application/json");
                    }
                    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

                    /* Specify URL to get */
                    curl_easy_setopt(curl, CURLOPT_URL, parameters.url.c_str());

                    if (parameters.outputFile != "") {
                        // If cache directory is not created, create it
                        if (!boost::filesystem::exists(boost::filesystem::path("cache/"))) boost::filesystem::create_directory(boost::filesystem::path("cache"));
                        FILE *fp = fopen(parameters.outputFile.c_str(), "wb");
                        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeFileCallback);
                        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
                    } else {
                        callbackStruct.memory = static_cast<char *>(malloc(1));  /* Will be grown as needed by the realloc above */
                        callbackStruct.size = 0;    /* No data at this point */

                        /* Send all data to this function  */
                        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeMemoryCallback);

                        /* We pass our 'chunk' struct to the callback function */
                        curl_easy_setopt(curl, CURLOPT_WRITEDATA, static_cast<void *>(&callbackStruct));
                    }

                    if (parameters.postDatas != "") {
                        // Set POST data
                        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, parameters.postDatas.c_str());
                    }

                    if (parameters.customRequest != "") {
                        // Set custom request
                        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, parameters.customRequest.c_str());
                    }

                    if (parameters.fileName != "") {
                        /* Create the form */
                        form = curl_mime_init(curl);

                        /* Fill in the file upload field */
                        curl_mimepart *field = curl_mime_addpart(form);
                        curl_mime_name(field, "sendfile");
                        curl_mime_filedata(field, parameters.fileName.c_str());

                        /* Fill in the filename field */
                        field = curl_mime_addpart(form);
                        curl_mime_name(field, "filename");
                        curl_mime_data(field, parameters.fileName.c_str(), CURL_ZERO_TERMINATED);

                        /* Fill in the submit field too*/
                        field = curl_mime_addpart(form);
                        curl_mime_name(field, "submit");
                        curl_mime_data(field, "send", CURL_ZERO_TERMINATED);

                        curl_easy_setopt(curl, CURLOPT_MIMEPOST, form);
                    }

                    long httpCode = 0;
                    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);

                    /* Get it */
                    res = curl_easy_perform(curl);

                    /* Check for errors */
                    if (res != CURLE_OK) {
                        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
                    }

                    /* Cleanup cURL stuff */
                    curl_easy_cleanup(curl);

                    if (parameters.fileName != "") {
                        curl_mime_free(form);
                    }

                    if (httpCode == 429) { // We are rate limited
                        // Set the end of the rate limit
                        rateLimitEnd = std::time(0) + json::parse(callbackStruct.memory).value("retry_after", double(0));

                        // Reset our MemoryStruct
                        free(callbackStruct.memory);
                        callbackStruct.size = 0;

                        // Redo the request
                        requestApi(parameters);
                    } else {
                        switch (parameters.type) {
                            case GetUser:
                                {
                                    User *user;
                                    unmarshal<User>(json::parse(callbackStruct.memory), "", &user);
                                    parameters.callback(static_cast<void *>(user));
                                    break;
                                }
                            case GetGuilds:
                                {
                                    std::vector<Guild *> *guilds;
                                    unmarshalMultiple<Guild>(json::parse(callbackStruct.memory), "", &guilds);
                                    parameters.callback(static_cast<void *>(guilds));
                                    break;
                                }
                            case GetGuildChannels:
                                {
                                    std::vector<Channel *> *channels;
                                    unmarshalMultiple<Channel>(json::parse(callbackStruct.memory), "", &channels);
                                    parameters.callback(static_cast<void *>(channels));
                                    break;
                                }
                            case GetPrivateChannels:
                                {
                                    std::vector<Channel *> *privateChannels;
                                    unmarshalMultiple<Channel>(json::parse(callbackStruct.memory), "", &privateChannels);
                                    parameters.callback(static_cast<void *>(privateChannels));
                                    break;
                                }
                            case GetMessages:
                                {
                                    std::vector<Message *> *messages;
                                    unmarshalMultiple<Message>(json::parse(callbackStruct.memory), "", &messages);
                                    parameters.callback(static_cast<void *>(messages));
                                    break;
                                }
                            case GetClient:
                                {
                                    Client *client;
                                    unmarshal<Client>(json::parse(callbackStruct.memory), "", &client);/*
                                    std::function<void(void *)> callback = *parameters.callback;
                                    callback(static_cast<void *>(nullptr));*/
                                    parameters.callback(static_cast<void *>(client));
                                    break;
                                }
                            case GetClientSettings:
                                {
                                    ClientSettings *clientSettings;
                                    unmarshal<ClientSettings>(json::parse(callbackStruct.memory), "", &clientSettings);
                                    parameters.callback(static_cast<void *>(clientSettings));
                                    break;
                                }
                            case GetWsUrl:
                                {
                                    try {
                                        json jsonUrl = json::parse(callbackStruct.memory).at("url");
                                        if (jsonUrl.is_null())
                                            break;
                                        std::string url = std::string(jsonUrl.get<std::string>());
                                        parameters.callback(static_cast<void *>(&url));
                                    } catch(std::exception&) {
                                        break;
                                    }
                                break;
                                }
                            case GetImage:
                                {
                                    std::string imageFileName = parameters.outputFile;
                                    parameters.callback(static_cast<void *>(&imageFileName));
                                    break;
                                }
                        }
                        requestQueue.pop();
                    }
                }
            } while (!requestQueue.empty());
        } else {
            std::unique_lock<std::mutex> uniqueLock(lock);
            requestWaiter.wait(uniqueLock);
        }
    }
}

void Request::startLoop()
{
    loop = std::thread(RequestLoop);
}

void Request::stopLoop()
{
    stopped = true;
    loop.join();
    requestWaiter.notify_all();
}

void Request::requestApi(const RequestParameters &parameters)
{
    lock.lock();
    requestQueue.push(parameters);
    requestWaiter.notify_one();
    lock.unlock();
}

// Functions that request the API to retrieve data

void Request::getImage(std::function<void(void *)> callback, const std::string& url, const std::string& fileName)
{
    requestApi({
        callback,
        url,
        "",
        "",
        "",
        "cache/" + fileName,
        GetImage,
        false});
}

void Request::getPrivateChannels(std::function<void(void *)> callback)
{
    requestApi({
        callback,
        "https://discord.com/api/v9/users/@me/channels",
        "",        
        "",
        "",
        "",
        GetPrivateChannels,
        false});
}

void Request::getGuilds(std::function<void(void *)> callback)
{
    requestApi({
        callback,
        "https://discord.com/api/v9/users/@me/guilds",
        "",        
        "",
        "",
        "",
        GetGuilds,
        false});
}

void Request::getGuildChannels(std::function<void(void *)> callback, const std::string& id)
{
    requestApi({
        callback,
        "https://discord.com/api/v9/guilds/" + id + "/channels",
        "",        
        "",
        "",
        "",
        GetGuildChannels,
        false});
}

void Request::getMessages(std::function<void(void *)> callback, const std::string& channelId, unsigned int limit)
{
    limit = limit >= 50 ? 50 : limit;

    requestApi({
        callback,
        "https://discord.com/api/v9/channels/" + channelId + "/messages?limit=" + std::to_string(limit),
        "",        
        "",
        "",
        "",
        GetMessages,
        false});
}

void Request::getClient(std::function<void(void *)> callback)
{
    requestApi({
        callback,
        "https://discord.com/api/v9/users/@me",
        "",        
        "",
        "",
        "",
        GetClient,
        false});
}

void Request::getClientSettings(std::function<void(void *)> callback)
{
    requestApi({
        callback,
        "https://discord.com/api/v9/users/@me/settings",
        "",        
        "",
        "",
        "",
        GetClientSettings,
        false});
}

void Request::getUser(std::function<void(void *)> callback, const std::string& userId)
{
    requestApi({
        callback,
        "https://discord.com/api/v9/users/" + userId,
        "",
        "",
        "",
        "",
        GetUser,
        false});
}

// Functions that request the API to send data

void Request::setStatus(const std::string& status)
{
    requestApi({
        nullptr,
        "https://discord.com/api/v9/users/@me/settings",
        "{\"status\":\"" + status + "\"}",
        "PATCH",
        "",
        "",
        SetStatus,
        false});
}

void Request::sendTyping(const std::string& channelId)
{
    requestApi({
            nullptr,
            "https://discord.com/api/v9/channels/" + channelId + "/typing",
            "",
            "POST",
            "",
            "",
            SendTyping,
            false});
}

void Request::sendMessage(const std::string& content, const std::string& channelId)
{
    requestApi({
            nullptr,
            "https://discord.com/api/v9/channels/" + channelId + "/messages",
            "{\"content\":\"" + content + "\"}",
            "",
            "",
            "",
            SendMessage,
            true});
}

void Request::sendMessageWithFile(const std::string& content, const std::string& channelId, const std::string& filePath)
{
    requestApi({
            nullptr,
            "https://discord.com/api/v9/channels/" + channelId + "/messages",
            "{\"content\":\"" + content + "\"}",
            "",
            filePath,
            "",
            SendMessageWithFile,
            false});
}

void Request::deleteMessage(const std::string& channelId, const std::string& messageId)
{
    requestApi({
        nullptr,
        "https://discord.com/api/v9/channels/" + channelId + "/messages/" + messageId,
        "",
        "DELETE",
        "",
        "",
        DeleteMessage,
        false});
}

void Request::pinMessage(const std::string& channelId, const std::string& messageId)
{
    requestApi({
        nullptr,
        "https://discord.com/api/v9/channels/" + channelId + "/pins/" + messageId,
        "",
        "PUT",
        "",
        "",
        PinMessage,
        false});
}

void Request::unpinMessage(const std::string& channelId, const std::string& messageId)
{
    requestApi({
        nullptr,
        "https://discord.com/api/v9/channels/" + channelId + "/pins/" + messageId,
        "",
        "PUT",
        "",
        "",
        UnpinMessage,
        false});
}

} // namespace Api
