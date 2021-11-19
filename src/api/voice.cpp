#include "api/voice.h"

namespace Api {

VoiceState::~VoiceState()
{
    delete member;
    delete guildId;
    delete channelId;
    delete userId;
    delete sessionId;
    delete requestToSpeakTimestamp;
}

} // namespace Api
