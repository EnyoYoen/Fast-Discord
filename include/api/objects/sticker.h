#pragma once

#include "api/objects/user.h"
#include "api/objects/snowflake.h"

#include <QString>

namespace Api {

//https://discord.com/developers/docs/resources/sticker#sticker-object

struct StickerItem
{
    QString   name;
    Snowflake id;
    qint32    formatType;
};

struct Sticker
{
    ~Sticker()
    {
        delete user;
    }

    User      *user;
    QString    name;
    QString    description;
    QString    tags;
    QString    asset;
    Snowflake  id;
    Snowflake  packId;
    Snowflake  guildId;
    qint32     type;
    qint32     formatType;
    qint32     sortValue;
    optbool    available;
};

} // namespace Api
