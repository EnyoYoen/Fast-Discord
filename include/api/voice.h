#pragma once

#include "guildmember.h"

#include <string>

namespace Api {

//https://discord.com/developers/docs/resources/voice#voice-state-object
struct VoiceState
{
    ~VoiceState();

    GuildMember *member;
    std::string *guildId;
    std::string *channelId;
    std::string *userId;
    std::string *sessionId;
    std::string *requestToSpeakTimestamp;
    bool         deaf;
    bool         mute;
    bool         selfDeaf;
    bool         selfMute;
    bool         selfStream;
    bool         selfVideo;
    bool         suppress;
};

} // namespace Api
