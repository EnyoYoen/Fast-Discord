#include "api/emoji.h"

namespace Api {

Emoji::~Emoji()
{
    delete user;
    delete roles;
    delete id;
    delete name;
}

Reaction::~Reaction()
{
    delete emoji;
}

} // namespace Api
