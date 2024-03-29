#pragma once

#include "api/objects/snowflake.h"
#include "api/objects/optional.h"

namespace Api {

//https://discord.com/developers/docs/resources/channel#message-object-message-structure
struct Attachment
{
    QString   filename;
    QString   contentType;
    QString   description;
    QString   url;
    QString   proxyUrl;
    Snowflake id;
    qint32    size;
    qint32    height;
    qint32    width;
    optbool   ephemeral;
};

} // namespace Api

