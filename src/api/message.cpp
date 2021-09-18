#include "../../include/api/message.h"

#include "../../include/api/request.h"
#include "../../include/api/jsonutils.h"

#include "../../lib/nlohmann/json.hpp"

namespace Api {

MessageActivity::~MessageActivity()
{
    delete partyId;
}

MessageReference::~MessageReference()
{
    delete messageId, channelId, guildId;
}

MessageInteraction::~MessageInteraction()
{
    delete user, id, name;
}

SelectOption::~SelectOption()
{
    delete emoji, label, value, description;
}

MessageComponent::~MessageComponent()
{
    delete emoji, options, components, customId, label, url, placeholder;
}

Message::~Message()
{
    delete application, author, activity, member, referencedMessage, thread, interaction, reactions, embeds, mentions, attachments, mentionChannels, mentionRoles, components, stickerItems, stickers, id, channelId, guildId, content, timestamp, editedTimestamp, webhookId, applicationId, nonceStr;
}

std::string getFileNameFromPath(const std::string& filepath)
{
#ifdef _WIN32
    #define SEP "\\"
#else
    #define SEP "/"
#endif
    return filepath.substr(filepath.find_last_of(SEP) + 1);
}

void typing(const std::string& channelId)
{
    request("https://discord.com/api/v9/channels/" + channelId + "/typing",
            " ",
            nullptr,
            "POST",
            "");
}

void sendMessage(const std::string& content, const std::string& channelId)
{
    requestJson("https://discord.com/api/v9/channels/" + channelId + "/messages",
            "{\"content\":\"" + content + "\"}",
            nullptr,
            "",
            "");
}

void sendMessageWithFile(const std::string& content, const std::string& channelId, const std::string& filePath)
{
    request("https://discord.com/api/v9/channels/" + channelId + "/messages",
            "{\"content\":\"" + content + "\"}",
            nullptr,
            "",
            filePath);
}

std::vector<Message> getMessages(const std::string& channelId, unsigned int limit)
{
    MemoryStruct response;

    limit = limit >= 100 ? 100 : limit;

    request(
        "https://discord.com/api/v9/channels/" + channelId + "/messages?limit=" + std::to_string(limit),
        "",
        &response,
        "",
        "");

    json jsonMessages = json::parse(response.memory);

    std::vector<Message> messages;
    for (unsigned int i = 0 ; i < jsonMessages.size() ; i++) {
        messages.push_back(*getMessageFromJson(jsonMessages[i], ""));
    }

    return messages;
}

void deleteMessage(const std::string& channelId, const std::string& messageId)
{
    request(
        "https://discord.com/api/v9/channels/" + channelId + "/messages/" + messageId,
        "",
        nullptr,
        "DELETE",
        "");
}

void pinMessage(const std::string& channelId, const std::string& messageId)
{
    request(
        "https://discord.com/api/v9/channels/" + channelId + "/pins/" + messageId,
        "",
        nullptr,
        "PUT",
        "");
}

void unpinMessage(const std::string& channelId, const std::string& messageId)
{
    request(
        "https://discord.com/api/v9/channels/" + channelId + "/pins/" + messageId,
        "",
        nullptr,
        "PUT",
        "");
}

} // namespace Api
