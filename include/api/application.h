#pragma once

#include "api/team.h"
#include "api/user.h"
#include "api/snowflake.h"

#include <QString>
#include <QVector>

namespace Api {

//https://discord.com/developers/docs/resources/application
struct Application
{
    ~Application()
    {
        delete owner;
        delete team;
    }

    User             *owner;
    Team             *team;
    QVector<QString>  rpcOrigins;
    QString           name;
    QString           icon;
    QString           description;
    QString           termsOfServiceUrl;
    QString           privacyPolicyUrl;
    QString           summary;
    QString           verifyKey;
    QString           slug;
    QString           coverImage;
    Snowflake         id;
    Snowflake         guildId;
    Snowflake         primarySkuId;
    qint32            flags;
    bool              botPublic;
    bool              botRequireCodeGrant;
};

} // namespace Api
