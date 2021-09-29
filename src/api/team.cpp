#include "../../include/api/team.h"

namespace Api {

TeamMember::~TeamMember()
{
    delete user;
    delete permissions;
    delete teamId;
}

Team::~Team()
{
    delete members;
    delete icon;
    delete id;
    delete name;
    delete ownerUserId;
}

} // namespace Api
