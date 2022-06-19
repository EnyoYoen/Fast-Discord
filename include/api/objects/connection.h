#pragma once

namespace Api {

struct Connection
{
    QString   type;
    QString   id;
    QString   name;
    QString   accessToken;
    qint32    visibility;
    bool      revoked;
    bool      friendSync;
    bool      showActivity;
    bool      verified;
};

} // namespace Api