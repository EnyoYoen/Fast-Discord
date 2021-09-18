#include "../../include/api/sticker.h"

namespace Api {

StickerItem::~StickerItem()
{
    delete id, name;
}

Sticker::~Sticker()
{
    delete user, id, packId, name, description, tags, asset, guildId;
}

} // namespace Api
