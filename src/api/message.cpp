#include "../../include/api/message.h"

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
    //Putting delete for each field leads to a double free error, I don't know why
    delete application, author, activity, member, referencedMessage, thread, interaction, reactions, embeds, mentions, attachments, mentionChannels, mentionRoles, components, stickerItems, stickers, id, channelId, guildId, content, timestamp, editedTimestamp, webhookId, applicationId, nonceStr;
}

} // namespace Api
