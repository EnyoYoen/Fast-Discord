#pragma once

#include "api/objects/team.h"
#include "api/objects/user.h"
#include "api/objects/snowflake.h"

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
