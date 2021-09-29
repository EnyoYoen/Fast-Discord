#include "../../include/api/guildmember.h"

namespace Api {

GuildMember::~GuildMember()
{
    delete user;
    delete roles;
    delete nick;
    delete joinedAt;
    delete premiumSince;
    delete permissions;
}

GuildMessageMember::~GuildMessageMember()
{
    delete roles;
    delete nick;
    delete joinedAt;
    delete premiumSince;
    delete permissions;
}

} // namespace Api
