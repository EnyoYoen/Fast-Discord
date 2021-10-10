#include "../../include/api/guild.h"

#include "../../include/api/jsonutils.h"
#include "../../include/api/request.h"

#include "../../lib/nlohmann/json.hpp"

namespace Api {

WelcomeScreenChannel::~WelcomeScreenChannel()
{
    delete channelId;
    delete description;
    delete emojiId;
    delete emojiName;
}

WelcomeScreen::~WelcomeScreen()
{
    delete welcomeChannels;
    delete description;
}

StageInstance::~StageInstance()
{
    delete id;
    delete guildId;
    delete channelId;
    delete topic;
}

Guild::~Guild()
{
    delete welcomeScreen;
    delete guildFeatures;
    delete voiceStates;
    delete members;
    delete channels;
    delete threads;
    free(presences);
    delete stageInstances;
    delete stickers;
    delete id;
    delete name;
    delete icon;
    delete iconHash;
    delete splash;
    delete discoverySplash;
    delete ownerId;
    delete permissions;
    delete region;
    delete afkChannelId;
    delete widgetChannelId;
    delete applicationId;
    delete systemChannelId;
    delete rulesChannelId;
    delete joinedAt;
    delete vanityUrlCode;
    delete description;
    delete banner;
    delete preferredLocale;
    delete publicUpdatesChannelId;
}

std::vector<Guild *> getGuilds()
{
    MemoryStruct response;

    request(
        "https://discord.com/api/v9/users/@me/guilds",
        "",
        &response,
        "",
        "");

    json jsonGuilds = json::parse(response.memory);

    std::vector<Guild *> guilds;
    if (!jsonGuilds.empty()) {
        for (unsigned int i = 0 ; i < jsonGuilds.size() ; i++) {
            guilds.push_back(getGuildFromJson(jsonGuilds[i], std::string("")));
        }
    }

    return guilds;
}

} // namespace Api
