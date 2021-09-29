#include "../../include/api/application.h"

namespace Api {

Application::~Application()
{
    delete owner;
    delete team;
    delete rpcOrigins;
    delete id;
    delete name;
    delete icon;
    delete description;
    delete termsOfServiceUrl;
    delete privacyPolicyUrl;
    delete summary;
    delete verifyKey;
    delete guildId;
    delete primarySkuId;
    delete slug;
    delete coverImage;
}

} // namespace Api
