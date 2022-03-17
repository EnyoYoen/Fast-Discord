#pragma once

#include "api/snowflake.h"
#include "api/optional.h"

#include <QString>

namespace Api {

//https://discord.com/developers/docs/resources/channel#thread-metadata-object
struct ThreadMetadata
{
    QString archiveTimestamp;
    QString createTimestamp;
    qint32  autoArchiveDuration;
    optbool invitable;
    bool    archived;
    bool    locked;
};

//https://discord.com/developers/docs/resources/channel#thread-member-object
struct ThreadMember
{
    QString   joinTimestamp;
    Snowflake id;
    Snowflake userId;
    qint32    flags;
};

} // namespace Api
