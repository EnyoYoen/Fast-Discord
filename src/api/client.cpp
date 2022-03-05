#include "api/client.h"

namespace Api {

CustomStatus::~CustomStatus()
{
    delete text;
    delete expiresAt;
    delete emojiName;
    delete emojiId;
}

GuildFolder::~GuildFolder()
{
    delete guildIds;
    delete name;
    delete strId;
}

ClientSettings::~ClientSettings()
{
    delete guildFolders;
    delete guildPositions;
    delete restrictedGuilds;
    delete locale;
    delete status;
    delete theme;
}

Client::~Client()
{
    delete id;
    delete username;
    delete avatar;
    delete discriminator;
    delete banner;
    delete bio;
    delete locale;
    delete email;
    delete phone;
}

} // namespace Api
