#pragma once

#include "user.h"

#include <string>
#include <vector>

namespace Api {

//https://discord.com/developers/docs/topics/teams#data-models-team-object

struct TeamMember
{
    ~TeamMember();

    User                     *user;
    std::vector<std::string> *permissions;
    std::string              *teamId;
    int                       memberShipState;
};

struct Team
{
    ~Team();

    std::vector<TeamMember> *members;
    std::string             *icon;
    std::string             *id;
    std::string             *name;
    std::string             *ownerUserId;
};

} // namespace Api
