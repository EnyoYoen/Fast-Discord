#pragma once

#include "api/user.h"
#include "api/snowflake.h"

#include <QString>
#include <QVector>

namespace Api {

//https://discord.com/developers/docs/topics/teams#data-models-team-object

struct TeamMember
{
    User             user;
    QVector<QString> permissions;
    Snowflake        teamId;
    qint32           memberShipState;
};

struct Team
{
    QVector<TeamMember *> members;
    QString               icon;
    QString               name;
    Snowflake             id;
    Snowflake             ownerUserId;
};

} // namespace Api
