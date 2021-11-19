#include "api/attachment.h"

namespace Api {

Attachment::~Attachment()
{
    delete id;
    delete filename;
    delete contentType;
    delete url;
    delete proxyUrl;
}

} // namespace Api
