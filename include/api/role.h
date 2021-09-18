#pragma once

#include <string>

namespace Api {

//https://discord.com/developers/docs/topics/permissions#role-object

struct RoleTags
{
    ~RoleTags();

    std::string *botId;
    std::string *integrationId;
};


struct Role
{
    ~Role();

    RoleTags    *tags;
    std::string *id;
    std::string *name;
    std::string *permissions;
    int          color;
    int          position;
    bool         hoist;
    bool         managed;
    bool         mentionable;
};

} // namespace Api
