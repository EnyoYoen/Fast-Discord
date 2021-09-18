#pragma once

#include <string>

namespace Api {

struct Guild // TODO finish implementing Guild struct
{
    ~Guild();

    std::string *id;
    std::string *name;
    std::string *icon;
    std::string *iconHash;
    std::string *splash;
    std::string *discoverySplash;
    std::string *ownerId;
    std::string *permissions;
    std::string *region;
    std::string *afkChannelId;
    std::string *widgetChannelId;
    std::string *applicationId;
    std::string *systemChannelId;
    std::string *rulesChannelId;
    std::string *joinedAt;
    std::string *vanityUrlCode;
    std::string *description;
    std::string *banner;
    std::string *preferredLocale;
    std::string *publicUpdatesChannelId;
};

} // namespace Api
