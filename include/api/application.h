#pragma once

#include "team.h"
#include "user.h"

#include <string>
#include <vector>

namespace Api {

//https://discord.com/developers/docs/resources/application
struct Application
{
    ~Application();

    User                     *owner;
    Team                     *team;
    std::vector<std::string> *rpcOrigins;
    std::string              *id;
    std::string              *name;
    std::string              *icon;
    std::string              *description;
    std::string              *termsOfServiceUrl;
    std::string              *privacyPolicyUrl;
    std::string              *summary;
    std::string              *verifyKey;
    std::string              *guildId;
    std::string              *primarySkuId;
    std::string              *slug;
    std::string              *coverImage;
    int                       flags;
    bool                      botPublic;
    bool                      botRequireCodeGrant;
};

} // namespace Api
