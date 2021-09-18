#include "../../include/api/request.h"

#include "../../include/token.h"

#include <curl/curl.h>

#include <cstring>
#include <stdlib.h>

namespace Api {

size_t writeMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    size_t realsize = size * nmemb;
    MemoryStruct *mem = (MemoryStruct *)userp;

    char *ptr = static_cast<char *>(realloc(mem->memory, mem->size + realsize + 1));
    if (ptr == nullptr) {
        /* out of memory! */
        printf("Not enough memory (realloc returned nullptr)\nMemory size : %lu\nMemory to allocate : %lu\n", mem->size, realsize);
        return 0;
    }

    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}

size_t writeFile(void *contents, size_t size, size_t nmemb, FILE *stream)
{
    size_t written = fwrite(contents, size, nmemb, stream);
    return written;
}

size_t noOutputFunction(void *buffer, size_t size, size_t nmemb, void *userp)
{
    return size * nmemb;
}

void _request(const std::string& url, const std::string& postDatas, MemoryStruct *callbackStruct, const std::string& customRequest, const std::string& fileName, const std::string& outputFile, bool json)
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
            FILE *fp = fopen(outputFile.c_str(), "wb");
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeFile);
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
                curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, noOutputFunction);
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

void request(const std::string& url, const std::string& postDatas, MemoryStruct *callbackStruct, const std::string& customRequest, const std::string& fileName)
{
    _request(url, postDatas, callbackStruct, customRequest, fileName, "", false);
}

void requestJson(const std::string& url, const std::string& postDatas, MemoryStruct *callbackStruct, const std::string& customRequest, const std::string& fileName)
{
    _request(url, postDatas, callbackStruct, customRequest, fileName, "", true);
}

void requestFile(const std::string& url, const std::string& fileName)
{
    _request(url, "", nullptr, "", "", fileName, false);
}

} // namespace Api
