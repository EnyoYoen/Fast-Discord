#include "../../include/api/channel.h"

namespace Api {

/*Channel::Channel(const Channel &c)
{
    recipients = c.recipients;
    permissionOverwrites = c.permissionOverwrites;
    member = c.member;
    threadMetadata = c.threadMetadata;
    id = c.id;
    name = c.name;
    topic = c.topic;
    icon = c.icon;
    lastPinTimestamp = c.lastPinTimestamp;
    rtcRegion = c.rtcRegion;
    permissions = c.permissions;
    guildId = c.guildId;
    lastMessageId = c.lastMessageId;
    ownerId = c.ownerId;
    applicationId = c.applicationId;
    parentId = c.parentId;
    type = c.type;
    position = c.position;
    bitrate = c.bitrate;
    userLimit = c.userLimit;
    rateLimitPerUser = c.rateLimitPerUser;
    videoQualityMode = c.videoQualityMode;
    messageCount = c.videoQualityMode;
    memberCount = c.videoQualityMode;
    defaultAutoArchiveDuration = c.videoQualityMode;
    nsfw = c.videoQualityMode;
}*/

Channel::~Channel()
{
    delete recipients;
    delete permissionOverwrites;
    delete member;
    delete threadMetadata;
    delete id;
    delete name;
    delete topic;
    delete icon;
    delete lastPinTimestamp;
    delete rtcRegion;
    delete permissions;
    delete guildId;
    delete lastMessageId;
    delete ownerId;
    delete applicationId;
    delete parentId;
}

ChannelMention::~ChannelMention()
{
    delete id;
    delete guilId;
    delete name;
}

} // namespace Api
