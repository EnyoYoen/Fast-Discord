#pragma once

#include "api/objects/user.h"
#include "api/objects/presence.h"

#include <QtGlobal>
#include <QMap>

namespace Api {

//https://discord.com/developers/docs/resources/guild#guild-member-object
struct GuildMember
{
    ~GuildMember()
    {
        delete user;
    }

    User              *user;
    Presence          *presence;
    QVector<Snowflake> roles;
    QString            nick;
    QString            avatar;
    QString            joinedAt;
    QString            premiumSince;
    QString            permissions;
    QString            communicationDisabledUntil;
    bool               deaf;
    bool               mute;
    bool               pending;
};

struct GuildGroup {
    QString id;
    qint32 count;
};

struct MemberOrGroup {
    void *content;
    bool  member;
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

enum class GuildMemberOp : quint8
{
    Sync,
    Update,
    Delete,
    Insert
};

struct GuildMemberGateway
{
    QMap<QString, qint32> groups;
    QVector<void *> structs;
    QVector<GuildMemberOp> ops;
    QString   id;
    Snowflake guildId;
    qint32    onlineCount;
    qint32    memberCount;
};


struct GuildMemberSync
{
    QMap<QString, QVector<GuildMember *>> groupsMembers;
    QMap<QString, qint32>                 groupsCount;
    QVector<qint32>                       membersRange;
};

struct GuildMemberUpdate
{
    MemberOrGroup *memberOrGroup;
    qint32 index;
};

struct GuildMemberDelete
{
    qint32 index;
};

struct GuildMemberInsert
{
    MemberOrGroup *memberOrGroup;
    qint32 index;
};

} // namespace Api
