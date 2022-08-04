#pragma once

#include "api/objects/user.h"
#include "api/objects/guildmember.h"
#include "api/objects/role.h"
#include "api/objects/channel.h"
#include "api/objects/emoji.h"
#include "api/objects/application.h"
#include "api/objects/attachment.h"
#include "api/objects/embed.h"
#include "api/objects/sticker.h"
#include "api/objects/snowflake.h"

#include <QObject>
#include <QString>
#include <QVector>

namespace Api {

//https://discord.com/developers/docs/resources/channel#message-object

enum MessageType {
    Default,
    RecipientAdd,
    RecipientRemove,
    CallT,
    ChannelNameChange,
    ChannelIconChange,
    ChannelPinnedMessage,
    GuildMemberJoin,
    UserPremiumGuildSubscription,
    UserPremiumGuildSubscriptionTier1,
    UserPremiumGuildSubscriptionTier2,
    UserPremiumGuildSubscriptionTier3,
    ChannelFollowAdd,
    GuildDiscoveryDisqualified,
    GuildDiscoveryRequalified,
    GuildDiscoveryGracePeriodInitialWarning,
    GuildDiscoveryGracePeriodFinalWarning,
    ThreadCreated,
    Reply,
    ChatInputCommand,
    ThreadStarterMessage,
    GuildInviteReminder,
    ContextMenuCommand
};

struct MessageActivity
{
    QString partyId;
    qint32  type;
};

struct MessageReference
{
    Snowflake messageId;
    Snowflake channelId;
    Snowflake guildId;
    bool      failIfNotExists;
};

struct MessageInteraction
{
    ~MessageInteraction()
    {
        delete member;
    }

    User         user;
    GuildMember *member;
    QString      name;
    Snowflake    id;
    qint32       type;
};

struct SelectOption
{
    ~SelectOption()
    {
        delete emoji;
    }

    Emoji   *emoji;
    QString  label;
    QString  value;
    QString  description;
    optbool  sodefault;
};

struct MessageComponent
{
    ~MessageComponent()
    {
        delete emoji;
    }

    Emoji                       *emoji;
    QVector<SelectOption *>      options;
    QVector<MessageComponent *>  components;
    QString                      customId;
    QString                      label;
    QString                      url;
    QString                      placeholder;
    qint32                       type;
    qint32                       style;
    qint32                       minValues;
    qint32                       maxValues;
    optbool                      disabled;
};

struct Call
{
    QVector<Snowflake> participants;
    QString          endedTimestamp;
};

struct Message
{
    ~Message()
    {
        delete application;
        delete activity;
        delete referencedMessage;
        delete thread;
        delete interaction;
    }

    User                         author;
    Application                 *application;
    MessageActivity             *activity;
    Message                     *referencedMessage;
    Channel                     *thread;
    MessageInteraction          *interaction;
    Call                        *call;
    QVector<Reaction *>          reactions;
    QVector<Embed *>             embeds;
    QVector<User *>              mentions;
    QVector<Attachment *>        attachments;
    QVector<ChannelMention *>    mentionChannels;
    QVector<QString>             mentionRoles;
    QVector<MessageComponent *>  components;
    QVector<StickerItem *>       stickerItems;
    QVector<Sticker *>           stickers;
    QString                      content;
    QString                      timestamp;
    QString                      editedTimestamp;
    QString                      nonceStr;
    Snowflake                    id;
    Snowflake                    channelId;
    Snowflake                    guildId;
    Snowflake                    webhookId;
    Snowflake                    applicationId;
    qint32                       nonceint;
    qint32                       authorPublicFlags;
    qint32                       type;
    qint32                       flags;
    bool                         tts;
    bool                         pinned;
    bool                         mentionEveryone;
};

} // namespace Api

Q_DECLARE_METATYPE(Api::Message);
