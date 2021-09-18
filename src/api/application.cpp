#include "../../include/api/application.h"

namespace Api {

Application::~Application()
{
    delete owner, team, rpcOrigins, id, name, icon, description, termsOfServiceUrl, privacyPolicyUrl, summary, verifyKey, guildId, primarySkuId, slug, coverImage;
}

} // namespace Api
