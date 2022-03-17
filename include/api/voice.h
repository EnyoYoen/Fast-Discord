#pragma once

#include "api/guildmember.h"
#include "api/snowflake.h"

namespace Api {

//https://discord.com/developers/docs/resources/voice#voice-state-object
struct VoiceState
{
    ~VoiceState()
    {
        delete member;
    }

    GuildMember *member;
    QString      sessionId;
    QString      requestToSpeakTimestamp;
    Snowflake    guildId;
    Snowflake    channelId;
    Snowflake    userId;
    bool         deaf;
    bool         mute;
    bool         selfDeaf;
    bool         selfMute;
    bool         selfStream;
    bool         selfVideo;
    bool         suppress;
};

} // namespace Api
