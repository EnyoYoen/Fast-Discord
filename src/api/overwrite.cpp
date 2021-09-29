#include "../../include/api/overwrite.h"

namespace Api {

Overwrite::~Overwrite()
{
    delete id;
    delete allow;
    delete deny;
}

} // namespace Api
