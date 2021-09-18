#include "../../include/api/guildmember.h"

namespace Api {

GuildMember::~GuildMember()
{
    delete user, roles, nick, joinedAt, premiumSince, permissions;
}

GuildMessageMember::~GuildMessageMember()
{
    delete roles, nick, joinedAt, premiumSince, permissions;
}

} // namespace Api
