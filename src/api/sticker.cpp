#include "api/sticker.h"

namespace Api {

StickerItem::~StickerItem()
{
    delete id;
    delete name;
}

Sticker::~Sticker()
{
    delete user;
    delete id;
    delete packId;
    delete name;
    delete description;
    delete tags;
    delete asset;
    delete guildId;
}

} // namespace Api
