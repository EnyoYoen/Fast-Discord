#include "../../include/api/channel.h"

#include "../../include/api/jsonutils.h"
#include "../../include/api/request.h"

#include "../../lib/nlohmann/json.hpp"

using json = nlohmann::json;

namespace Api {

Channel::~Channel()
{
    delete recipients, permissionOverwrites, member, threadMetadata, id, name, topic, icon, lastPinTimestamp, rtcRegion, permissions, guildId, lastMessageId, ownerId, applicationId, parentId;
}

ChannelMention::~ChannelMention()
{
    delete id, guilId, name;
}


std::vector<Channel *> getPrivateChannels()
{
    MemoryStruct response;

    request(
        "https://discord.com/api/v9/users/@me/channels",
        "",
        &response,
        "",
        "");

    json jsonChannels = json::parse(response.memory);

    std::vector<Channel *> channels;
    for (unsigned int i = 0 ; i < jsonChannels.size() ; i++) {
        channels.push_back(getChannelFromJson(jsonChannels[i], std::string("")));
    }

    return channels;
}

} // namespace Api
