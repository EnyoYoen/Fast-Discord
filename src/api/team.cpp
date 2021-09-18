#include "../../include/api/team.h"

namespace Api {

TeamMember::~TeamMember()
{
    delete user, permissions, teamId;
}

Team::~Team()
{
    delete members, icon, id, name, ownerUserId;
}

} // namespace Api
