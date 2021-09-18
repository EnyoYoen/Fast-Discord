#include "../../include/api/status.h"

namespace Api {

void setStatus(const std::string& status)
{
    request(
        "https://discord.com/api/v9/users/@me/settings",
        "{\"status\":\"" + status + "\"}",
        nullptr,
        "PATCH",
        ""
    );
}

} // namespace Api
