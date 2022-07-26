#pragma once

#include "api/objects/role.h"
#include "api/objects/emoji.h"
#include "api/objects/user.h"
#include "api/objects/channel.h"
#include "api/objects/voice.h"
#include "api/objects/sticker.h"
#include "api/objects/guildmember.h"
#include "api/objects/snowflake.h"

#include <QString>
#include <QVector>

namespace Api {

//https://discord.com/developers/docs/resources/guild

struct WelcomeScreenChannel
{
    QString   description;
    QString   emojiName;
    Snowflake channelId;
    Snowflake emojiId;
};

struct WelcomeScreen
{
    QVector<WelcomeScreenChannel *> welcomeChannels;
    QString                         description;
};

struct StageInstance
{
    QString   topic;
    Snowflake id;
    Snowflake guildId;
    Snowflake channelId;
    Snowflake guildScheduledEventId;
    qint32    privacyLevel;
    bool      discoverableDisabled;
};

struct Guild
{
    ~Guild()
    {
        delete welcomeScreen;
    }

    WelcomeScreen            *welcomeScreen;
    QVector<QString>          guildFeatures;
    QVector<VoiceState *>     voiceStates;
    QVector<GuildMember *>    members;
    QVector<Channel *>        channels;
    QVector<Channel *>        threads;
    void                     *presences; // TODO
    QVector<Role *>           roles;
    QVector<StageInstance *>  stageInstances;
    QVector<Sticker *>        stickers;
    QString                   name;
    QString                   icon;
    QString                   iconHash;
    QString                   splash;
    QString                   discoverySplash;
    QString                   permissions;
    QString                   region;
    QString                   joinedAt;
    QString                   vanityUrlCode;
    QString                   description;
    QString                   banner;
    QString                   preferredLocale;
    Snowflake                 id;
    Snowflake                 ownerId;
    Snowflake                 afkChannelId;
    Snowflake                 widgetChannelId;
    Snowflake                 applicationId;
    Snowflake                 systemChannelId;
    Snowflake                 rulesChannelId;
    Snowflake                 publicUpdatesChannelId;
    qint32                    afkTimeout;
    qint32                    verificationLevel;
    qint32                    defaultMessageNotifications;
    qint32                    explicitContentFilter;
    qint32                    mfaLevel;
    qint32                    systemChannelFlags;
    qint32                    memberCount;
    qint32                    maxPresences;
    qint32                    maxMembers;
    qint32                    premiumTier;
    qint32                    premiumSubscriptionCount;
    qint32                    maxVideoChannelUsers;
    qint32                    approximateMemberCount;
    qint32                    approximatePresenceCount;
    qint32                    nsfw;
    qint32                    nsfwLevel;
    optbool                   owner;
    optbool                   widgetEnabled;
    optbool                   large;
    optbool                   lazy;
    optbool                   unavailable;
    bool                      premiumProgressBarEnabled;
};

} // namespace Api
