#pragma once

#include "api/objects/user.h"

namespace Api {

//https://discord.com/developers/docs/resources/guild#guild-member-object
struct GuildMember
{
    ~GuildMember()
    {
        delete user;
    }

    User             *user;
    QVector<QString>  roles;
    QString           nick;
    QString           avatar;
    QString           joinedAt;
    QString           premiumSince;
    QString           permissions;
    QString           communicationDisabledUntil;
    bool              deaf;
    bool              mute;
    bool              pending;
};

struct GuildMessageMember
{
    QVector<QString> roles;
    QString          nick;
    QString          joinedAt;
    QString          premiumSince;
    QString          permissions;
    bool             deaf;
    bool             mute;
    bool             pending;
};

} // namespace Api
