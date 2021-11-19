#include "api/thread.h"

namespace Api {

ThreadMetadata::~ThreadMetadata()
{
    delete archiveTimestamp;
}

ThreadMember::~ThreadMember()
{
    delete joinTimestamp;
    delete id;
    delete userId;
}

} // namespace Api
