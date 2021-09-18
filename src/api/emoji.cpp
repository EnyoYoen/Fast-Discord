#include "../../include/api/emoji.h"

namespace Api {

Emoji::~Emoji()
{
    delete user, roles, id, name;
}

Reaction::~Reaction()
{
    delete emoji;
}

} // namespace Api
