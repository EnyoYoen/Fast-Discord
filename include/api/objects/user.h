#pragma once

#include "api/objects/snowflake.h"
#include "api/objects/optional.h"

#include <QString>

namespace Api {

//https://discord.com/developers/docs/resources/user#user-object
struct User
{
    ~User() {
        int a = 0;
        a++;
        int b = a +1;
    }

    QString   username;
    QString   discriminator;
    QString   avatar;
    QString   banner;
    QString   locale;
    QString   email;
    Snowflake id;
    qint32    accentColor;
    qint32    flags;
    qint32    premiumType;
    qint32    publicFlags;
    optbool   bot;
    optbool   system;
    optbool   mfaEnabled;
    optbool   verified;
};

} // namespace Api
