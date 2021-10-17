#pragma once

#include "role.h"
#include "emoji.h"
#include "user.h"
#include "channel.h"
#include "voice.h"
#include "sticker.h"
#include "guildmember.h"

#include <string>

namespace Api {

struct WelcomeScreenChannel
{
    ~WelcomeScreenChannel();

    std::string *channelId;
    std::string *description;
    std::string *emojiId;
    std::string *emojiName;
};

struct WelcomeScreen
{
    ~WelcomeScreen();

    std::vector<WelcomeScreenChannel> *welcomeChannels;
    std::string                       *description;
};

struct StageInstance
{
    ~StageInstance();

    std::string *id;
    std::string *guildId;
    std::string *channelId;
    std::string *topic;
    int          privacyLevel;
    bool         discoverableDisabled;
};

struct Guild
{
    ~Guild();

    WelcomeScreen              *welcomeScreen;
    std::vector<std::string>   *guildFeatures;
    std::vector<VoiceState>    *voiceStates;
    std::vector<GuildMember>   *members;
    std::vector<Channel *>     *channels;
    std::vector<Channel *>     *threads;
    void                       *presences; // TODO
    std::vector<StageInstance> *stageInstances;
    std::vector<Sticker>       *stickers;
    std::string                *id;
    std::string                *name;
    std::string                *icon;
    std::string                *iconHash;
    std::string                *splash;
    std::string                *discoverySplash;
    std::string                *ownerId;
    std::string                *permissions;
    std::string                *region;
    std::string                *afkChannelId;
    std::string                *widgetChannelId;
    std::string                *applicationId;
    std::string                *systemChannelId;
    std::string                *rulesChannelId;
    std::string                *joinedAt;
    std::string                *vanityUrlCode;
    std::string                *description;
    std::string                *banner;
    std::string                *preferredLocale;
    std::string                *publicUpdatesChannelId;
    int                         afkTimeout;
    int                         verificationLevel;
    int                         defaultMessageNotifications;
    int                         explicitContentFilter;
    int                         mfaLevel;
    int                         systemChannelFlags;
    int                         memberCount;
    int                         maxPresences;
    int                         maxMembers;
    int                         premiumTier;
    int                         premiumSubscriptionCount;
    int                         maxVideoChannelUsers;
    int                         approximateMemberCount;
    int                         approximatePresenceCount;
    int                         nsfwLevel;
    bool                        owner;
    bool                        widgetEnabled;
    bool                        large;
    bool                        unavailable;
};

} // namespace Api
