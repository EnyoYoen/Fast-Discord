#include "../../include/api/attachment.h"

namespace Api {

Attachment::~Attachment()
{
    delete id, filename, contentType, url, proxyUrl;
}

} // namespace Api
