#pragma once

#include <string>

namespace Api {

//https://discord.com/developers/docs/topics/permissions#role-object

struct RoleTags
{
    QString botId;
    QString integrationId;
};


struct Role
{
    ~Role()
    {
        delete tags;
    }

    RoleTags *tags;
    QString   id;
    QString   name;
    QString   permissions;
    qint32    color;
    qint32    position;
    bool      hoist;
    bool      managed;
    bool      mentionable;
};

} // namespace Api
