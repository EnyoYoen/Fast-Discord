#pragma once

#include "api/objects/thread.h"
#include "api/objects/user.h"
#include "api/objects/overwrite.h"
#include "api/objects/snowflake.h"

#include <QString>
#include <QVector>

namespace Api {

enum ChannelTypes {
    GuildText,
    DM,
    GuildVoice,
    GroupDM,
    GuildCategory,
    GuildNews,
    GuildStore,
    GuildNewsThread,
    GuildPublicThread,
    GuildPrivateThread,
    GuildStageVoice
};

//https://discord.com/developers/docs/resources/channel
struct Channel
{
    ~Channel()
    {
        delete member;
        delete threadMetadata;
    }

    QVector<User *>       recipients;
    QVector<Overwrite *>  permissionOverwrites;
    ThreadMember         *member;
    ThreadMetadata       *threadMetadata;
    QString               name;
    QString               topic;
    QString               icon;
    QString               lastPinTimestamp;
    QString               rtcRegion;
    QString               permissions;
    Snowflake             id;
    Snowflake             guildId;
    Snowflake             lastMessageId;
    Snowflake             ownerId;
    Snowflake             applicationId;
    Snowflake             parentId;
    qint32                type;
    qint32                position;
    qint32                bitrate;
    qint32                userLimit;
    qint32                rateLimitPerUser;
    qint32                videoQualityMode;
    qint32                messageCount;
    qint32                memberCount;
    qint32                defaultAutoArchiveDuration;
    bool                  nsfw;
};

//https://discord.com/developers/docs/resources/channel#channel-mention-object
struct ChannelMention
{
    QString   name;
    Snowflake id;
    Snowflake guilId;
    qint32    type;
};

struct PrivateChannel
{
    QVector<Snowflake> recipientIds;
    QString            icon;
    QString            name;
    Snowflake          id;
    Snowflake          lastMessageId;
    Snowflake          ownerId;
    qint32             type;
};

} // namespace Api
