#pragma once

#include "user.h"

#include <string>

namespace Api {

//https://discord.com/developers/docs/resources/sticker#sticker-object

struct StickerItem
{
    ~StickerItem();

    std::string *id;
    std::string *name;
    int          formatType;
};

struct Sticker
{
    ~Sticker();

    User        *user;
    std::string *id;
    std::string *packId;
    std::string *name;
    std::string *description;
    std::string *tags;
    std::string *asset;
    std::string *guildId;
    int          type;
    int          formatType;
    int          sortValue;
    bool         available;
};

} // namespace Api
