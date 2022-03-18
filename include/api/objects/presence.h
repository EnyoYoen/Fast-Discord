#pragma once

#include "api/objects/user.h"

#include <QVector>
#include <QString>

namespace Api {

enum class ActivityTypes
{
    Playing,
    Streaming,
    Listening,
    Watching,
    Custom,
    Competing
};

struct ActivityTimestamps
{
    qint64 start;
    qint64 end;
};

struct ActivityAssets
{
    QString largeImage;
    QString largeText;
    QString smallImage;
    QString smallText;
};

struct PartySize
{
    qint32 currentSize;
    qint32 maxSize;
};

struct ActivityParty
{
    PartySize *size;
    QString    id;
};

struct ActivitySecrets
{
    QString match;
    QString join;
    QString spectate;
};

struct Activity
{
    ~Activity()
    {
        delete timestamps;
        delete assets;
        delete party;
        delete secrets;
    }

    ActivityTimestamps *timestamps;
    ActivityAssets     *assets;
    ActivityParty      *party;
    ActivitySecrets    *secrets;
    QString             name;
    QString             state;
    QString             details;
    qint64              applicationId;
    bool                instance;

};

struct ClientStatus
{
    QString desktop;
    QString mobile;
    QString web;
};

struct Presence
{
    ~Presence()
    {
        delete user;
        delete clientStatus;
    }

    User                *user;
    ClientStatus        *clientStatus;
    QVector<Activity *>  activities;
    QString              status;
    Snowflake            userId;
    Snowflake            guildId;
};

} // namespace Api
