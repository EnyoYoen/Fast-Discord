#include "api/message.h"

namespace Api {

MessageActivity::~MessageActivity()
{
    delete partyId;
}

MessageReference::~MessageReference()
{
    delete messageId;
    delete channelId;
    delete guildId;
}

MessageInteraction::~MessageInteraction()
{
    delete user;
    delete id;
    delete name;
}

SelectOption::~SelectOption()
{
    delete emoji;
    delete label;
    delete value;
    delete description;
}

MessageComponent::~MessageComponent()
{
    delete emoji;
    delete options;
    delete components;
    delete customId;
    delete label;
    delete url;
    delete placeholder;
}

Message::~Message()
{
    //Putting delete for each field leads to a double free error;delete I don't know why
    delete application;
    delete author;
    delete activity;
    delete member;
    delete referencedMessage;
    delete thread;
    delete interaction;
    delete reactions;
    delete embeds;
    delete mentions;
    delete attachments;
    delete mentionChannels;
    delete mentionRoles;
    delete components;
    delete stickerItems;
    delete stickers;
    delete id;
    delete channelId;
    delete guildId;
    delete content;
    delete timestamp;
    delete editedTimestamp;
    delete webhookId;
    delete applicationId;
    delete nonceStr;
}

} // namespace Api
