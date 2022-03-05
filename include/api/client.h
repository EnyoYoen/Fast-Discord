#pragma once

#include <string>
#include <vector>

namespace Api {

//https://luna.gitlab.io/discord-unofficial-docs/user_settings.html#get-users-me-settings

struct CustomStatus
{
    ~CustomStatus();

    std::string *text;
    std::string *expiresAt;
    std::string *emojiName;
    std::string *emojiId;
};

struct FriendSourceFlags
{
    ~FriendSourceFlags();

    bool all;
    bool mutualFriends;
    bool mutualGuilds;
};

struct GuildFolder
{
    ~GuildFolder();

    std::vector<std::string> *guildIds;
    std::string              *name;
    std::string              *strId;
    long long                 intId;
    int                       color;
};

struct ClientSettings
{
    ~ClientSettings();

    CustomStatus               *customStatus;
    FriendSourceFlags          *friendSourceFlags;
    std::vector<GuildFolder *> *guildFolders;
    std::vector<std::string>   *guildPositions;
    std::vector<std::string>   *restrictedGuilds;
    std::string                *locale;
    std::string                *status;
    std::string                *theme;
    int                         afkTimeout;
    int                         animateStickers;
    int                         explicitContentFilter;
    int                         friendDiscoveryFlags;
    int                         timezoneOffset;
    bool                        allowAccessibilityDetection;
    bool                        animateEmoji;
    bool                        contactSyncEnabled;
    bool                        convertEmoticons;
    bool                        defaultGuildsRestricted;
    bool                        detectPlatformAccounts;
    bool                        developerMode;
    bool                        disableGamesTab;
    bool                        enableTtsCommand;
    bool                        gifAutoPlay;
    bool                        inlineAttachmentMedia;
    bool                        inlineEmbedMedia;
    bool                        messageDisplayCompact;
    bool                        nativePhoneIntegrationEnabled;
    bool                        renderEmbeds;
    bool                        renderReactions;
    bool                        showCurrentGame;
    bool                        streamNotificationsEnabled;
    bool                        viewNsfwGuilds;
};

struct Client
{
    ~Client();

    std::string *id;
    std::string *username;
    std::string *avatar;
    std::string *discriminator;
    std::string *banner;
    std::string *bio;
    std::string *locale;
    std::string *email;
    std::string *phone;
    int          publicFlags;
    int          flags;
    int          purchased_flags;
    int          bannerColor;
    int          accentColor;
    bool         nfswAllowed;
    bool         mfaEnabled;
    bool         verified;
};

} // namespace Api

