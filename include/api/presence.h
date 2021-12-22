#pragma once

#include "user.h"

#include <vector>
#include <string>

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
    int start;
    int end;
};

struct ActivityAssets
{
    ~ActivityAssets();

    std::string *largeImage;
    std::string *largeText;
    std::string *smallImage;
    std::string *smallText;
};

struct PartySize
{
    int currentSize;
    int maxSize;
};

struct ActivityParty
{
    ~ActivityParty();

    PartySize   *size;
    std::string *id;
};

struct ActivitySecrets
{
    ~ActivitySecrets();

    std::string *match;
    std::string *join;
    std::string *spectate;
};

struct Activity
{
    ~Activity();

    ActivityTimestamps *timestamps;
    ActivityAssets     *assets;
    ActivityParty      *party;
    ActivitySecrets    *secrets;

    std::string *applicationId;
    std::string *id;
    std::string *name;
    std::string *state;
    std::string *details;

    int createdAt;
    int type;

    bool instance;

};

struct ClientStatus
{
    ~ClientStatus();

    std::string *desktop;
    std::string *mobile;
    std::string *web;
};

struct Presence
{
    ~Presence();

    User         *user;
    ClientStatus *clientStatus;

    std::vector<Activity *> *activities;

    std::string *userId;
    std::string *guildId;
    std::string *status;
};

} // namespace Api
