#pragma once

#include "api/snowflake.h"

#include <QString>

namespace Api {

//https://discord.com/developers/docs/resources/channel#overwrite-object
struct Overwrite
{
    QString   allow;
    QString   deny;
    Snowflake id;
    qint32    type;
};

} // namespace Api
