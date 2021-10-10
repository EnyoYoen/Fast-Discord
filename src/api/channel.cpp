#include "../../include/api/channel.h"

#include "../../include/api/jsonutils.h"
#include "../../include/api/request.h"

#include "../../lib/nlohmann/json.hpp"

using json = nlohmann::json;

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
    if (!jsonChannels.empty()) {
        for (unsigned int i = 0 ; i < jsonChannels.size() ; i++) {
            channels.push_back(getChannelFromJson(jsonChannels[i], std::string("")));
        }
    }

    return channels;
}

} // namespace Api
