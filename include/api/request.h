#pragma once

#include <string>
#include <cstddef>

namespace Api {

typedef struct
{
    char *memory;
    size_t size;
} MemoryStruct;

//Functions to request to the api
void request(const std::string& url, const std::string& postDatas, MemoryStruct *callbackStruct, const std::string& customRequest, const std::string& fileName);
void requestJson(const std::string& url, const std::string& postDatas, MemoryStruct *callbackStruct, const std::string& customRequest, const std::string& fileName);
void requestFile(const std::string& url, const std::string& fileName);
size_t writeMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp);

} // namespace Api
