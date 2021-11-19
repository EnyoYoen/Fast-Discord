#include "api/user.h"

namespace Api {

User::~User()
{
    delete username;
    delete discriminator;
    delete avatar;
    delete locale;
    delete email;
    delete id;
}

} // namespace Api
