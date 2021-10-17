#pragma once

#include "thread.h"
#include "user.h"
#include "overwrite.h"

#include <string>
#include <vector>

namespace Api {

//https://discord.com/developers/docs/resources/channel
struct Channel
{
        ~Channel();
        //Channel(const Channel &c);
        std::vector<User>      *recipients;
        std::vector<Overwrite> *permissionOverwrites;
        ThreadMember           *member;
        ThreadMetadata         *threadMetadata;
        std::string            *id;
        std::string            *name;
        std::string            *topic;
        std::string            *icon;
        std::string            *lastPinTimestamp;
        std::string            *rtcRegion;
        std::string            *permissions;
        std::string            *guildId;
        std::string            *lastMessageId;
        std::string            *ownerId;
        std::string            *applicationId;
        std::string            *parentId;
        int                     type;
        int                     position;
        int                     bitrate;
        int                     userLimit;
        int                     rateLimitPerUser;
        int                     videoQualityMode;
        int                     messageCount;
        int                     memberCount;
        int                     defaultAutoArchiveDuration;
        bool                    nsfw;
};

struct ChannelMention
{
    ~ChannelMention();

    std::string *id;
    std::string *guilId;
    std::string *name;
    int          type;
};

} // namespace Api
