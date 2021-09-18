#include "../../include/api/role.h"

namespace Api {

RoleTags::~RoleTags()
{
    delete botId, integrationId;
}

Role::~Role()
{
    delete tags, id, name, permissions;
}

} // namespace Api
