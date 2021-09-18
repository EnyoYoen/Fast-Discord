#pragma once

#include "request.h"

#include <string>

namespace Api {

//changing the status by requesting to the api
void setStatus(const std::string& status);

} // namespace Api
