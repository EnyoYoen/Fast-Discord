#pragma once

#include "api/snowflake.h"

#include <QString>
#include <QVector>

namespace Api {

//https://luna.gitlab.io/discord-unofficial-docs/user_settings.html#get-users-me-settings

struct CustomStatus
{
    QString   text;
    QString   expiresAt;
    QString   emojiName;
    Snowflake emojiId;
};

struct FriendSourceFlags
{
    bool all;
    bool mutualFriends;
    bool mutualGuilds;
};

struct GuildFolder
{
    QVector<Api::Snowflake> guildIds;
    QString                 name;
    QString                 strId;
    qint64                  intId;
    qint32                  color;
};

struct ClientSettings
{
    ~ClientSettings()
    {
        delete customStatus;
        delete friendSourceFlags;
    }

    CustomStatus           *customStatus;
    FriendSourceFlags      *friendSourceFlags;
    QVector<GuildFolder *>  guildFolders;
    QVector<QString>        guildPositions;
    QVector<QString>        restrictedGuilds;
    QString                 locale;
    QString                 status;
    QString                 theme;
    qint32                  afkTimeout;
    qint32                  animateStickers;
    qint32                  explicitContentFilter;
    qint32                  friendDiscoveryFlags;
    qint32                  timezoneOffset;
    bool                    allowAccessibilityDetection;
    bool                    animateEmoji;
    bool                    contactSyncEnabled;
    bool                    convertEmoticons;
    bool                    defaultGuildsRestricted;
    bool                    detectPlatformAccounts;
    bool                    developerMode;
    bool                    disableGamesTab;
    bool                    enableTtsCommand;
    bool                    gifAutoPlay;
    bool                    inlineAttachmentMedia;
    bool                    inlineEmbedMedia;
    bool                    messageDisplayCompact;
    bool                    nativePhoneIntegrationEnabled;
    bool                    renderEmbeds;
    bool                    renderReactions;
    bool                    showCurrentGame;
    bool                    streamNotificationsEnabled;
    bool                    viewNsfwGuilds;
};

struct Client
{
    QString   username;
    QString   avatar;
    QString   discriminator;
    QString   banner;
    QString   bio;
    QString   locale;
    QString   email;
    QString   phone;
    Snowflake id;
    qint32    publicFlags;
    qint32    flags;
    qint32    purchased_flags;
    qint32    bannerColor;
    qint32    accentColor;
    bool      nfswAllowed;
    bool      mfaEnabled;
    bool      verified;
};

} // namespace Api

