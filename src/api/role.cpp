#include "../../include/api/role.h"

namespace Api {

RoleTags::~RoleTags()
{
    delete botId;
    delete integrationId;
}

Role::~Role()
{
    delete tags;
    delete id;
    delete name;
    delete permissions;
}

} // namespace Api
