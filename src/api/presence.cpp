#include "api/presence.h"

namespace Api {

ActivityAssets::~ActivityAssets()
{
    delete largeImage;
    delete largeText;
    delete smallImage;
    delete smallText;
}

ActivityParty::~ActivityParty()
{
    delete size;
    delete id;
}

ActivitySecrets::~ActivitySecrets()
{
    delete match;
    delete join;
    delete spectate;
}

Activity::~Activity()
{
    delete timestamps;
    delete assets;
    delete party;
    delete secrets;
    delete applicationId;
    delete id;
    delete name;
    delete state;
    delete details;
}

ClientStatus::~ClientStatus()
{
    delete desktop;
    delete mobile;
    delete web;
}

Presence::~Presence()
{
    delete user;
    delete clientStatus;
    delete activities;
    delete userId;
    delete guildId;
    delete status;
}

} // namespace Api
