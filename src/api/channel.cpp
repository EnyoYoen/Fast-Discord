#include "api/channel.h"

namespace Api {

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

PrivateChannel::~PrivateChannel()
{
    delete recipientIds;
    delete icon;
    delete id;
    delete lastMessageId;
    delete name;
    delete ownerId;
}

} // namespace Api
