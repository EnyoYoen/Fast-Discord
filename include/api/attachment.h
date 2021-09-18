#pragma once

#include <string>

namespace Api {

//https://discord.com/developers/docs/resources/channel#message-object-message-structure
struct Attachment
{
    ~Attachment();

    std::string *id;
    std::string *filename;
    std::string *contentType;
    std::string *url;
    std::string *proxyUrl;
    int          size;
    int          height;
    int          width;
};

} // namespace Api

