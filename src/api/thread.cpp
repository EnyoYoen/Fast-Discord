#include "../../include/api/thread.h"

namespace Api {

ThreadMetadata::~ThreadMetadata()
{
    delete archiveTimestamp;
}

ThreadMember::~ThreadMember()
{
    delete joinTimestamp, id, userId;
}

} // namespace Api
