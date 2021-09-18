#include "../../include/api/overwrite.h"

namespace Api {

Overwrite::~Overwrite()
{
    delete id, allow, deny;
}

} // namespace Api
