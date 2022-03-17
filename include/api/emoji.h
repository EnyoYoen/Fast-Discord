#pragma once

#include "api/user.h"
#include "api/snowflake.h"

#include <QString>
#include <QVector>

namespace Api {

//https://discord.com/developers/docs/resources/emoji
struct Emoji
{
    ~Emoji()
    {
        delete user;
    }

    User               *user;
    QVector<Snowflake>  roles;
    QString             name;
    Snowflake           id;
    optbool             requireColons;
    optbool             managed;
    optbool             animated;
    optbool             available;
};

struct Reaction
{
    Emoji  emoji;
    qint32 count;
    bool   me;
};

} // namespace Api
