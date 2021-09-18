#pragma once

#include <string>

namespace Api {

//https://discord.com/developers/docs/resources/channel#thread-metadata-object
struct ThreadMetadata
{
    ~ThreadMetadata();

    std::string *archiveTimestamp;
    int          autoArchiveDuration;
    bool         archived;
    bool         locked;
};

//https://discord.com/developers/docs/resources/channel#thread-member-object
struct ThreadMember
{
    ~ThreadMember();

    std::string *joinTimestamp;
    std::string *id;
    std::string *userId;
    int          flags;
};

} // namespace Api
