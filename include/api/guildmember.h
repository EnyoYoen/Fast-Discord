#pragma once

#include "user.h"

#include <string>
#include <vector>

namespace Api {

struct GuildMember
{
    ~GuildMember();

    User                     *user;
    std::vector<std::string> *roles;
    std::string              *nick;
    std::string              *avatar;
    std::string              *joinedAt;
    std::string              *premiumSince;
    std::string              *permissions;
    bool                      deaf;
    bool                      mute;
    bool                      pending;
};

struct GuildMessageMember
{
    ~GuildMessageMember();

    std::vector<std::string> *roles;
    std::string              *nick;
    std::string              *joinedAt;
    std::string              *premiumSince;
    std::string              *permissions;
    bool                      deaf;
    bool                      mute;
    bool                      pending;
};

} // namespace Api
