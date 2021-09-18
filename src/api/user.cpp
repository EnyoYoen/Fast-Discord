#include "../../include/api/user.h"

namespace Api {

User::~User()
{
    delete username, discriminator, avatar, locale, email, id;
}

} // namespace Api
