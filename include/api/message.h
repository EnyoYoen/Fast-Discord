#pragma once

#include "user.h"
#include "guildmember.h"
#include "role.h"
#include "channel.h"
#include "emoji.h"
#include "application.h"
#include "attachment.h"
#include "embed.h"
#include "sticker.h"

#include <string>

namespace Api {

//https://discord.com/developers/docs/resources/channel#message-object

struct MessageActivity
{
    ~MessageActivity();

    std::string *partyId;
    int          type;
};

struct MessageReference
{
    ~MessageReference();

    std::string *messageId;
    std::string *channelId;
    std::string *guildId;
    bool         failIfNotExists;
};

struct MessageInteraction
{
    ~MessageInteraction();

    User        *user;
    std::string *id;
    std::string *name;
    int          type;
};

struct SelectOption
{
    ~SelectOption();

    Emoji       *emoji;
    std::string *label;
    std::string *value;
    std::string *description;
    bool         sodefault;
};

struct MessageComponent
{
    ~MessageComponent();

    Emoji                         *emoji;
    std::vector<SelectOption>     *options;
    std::vector<MessageComponent> *components;
    std::string                   *customId;
    std::string                   *label;
    std::string                   *url;
    std::string                   *placeholder;
    int                            type;
    int                            style;
    int                            minValues;
    int                            maxValues;
    bool                           disabled;
};

struct Message
{
    ~Message();

    Application                   *application;
    User                          *author;
    MessageActivity               *activity;
    GuildMessageMember            *member;
    Message                       *referencedMessage;
    Channel                       *thread;
    MessageInteraction            *interaction;
    std::vector<Reaction>         *reactions;
    std::vector<Embed>            *embeds;
    std::vector<User>             *mentions;
    std::vector<Attachment>       *attachments;
    std::vector<ChannelMention>   *mentionChannels;
    std::vector<std::string>      *mentionRoles;
    std::vector<MessageComponent> *components;
    std::vector<StickerItem>      *stickerItems;
    std::vector<Sticker>          *stickers;
    std::string                   *id;
    std::string                   *channelId;
    std::string                   *guildId;
    std::string                   *content;
    std::string                   *timestamp;
    std::string                   *editedTimestamp;
    std::string                   *webhookId;
    std::string                   *applicationId;
    std::string                   *nonceStr;
    int                            nonceInt;
    int                            authorPublicFlags;
    int                            type;
    int                            flags;
    bool                           tts;
    bool                           pinned;
    bool                           mentionEveryone;
};

void typing(const std::string& channelId);
void sendMessage(const std::string& content, const std::string& channelId);
void sendMessageWithFile(const std::string& content, const std::string& channelId, const std::string& filePath);
std::vector<Message> getMessages(const std::string& channelId, unsigned int limit);
void deleteMessage(const std::string& channelId, const std::string& messageId);

void pinMessage(const std::string& channelId, const std::string& messageId);
void unpinMessage(const std::string& channelId, const std::string& messageId);

} // namespace Api
