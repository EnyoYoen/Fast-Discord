#include "../../include/api/jsonutils.h"

namespace Api {

std::string *valueNoNull(const json& jsonObj, const std::string& key)
{
    try {
        json attribute = key == "" ? jsonObj : jsonObj.at(key);
        if (attribute.is_null())
            return nullptr;
        return new std::string(attribute.get<std::string>());
    } catch(json::out_of_range& e) {
        return nullptr;
    }
}

std::vector<std::string> *getStringsFromJson(const json& jsonObj, const std::string& key)
{
    try {
        json jsonstrings = key == "" ? jsonObj : jsonObj.at(key);
        std::vector<std::string> *strings = new std::vector<std::string>;

        for (unsigned int i = 0 ; i < jsonstrings.size() ; i++) {
            strings->push_back(*valueNoNull(jsonstrings[i], ""));
        }

        return strings;
    } catch(json::out_of_range& e) {
        return nullptr;
    }
}


User *getUserFromJson(const json& jsonObj, const std::string& key)
{
    try {
        json jsonUser = key == "" ? jsonObj : jsonObj.at(key);

        return new User {
            valueNoNull(jsonUser, "username"     ),
            valueNoNull(jsonUser, "discriminator"),
            valueNoNull(jsonUser, "avatar"       ),
            valueNoNull(jsonUser, "locale"       ),
            valueNoNull(jsonUser, "email"        ),
            valueNoNull(jsonUser, "id"           ),

            jsonUser.value("flags"       , -1),
            jsonUser.value("premium_type", -1),
            jsonUser.value("public_flags", -1),

            jsonUser.value("bot"        , false),
            jsonUser.value("system"     , false),
            jsonUser.value("mfa_enabled", false),
            jsonUser.value("verified"   , false)
        };
    } catch(json::out_of_range& e) {
        return nullptr;
    }
}

std::vector<User *> *getUsersFromJson(const json& jsonObj, const std::string& key)
{
    try {
        json jsonRecipients = key == "" ? jsonObj : jsonObj.at(key);
        std::vector<User *> *recipients = new std::vector<User *>;

        for (unsigned int i = 0 ; i < jsonRecipients.size() ; i++) {
            recipients->push_back(getUserFromJson(jsonRecipients[i], std::string("")));
        }

        return recipients;
    } catch(json::out_of_range& e) {
        return nullptr;
    }
}

Overwrite *getOverwriteFromJson(const json& jsonObj, const std::string& key)
{
    try {
        json jsonOverwrite = key == "" ? jsonObj : jsonObj.at(key);

        return new Overwrite {
            valueNoNull(jsonOverwrite, "id"   ),
            valueNoNull(jsonOverwrite, "allow"),
            valueNoNull(jsonOverwrite, "deny" ),

            jsonOverwrite.value("type", -1)
        };
    } catch(json::out_of_range& e) {
        return nullptr;
    }
}

std::vector<Overwrite *> *getOverwritesFromJson(const json& jsonObj, const std::string& key)
{
    try {
        json jsonOverwrites = key == "" ? jsonObj : jsonObj.at(key);
        std::vector<Overwrite *> *overwrites = new std::vector<Overwrite *>;

        for (unsigned int i = 0 ; i < jsonOverwrites.size() ; i++) {
            overwrites->push_back(getOverwriteFromJson(jsonOverwrites[i], std::string("")));
        }

        return overwrites;
    } catch(json::out_of_range& e) {
        return nullptr;
    }
}

ThreadMember *getThreadMemberFromJson(const json& jsonObj, const std::string& key)
{
    try {
        json jsonThreadMember = key == "" ? jsonObj : jsonObj.at(key);

        return new ThreadMember {
            valueNoNull(jsonThreadMember, "join_timestamp"),
            valueNoNull(jsonThreadMember, "id"            ),
            valueNoNull(jsonThreadMember, "user_id"       ),

            jsonThreadMember.value("flags", -1)
        };
    } catch(json::out_of_range& e) {
        return nullptr;
    }
}

ThreadMetadata *getThreadMetadataFromJson(const json& jsonObj, const std::string& key)
{
    try {
        json jsonThreadMetadata = key == "" ? jsonObj : jsonObj.at(key);

        return new ThreadMetadata {
            valueNoNull(jsonThreadMetadata, "archive_timestamp"),

            jsonThreadMetadata.value("auto_archive_duration", -1),

            jsonThreadMetadata.value("archived", false),
            jsonObj.value("locked"  , false)
        };
    } catch(json::out_of_range& e) {
        return nullptr;
    }
}

Channel *getChannelFromJson(const json& jsonObj, const std::string& key)
{
    try {
        json jsonChannel = key == "" ? jsonObj : jsonObj.at(key);

        return new Channel {
            getUsersFromJson(jsonChannel, "recipients"),
            getOverwritesFromJson(jsonChannel, "permission_overwrites"),
            getThreadMemberFromJson(jsonChannel, "member"),
            getThreadMetadataFromJson(jsonChannel, "thread_metadata"),

            valueNoNull(jsonChannel, "id"                ),
            valueNoNull(jsonChannel, "name"              ),
            valueNoNull(jsonChannel, "topic"             ),
            valueNoNull(jsonChannel, "icon"              ),
            valueNoNull(jsonChannel, "last_pin_timestamp"),
            valueNoNull(jsonChannel, "rtc_region"        ),
            valueNoNull(jsonChannel, "permissions"       ),
            valueNoNull(jsonChannel, "guild_id"          ),
            valueNoNull(jsonChannel, "last_message_id"   ),
            valueNoNull(jsonChannel, "owner_id"          ),
            valueNoNull(jsonChannel, "application_id"    ),
            valueNoNull(jsonChannel, "parent_id"         ),

            jsonChannel.value("type"                         , -1),
            jsonChannel.value("position"                     , -1),
            jsonChannel.value("bitrate"                      , -1),
            jsonChannel.value("user_limit"                   , -1),
            jsonChannel.value("rate_limit_per_user"          , -1),
            jsonChannel.value("video_quality_mode"           , -1),
            jsonChannel.value("message_count"                , -1),
            jsonChannel.value("member_count"                 , -1),
            jsonChannel.value("default_auto_archive_duration", -1),

            jsonChannel.value("nsfw", false)
        };
    } catch(json::out_of_range& e) {
        return nullptr;
    }
}

std::vector<Channel *> *getChannelsFromJson(const json& jsonObj, const std::string& key)
{
    try {
        json jsonChannels = key == "" ? jsonObj : jsonObj.at(key);
        std::vector<Channel *> *channels = new std::vector<Channel *>;

        for (unsigned int i = 0 ; i < jsonChannels.size() ; i++) {
            channels->push_back(getChannelFromJson(jsonChannels[i], ""));
        }

        return channels;
    } catch(json::out_of_range& e) {
        return nullptr;
    }
}

TeamMember *getTeamMemberFromJson(const json& jsonObj, const std::string& key)
{
    try {
        json jsonTeamMember = key == "" ? jsonObj : jsonObj.at(key);

        return new TeamMember {
            getUserFromJson(jsonTeamMember, "user"),
            getStringsFromJson(jsonTeamMember, "permissions"),

            valueNoNull(jsonTeamMember, "team_id"),

            jsonTeamMember.value("member_ship_state", -1)
        };
    } catch(json::out_of_range& e) {
        return nullptr;
    }
}

std::vector<TeamMember *> *getTeamMembersFromJson(const json& jsonObj, const std::string& key)
{
    try {
        json jsonTeamMembers = key == "" ? jsonObj : jsonObj.at(key);
        std::vector<TeamMember *> *teamMembers = new std::vector<TeamMember *>;

        for (unsigned int i = 0 ; i < jsonTeamMembers.size() ; i++) {
            teamMembers->push_back(getTeamMemberFromJson(jsonTeamMembers[i], std::string("")));
        }

        return teamMembers;
    } catch(json::out_of_range& e) {
        return nullptr;
    }
}

Team *getTeamFromJson(const json& jsonObj, const std::string& key)
{
    try {
        json jsonTeam = key == "" ? jsonObj : jsonObj.at(key);

        return new Team {
            getTeamMembersFromJson(jsonTeam, "members"),

            valueNoNull(jsonTeam, "icon"         ),
            valueNoNull(jsonTeam, "id"           ),
            valueNoNull(jsonTeam, "name"         ),
            valueNoNull(jsonTeam, "owner_user_id")
        };
    } catch(json::out_of_range& e) {
        return nullptr;
    }
}

Application *getApplicationFromJson(const json& jsonObj, const std::string& key)
{
    try {
        json jsonApplication = key == "" ? jsonObj : jsonObj.at(key);

        return new Application {
            getUserFromJson(jsonApplication, "owner"),
            getTeamFromJson(jsonApplication, "team"),
            getStringsFromJson(jsonApplication, "rpc_origins"),

            valueNoNull(jsonApplication, "id"                  ),
            valueNoNull(jsonApplication, "name"                ),
            valueNoNull(jsonApplication, "icon"                ),
            valueNoNull(jsonApplication, "description"         ),
            valueNoNull(jsonApplication, "terms_of_service_url"),
            valueNoNull(jsonApplication, "privacy_policy_url"  ),
            valueNoNull(jsonApplication, "summary"             ),
            valueNoNull(jsonApplication, "verify_key"          ),
            valueNoNull(jsonApplication, "guild_id"            ),
            valueNoNull(jsonApplication, "primary_sku_id"      ),
            valueNoNull(jsonApplication, "slug"                ),
            valueNoNull(jsonApplication, "cover_image"         ),

            jsonApplication.value("flags", -1),

            jsonApplication.value("bot_public"            , false),
            jsonApplication.value("bot_require_code_grant", false)
        };
    } catch(json::out_of_range& e) {
        return nullptr;
    }
}

MessageActivity *getMessageActivityFromJson(const json& jsonObj, const std::string& key)
{
    try {
        json jsonMessageActivity = key == "" ? jsonObj : jsonObj.at(key);

        return new MessageActivity {
            valueNoNull(jsonMessageActivity, "party_id"),

            jsonMessageActivity.value("type", -1)
        };
    } catch(json::out_of_range& e) {
        return nullptr;
    }
}

GuildMessageMember *getGuildMessageMemberFromJson(const json& jsonObj, const std::string& key)
{
    try {
        json jsonGuildMessageMember = key == "" ? jsonObj : jsonObj.at(key);

        return new GuildMessageMember {
            getStringsFromJson(jsonGuildMessageMember, "roles"),

            valueNoNull(jsonGuildMessageMember, "nick"         ),
            valueNoNull(jsonGuildMessageMember, "joined_at"    ),
            valueNoNull(jsonGuildMessageMember, "premium_since"),
            valueNoNull(jsonGuildMessageMember, "permissions"  ),

            jsonGuildMessageMember.value("deaf"   , false),
            jsonGuildMessageMember.value("mute"   , false),
            jsonGuildMessageMember.value("pending", false)
        };
    } catch(json::out_of_range& e) {
        return nullptr;
    }
}

MessageInteraction *getMessageInteractionFromJson(const json& jsonObj, const std::string& key)
{
    try {
        json jsonMessageInteraction = key == "" ? jsonObj : jsonObj.at(key);

        return new MessageInteraction {
            getUserFromJson(jsonMessageInteraction, "user"),

            valueNoNull(jsonMessageInteraction, "id"  ),
            valueNoNull(jsonMessageInteraction, "name"),

            jsonMessageInteraction.value("type", -1)
        };
    } catch(json::out_of_range& e) {
        return nullptr;
    }
}

Emoji *getEmojiFromJson(const json& jsonObj, const std::string& key)
{
    try {
        json jsonEmoji = key == "" ? jsonObj : jsonObj.at(key);

        return new Emoji {
            getUserFromJson(jsonEmoji, "user"),
            getStringsFromJson(jsonEmoji, "roles"),

            valueNoNull(jsonEmoji, "id"),
            valueNoNull(jsonEmoji, "name"),

            jsonEmoji.value("requireColons", false),
            jsonEmoji.value("managed"      , false),
            jsonEmoji.value("animated"     , false),
            jsonEmoji.value("available"    , false)
        };
    } catch(json::out_of_range& e) {
        return nullptr;
    }
}

Reaction *getReactionFromJson(const json& jsonObj, const std::string& key)
{
    try {
        json jsonReaction = key == "" ? jsonObj : jsonObj.at(key);

        return new Reaction {
            getEmojiFromJson(jsonReaction, "emoji"),

            jsonReaction.value("count", -1),

            jsonReaction.value("me", false)
        };
    } catch(json::out_of_range& e) {
        return nullptr;
    }
}

std::vector<Reaction *> *getReactionsFromJson(const json& jsonObj, const std::string& key)
{
    try {
        json jsonReactions = key == "" ? jsonObj : jsonObj.at(key);
        std::vector<Reaction *> *reactions = new std::vector<Reaction *>;

        for (unsigned int i = 0 ; i < jsonReactions.size() ; i++) {
            reactions->push_back(getReactionFromJson(jsonReactions[i], ""));
        }

        return reactions;
    } catch(json::out_of_range& e) {
        return nullptr;
    }
}

EmbedField *getEmbedFieldFromJson(const json& jsonObj, const std::string& key)
{
    try {
        json jsonEmbedField = key == "" ? jsonObj : jsonObj.at(key);

        return new EmbedField {
            valueNoNull(jsonEmbedField, "name"),
            valueNoNull(jsonEmbedField, "value"),

            jsonEmbedField.value("inline", false)
        };
    } catch(json::out_of_range& e) {
        return nullptr;
    }
}

std::vector<EmbedField *> *getEmbedFieldsFromJson(const json& jsonObj, const std::string& key)
{
    try {
        json jsonEmbedFields = key == "" ? jsonObj : jsonObj.at(key);
        std::vector<EmbedField *> *embedFields = new std::vector<EmbedField *>;

        for (unsigned int i = 0 ; i < jsonEmbedFields.size() ; i++) {
            embedFields->push_back(getEmbedFieldFromJson(jsonEmbedFields[i], ""));
        }

        return embedFields;
    } catch(json::out_of_range& e) {
        return nullptr;
    }
}

EmbedFooter *getEmbedFooterFromJson(const json& jsonObj, const std::string& key)
{
    try {
        json jsonEmbedFooter = key == "" ? jsonObj : jsonObj.at(key);

        return new EmbedFooter {
            valueNoNull(jsonEmbedFooter, "text"          ),
            valueNoNull(jsonEmbedFooter, "icon_url"      ),
            valueNoNull(jsonEmbedFooter, "proxy_icon_url")
        };
    } catch(json::out_of_range& e) {
        return nullptr;
    }
}

EmbedTVI *getEmbedTVIFromJson(const json& jsonObj, const std::string& key)
{
    try {
        json jsonEmbedTVI = key == "" ? jsonObj : jsonObj.at(key);

        return new EmbedTVI {
            valueNoNull(jsonEmbedTVI, "url"      ),
            valueNoNull(jsonEmbedTVI, "proxy_url"),

            jsonEmbedTVI.value("height", -1),
            jsonEmbedTVI.value("width" , -1)
        };
    } catch(json::out_of_range& e) {
        return nullptr;
    }
}

EmbedProvider *getEmbedProviderFromJson(const json& jsonObj, const std::string& key)
{
    try {
        json jsonEmbedProvider = key == "" ? jsonObj : jsonObj.at(key);

        return new EmbedProvider {
            valueNoNull(jsonEmbedProvider, "name"),
            valueNoNull(jsonEmbedProvider, "url" )
        };
    } catch(json::out_of_range& e) {
        return nullptr;
    }
}

EmbedAuthor *getEmbedAuthorFromJson(const json& jsonObj, const std::string& key)
{
    try {
        json jsonEmbedAuthor = key == "" ? jsonObj : jsonObj.at(key);

        return new EmbedAuthor {
            valueNoNull(jsonEmbedAuthor, "name"          ),
            valueNoNull(jsonEmbedAuthor, "url"           ),
            valueNoNull(jsonEmbedAuthor, "icon_url"      ),
            valueNoNull(jsonEmbedAuthor, "proxy_icon_url")
        };
    } catch(json::out_of_range& e) {
        return nullptr;
    }
}

Embed *getEmbedFromJson(const json& jsonObj, const std::string& key)
{
    try {
        json jsonEmbed = key == "" ? jsonObj : jsonObj.at(key);

        return new Embed {
            getEmbedFieldsFromJson(jsonEmbed, "fields"),
            getEmbedFooterFromJson(jsonEmbed, "footer"),
            getEmbedTVIFromJson(jsonEmbed, "image"),
            getEmbedTVIFromJson(jsonEmbed, "thumbnail"),
            getEmbedTVIFromJson(jsonEmbed, "video"),
            getEmbedProviderFromJson(jsonEmbed, "provider"),
            getEmbedAuthorFromJson(jsonEmbed, "author"),

            valueNoNull(jsonEmbed, "title"      ),
            valueNoNull(jsonEmbed, "type"       ),
            valueNoNull(jsonEmbed, "description"),
            valueNoNull(jsonEmbed, "url"        ),
            valueNoNull(jsonEmbed, "timestamp"  ),

            jsonEmbed.value("color", -1)
        };
    } catch(json::out_of_range& e) {
        return nullptr;
    }
}

std::vector<Embed *> *getEmbedsFromJson(const json& jsonObj, const std::string& key)
{
    try {
        json jsonEmbeds = key == "" ? jsonObj : jsonObj.at(key);
        std::vector<Embed *> *embeds = new std::vector<Embed *>;

        for (unsigned int i = 0 ; i < jsonEmbeds.size() ; i++) {
            embeds->push_back(getEmbedFromJson(jsonEmbeds[i], ""));
        }

        return embeds;
    } catch(json::out_of_range& e) {
        return nullptr;
    }
}

Attachment *getAttachmentFromJson(const json& jsonObj, const std::string& key)
{
    try {
        json jsonAttachment = key == "" ? jsonObj : jsonObj.at(key);

        return new Attachment {
            valueNoNull(jsonAttachment, "id"          ),
            valueNoNull(jsonAttachment, "filename"    ),
            valueNoNull(jsonAttachment, "content_type"),
            valueNoNull(jsonAttachment, "url"         ),
            valueNoNull(jsonAttachment, "proxy_url"   ),

            jsonAttachment.value("size"  , -1),
            jsonAttachment.value("height", -1),
            jsonAttachment.value("width" , -1)
        };
    } catch(json::out_of_range& e) {
        return nullptr;
    }
}

std::vector<Attachment *> *getAttachmentsFromJson(const json& jsonObj, const std::string& key)
{
    try {
        json jsonAttachments = key == "" ? jsonObj : jsonObj.at(key);
        std::vector<Attachment *> *attachments = new std::vector<Attachment *>;

        for (unsigned int i = 0 ; i < jsonAttachments.size() ; i++) {
            attachments->push_back(getAttachmentFromJson(jsonAttachments[i], ""));
        }

        return attachments;
    } catch(json::out_of_range& e) {
        return nullptr;
    }
}

ChannelMention *getChannelMentionFromJson(const json& jsonObj, const std::string& key)
{
    try {
        json jsonChannelMention = key == "" ? jsonObj : jsonObj.at(key);

        return new ChannelMention {
            valueNoNull(jsonChannelMention, "id"      ),
            valueNoNull(jsonChannelMention, "guild_id"),
            valueNoNull(jsonChannelMention, "name"    ),

            jsonChannelMention.value("type", -1)
        };
    } catch(json::out_of_range& e) {
        return nullptr;
    }
}

std::vector<ChannelMention *> *getChannelMentionsFromJson(const json& jsonObj, const std::string& key)
{
    try {
        json jsonChannelMentions = key == "" ? jsonObj : jsonObj.at(key);
        std::vector<ChannelMention *> *channelMentions = new std::vector<ChannelMention *>;

        for (unsigned int i = 0 ; i < jsonChannelMentions.size() ; i++) {
            channelMentions->push_back(getChannelMentionFromJson(jsonChannelMentions[i], ""));
        }

        return channelMentions;
    } catch(json::out_of_range& e) {
        return nullptr;
    }
}

SelectOption *getSelectOptionFromJson(const json& jsonObj, const std::string& key)
{
    try {
        json jsonSelectOption = key == "" ? jsonObj : jsonObj.at(key);

        return new SelectOption {
            getEmojiFromJson(jsonSelectOption, "emoji"),

            valueNoNull(jsonSelectOption, "label"      ),
            valueNoNull(jsonSelectOption, "value"      ),
            valueNoNull(jsonSelectOption, "description"),

            jsonSelectOption.value("default", false)
        };
    } catch(json::out_of_range& e) {
        return nullptr;
    }
}

std::vector<SelectOption *> *getSelectOptionsFromJson(const json& jsonObj, const std::string& key)
{
    try {
        json jsonSelectOptions = key == "" ? jsonObj : jsonObj.at(key);
        std::vector<SelectOption *> *selectOptions = new std::vector<SelectOption *>;

        for (unsigned int i = 0 ; i < jsonSelectOptions.size() ; i++) {
            selectOptions->push_back(getSelectOptionFromJson(jsonSelectOptions[i], ""));
        }

        return selectOptions;
    } catch(json::out_of_range& e) {
        return nullptr;
    }
}

MessageComponent *getPartialMessageComponentFromJson(const json& jsonObj, const std::string& key)
{
    try {
        json jsonMessageComponent = key == "" ? jsonObj : jsonObj.at(key);

        return new MessageComponent {
            getEmojiFromJson(jsonMessageComponent, "emoji"),
            getSelectOptionsFromJson(jsonMessageComponent, "components"),
            nullptr,

            valueNoNull(jsonMessageComponent, "custom_id"  ),
            valueNoNull(jsonMessageComponent, "label"      ),
            valueNoNull(jsonMessageComponent, "url"        ),
            valueNoNull(jsonMessageComponent, "placeholder"),

            jsonMessageComponent.value("type"      , -1),
            jsonMessageComponent.value("style"     , -1),
            jsonMessageComponent.value("min_values", -1),
            jsonMessageComponent.value("max_values", -1),

            jsonMessageComponent.value("disabled", false)
        };
    } catch(json::out_of_range& e) {
        return nullptr;
    }
}

std::vector<MessageComponent *> *getMessageComponentsFromJson(const json& jsonObj, const std::string& key)
{
    try {
        json jsonMessageComponents = key == "" ? jsonObj : jsonObj.at(key);
        std::vector<MessageComponent *> *messageComponents = new std::vector<MessageComponent *>;

        for (unsigned int i = 0 ; i < jsonMessageComponents.size() ; i++) {
            messageComponents->push_back(getPartialMessageComponentFromJson(jsonMessageComponents[i], ""));
        }

        return messageComponents;
    } catch(json::out_of_range& e) {
        return nullptr;
    }
}

StickerItem *getStickerItemFromJson(const json& jsonObj, const std::string& key)
{
    try {
        json jsonStickerItem = key == "" ? jsonObj : jsonObj.at(key);

        return new StickerItem {
            valueNoNull(jsonStickerItem, "id"  ),
            valueNoNull(jsonStickerItem, "name"),

            jsonStickerItem.value("format_type", -1)
        };
    } catch(json::out_of_range& e) {
        return nullptr;
    }
}

std::vector<StickerItem *> *getStickerItemsFromJson(const json& jsonObj, const std::string& key)
{
    try {
        json jsonStickerItems = key == "" ? jsonObj : jsonObj.at(key);
        std::vector<StickerItem *> *stickerItems = new std::vector<StickerItem *>;

        for (unsigned int i = 0 ; i < jsonStickerItems.size() ; i++) {
            stickerItems->push_back(getStickerItemFromJson(jsonStickerItems[i], ""));
        }

        return stickerItems;
    } catch(json::out_of_range& e) {
        return nullptr;
    }
}

Sticker *getStickerFromJson(const json& jsonObj, const std::string& key)
{
    try {
        json jsonSticker = key == "" ? jsonObj : jsonObj.at(key);

        return new Sticker {
            getUserFromJson(jsonSticker, "user"),

            valueNoNull(jsonSticker, "id"         ),
            valueNoNull(jsonSticker, "pack_id"    ),
            valueNoNull(jsonSticker, "name"       ),
            valueNoNull(jsonSticker, "description"),
            valueNoNull(jsonSticker, "tags"       ),
            valueNoNull(jsonSticker, "asset"      ),
            valueNoNull(jsonSticker, "guild_id"   ),

            jsonSticker.value("type"       , -1),
            jsonSticker.value("format_type", -1),
            jsonSticker.value("sort_value" , -1),

            jsonSticker.value("available", false)
        };
    } catch(json::out_of_range& e) {
        return nullptr;
    }
}

std::vector<Sticker *> *getStickersFromJson(const json& jsonObj, const std::string& key)
{
    try {
        json jsonStickers = key == "" ? jsonObj : jsonObj.at(key);
        std::vector<Sticker *> *stickers = new std::vector<Sticker *>;

        for (unsigned int i = 0 ; i < jsonStickers.size() ; i++) {
            stickers->push_back(getStickerFromJson(jsonStickers[i], ""));
        }

        return stickers;
    } catch(json::out_of_range& e) {
        return nullptr;
    }
}

Message *getPartialMessageFromJson(const json& jsonObj, const std::string& key)
{
    try {
        json jsonMessage = key == "" ? jsonObj : jsonObj.at(key);

        return new Message {
            getApplicationFromJson(jsonMessage, "application"),
            getUserFromJson(jsonMessage, "author"),
            getMessageActivityFromJson(jsonMessage, "activity"),
            getGuildMessageMemberFromJson(jsonMessage, "member"),
            nullptr,
            getChannelFromJson(jsonMessage, "thread"),
            getMessageInteractionFromJson(jsonMessage, "interaction"),

            getReactionsFromJson(jsonMessage, "reactions"),
            nullptr,
            getUsersFromJson(jsonMessage, "mentions"),
            getAttachmentsFromJson(jsonMessage, "attachments"),
            getChannelMentionsFromJson(jsonMessage, "mention_channels"),
            getStringsFromJson(jsonMessage, "mention_roles"),
            getMessageComponentsFromJson(jsonMessage, "components"),
            getStickerItemsFromJson(jsonMessage, "sticker_items"),
            getStickersFromJson(jsonMessage, "stickers"),

            valueNoNull(jsonMessage, "id"              ),
            valueNoNull(jsonMessage, "channel_id"      ),
            valueNoNull(jsonMessage, "guild_id"        ),
            valueNoNull(jsonMessage, "content"         ),
            valueNoNull(jsonMessage, "timestamp"       ),
            valueNoNull(jsonMessage, "edited_timestamp"),
            valueNoNull(jsonMessage, "webhook_id"      ),
            valueNoNull(jsonMessage, "application_id"  ),
            valueNoNull(jsonMessage, "nonce"           ),

            //jsonMessage.value("nonce"              , -1),
            -1,
            jsonMessage.value("author_public_flags", -1),
            jsonMessage.value("type"               , -1),
            jsonMessage.value("flags"              , -1),

            jsonMessage.value("tts"             , false),
            jsonMessage.value("pinned"          , false),
            jsonMessage.value("mention_everyone", false)
        };
    } catch(json::out_of_range& e) {
        return nullptr;
    }
}

Message *getMessageFromJson(const json& jsonObj, const std::string& key)
{
    try {
        json jsonMessage = key == "" ? jsonObj : jsonObj.at(key);

        return new Message {
            getApplicationFromJson(jsonMessage, "application"),
            getUserFromJson(jsonMessage, "author"),
            getMessageActivityFromJson(jsonMessage, "activity"),
            getGuildMessageMemberFromJson(jsonMessage, "member"),
            getPartialMessageFromJson(jsonMessage, "referenced_message"),
            getChannelFromJson(jsonMessage, "thread"),
            getMessageInteractionFromJson(jsonMessage, "interaction"),

            getReactionsFromJson(jsonMessage, "reactions"),
            nullptr,
            getUsersFromJson(jsonMessage, "mentions"),
            getAttachmentsFromJson(jsonMessage, "attachments"),
            getChannelMentionsFromJson(jsonMessage, "mention_channels"),
            getStringsFromJson(jsonMessage, "mention_roles"),
            getMessageComponentsFromJson(jsonMessage, "components"),
            getStickerItemsFromJson(jsonMessage, "sticker_items"),
            getStickersFromJson(jsonMessage, "stickers"),

            valueNoNull(jsonMessage, "id"              ),
            valueNoNull(jsonMessage, "channel_id"      ),
            valueNoNull(jsonMessage, "guild_id"        ),
            valueNoNull(jsonMessage, "content"         ),
            valueNoNull(jsonMessage, "timestamp"       ),
            valueNoNull(jsonMessage, "edited_timestamp"),
            valueNoNull(jsonMessage, "webhook_id"      ),
            valueNoNull(jsonMessage, "application_id"  ),
            valueNoNull(jsonMessage, "nonce"           ),

            jsonMessage.value("nonce"              , -1),
            jsonMessage.value("author_public_flags", -1),
            jsonMessage.value("type"               , -1),
            jsonMessage.value("flags"              , -1),

            jsonMessage.value("tts"             , false),
            jsonMessage.value("pinned"          , false),
            jsonMessage.value("mention_everyone", false)
        };
    } catch(json::out_of_range& e) {
        return nullptr;
    }
}

std::vector<Message *> *getMessagesFromJson(const json& jsonObj, const std::string& key)
{
    try {
        json jsonMessages = key == "" ? jsonObj : jsonObj.at(key);
        std::vector<Message *> *messages = new std::vector<Message *>;

        for (unsigned int i = 0 ; i < jsonMessages.size() ; i++) {
            messages->push_back(getMessageFromJson(jsonMessages[i], ""));
        }

        return messages;
    } catch(json::out_of_range& e) {
        return nullptr;
    }
}

GuildMember *getGuildMemberFromJson(const json& jsonObj, const std::string& key)
{
    try {
        json jsonGuildMember = key == "" ? jsonObj : jsonObj.at(key);

        return new GuildMember {
            getUserFromJson(jsonGuildMember, "user"),
            getStringsFromJson(jsonGuildMember, "roles"),

            valueNoNull(jsonGuildMember, "nick"         ),
            valueNoNull(jsonGuildMember, "avatar"       ),
            valueNoNull(jsonGuildMember, "joined_at"    ),
            valueNoNull(jsonGuildMember, "premium_since"),
            valueNoNull(jsonGuildMember, "permissions"  ),

            jsonGuildMember.value("deaf"   , false),
            jsonGuildMember.value("mute"   , false),
            jsonGuildMember.value("pending", false)
        };
    } catch(json::out_of_range& e) {
        return nullptr;
    }
}

std::vector<GuildMember *> *getGuildMembersFromJson(const json& jsonObj, const std::string& key)
{
    try {
        json jsonGuildMembers = key == "" ? jsonObj : jsonObj.at(key);
        std::vector<GuildMember *> *guildMembers = new std::vector<GuildMember *>;

        for (unsigned int i = 0 ; i < jsonGuildMembers.size() ; i++) {
            guildMembers->push_back(getGuildMemberFromJson(jsonGuildMembers[i], ""));
        }

        return guildMembers;
    } catch(json::out_of_range& e) {
        return nullptr;
    }
}

VoiceState *getVoiceStateFromJson(const json& jsonObj, const std::string& key)
{
    try {
        json jsonVoiceState = key == "" ? jsonObj : jsonObj.at(key);

        return new VoiceState {
            getGuildMemberFromJson(jsonVoiceState, "member"),

            valueNoNull(jsonVoiceState, "guild_id"                  ),
            valueNoNull(jsonVoiceState, "channel_id"                ),
            valueNoNull(jsonVoiceState, "user_id"                   ),
            valueNoNull(jsonVoiceState, "session_id"                ),
            valueNoNull(jsonVoiceState, "request_to_speak_timestamp"),

            jsonVoiceState.value("deaf"       , false),
            jsonVoiceState.value("mute"       , false),
            jsonVoiceState.value("self_deaf"  , false),
            jsonVoiceState.value("self_mute"  , false),
            jsonVoiceState.value("self_stream", false),
            jsonVoiceState.value("self_video" , false),
            jsonVoiceState.value("suppress"   , false)
        };
    } catch(json::out_of_range& e) {
        return nullptr;
    }
}

std::vector<VoiceState *> *getVoiceStatesFromJson(const json& jsonObj, const std::string& key)
{
    try {
        json jsonVoiceStates = key == "" ? jsonObj : jsonObj.at(key);
        std::vector<VoiceState *> *voiceStates = new std::vector<VoiceState *>;

        for (unsigned int i = 0 ; i < jsonVoiceStates.size() ; i++) {
            voiceStates->push_back(getVoiceStateFromJson(jsonVoiceStates[i], ""));
        }

        return voiceStates;
    } catch(json::out_of_range& e) {
        return nullptr;
    }
}

WelcomeScreenChannel *getWelcomeScreenChannelFromJson(const json& jsonObj, const std::string& key)
{
    try {
        json jsonWelcomeScreenChannel = key == "" ? jsonObj : jsonObj.at(key);

        return new WelcomeScreenChannel {
            valueNoNull(jsonWelcomeScreenChannel, "channel_id" ),
            valueNoNull(jsonWelcomeScreenChannel, "description"),
            valueNoNull(jsonWelcomeScreenChannel, "emoji_id"   ),
            valueNoNull(jsonWelcomeScreenChannel, "emoji_name" )
        };
    } catch(json::out_of_range& e) {
        return nullptr;
    }
}

std::vector<WelcomeScreenChannel *> *getWelcomeScreenChannelsFromJson(const json& jsonObj, const std::string& key)
{
    try {
        json jsonWelcomeScreenChannels = key == "" ? jsonObj : jsonObj.at(key);
        std::vector<WelcomeScreenChannel *> *welcomeScreenChannels = new std::vector<WelcomeScreenChannel *>;

        for (unsigned int i = 0 ; i < jsonWelcomeScreenChannels.size() ; i++) {
            welcomeScreenChannels->push_back(getWelcomeScreenChannelFromJson(jsonWelcomeScreenChannels[i], ""));
        }

        return welcomeScreenChannels;
    } catch(json::out_of_range& e) {
        return nullptr;
    }
}

WelcomeScreen *getWelcomeScreenFromJson(const json& jsonObj, const std::string& key)
{
    try {
        json jsonWelcomeScreen = key == "" ? jsonObj : jsonObj.at(key);

        return new WelcomeScreen {
            getWelcomeScreenChannelsFromJson(jsonWelcomeScreen, "welcome_channels"),

            valueNoNull(jsonWelcomeScreen, "description")
        };
    } catch(json::out_of_range& e) {
        return nullptr;
    }
}

StageInstance *getStageInstanceFromJson(const json& jsonObj, const std::string& key)
{
    try {
        json jsonStageInstance = key == "" ? jsonObj : jsonObj.at(key);

        return new StageInstance {
            valueNoNull(jsonStageInstance, "id"        ),
            valueNoNull(jsonStageInstance, "guild_id"  ),
            valueNoNull(jsonStageInstance, "channel_id"),
            valueNoNull(jsonStageInstance, "topic"     ),

            jsonStageInstance.value("privacy_level" , -1),

            jsonStageInstance.value("discoverable_disabled" , false)
        };
    } catch(json::out_of_range& e) {
        return nullptr;
    }
}

std::vector<StageInstance *> *getStageInstancesFromJson(const json& jsonObj, const std::string& key)
{
    try {
        json jsonStageInstances = key == "" ? jsonObj : jsonObj.at(key);
        std::vector<StageInstance *> *stageInstances = new std::vector<StageInstance *>;

        for (unsigned int i = 0 ; i < jsonStageInstances.size() ; i++) {
            stageInstances->push_back(getStageInstanceFromJson(jsonStageInstances[i], ""));
        }

        return stageInstances;
    } catch(json::out_of_range& e) {
        return nullptr;
    }
}

Guild *getGuildFromJson(const json& jsonObj, const std::string& key)
{
    try {
        json jsonGuild = key == "" ? jsonObj : jsonObj.at(key);

        return new Guild {
            getWelcomeScreenFromJson(jsonGuild, "welcome_screen"),
            getStringsFromJson(jsonGuild, "guild_features"),
            getVoiceStatesFromJson(jsonGuild, "voice_states"),
            getGuildMembersFromJson(jsonGuild, "members"),
            getChannelsFromJson(jsonGuild, "channels"),
            getChannelsFromJson(jsonGuild, "threads"),
            nullptr,
            getStageInstancesFromJson(jsonGuild, "stage_instances"),
            getStickersFromJson(jsonGuild, "stickers"),

            valueNoNull(jsonGuild, "id"                       ),
            valueNoNull(jsonGuild, "name"                     ),
            valueNoNull(jsonGuild, "icon"                     ),
            valueNoNull(jsonGuild, "icon_hash"                ),
            valueNoNull(jsonGuild, "splash"                   ),
            valueNoNull(jsonGuild, "discovery_splash"         ),
            valueNoNull(jsonGuild, "owner_id"                 ),
            valueNoNull(jsonGuild, "permissions"              ),
            valueNoNull(jsonGuild, "region"                   ),
            valueNoNull(jsonGuild, "afk_channel_id"           ),
            valueNoNull(jsonGuild, "widget_channel_id"        ),
            valueNoNull(jsonGuild, "application_id"           ),
            valueNoNull(jsonGuild, "system_channel_id"        ),
            valueNoNull(jsonGuild, "rules_channel_id"         ),
            valueNoNull(jsonGuild, "joined_at"                ),
            valueNoNull(jsonGuild, "vanity_url_code"          ),
            valueNoNull(jsonGuild, "description"              ),
            valueNoNull(jsonGuild, "banner"                   ),
            valueNoNull(jsonGuild, "preferred_locale"         ),
            valueNoNull(jsonGuild, "public_updates_channel_id"),

            jsonGuild.value("afk_timeout"                  , -1),
            jsonGuild.value("verification_level"           , -1),
            jsonGuild.value("default_message_notifications", -1),
            jsonGuild.value("explicit_content_filter"      , -1),
            jsonGuild.value("mfa_level"                    , -1),
            jsonGuild.value("system_channel_flags"         , -1),
            jsonGuild.value("member_count"                 , -1),
            jsonGuild.value("max_presences"                , -1),
            jsonGuild.value("max_members"                  , -1),
            jsonGuild.value("premium_tier"                 , -1),
            jsonGuild.value("premium_subscription_count"   , -1),
            jsonGuild.value("max_video_channel_users"      , -1),
            jsonGuild.value("approximate_member_count"     , -1),
            jsonGuild.value("approximate_presence_count"   , -1),
            jsonGuild.value("nsfw_level"                   , -1),

            jsonGuild.value("owner"         , false),
            jsonGuild.value("widget_enabled", false),
            jsonGuild.value("large"         , false),
            jsonGuild.value("unavailable"   , false)
        };
    } catch(json::out_of_range& e) {
        return nullptr;
    }
}

std::vector<Guild *> *getGuildsFromJson(const json& jsonObj, const std::string& key)
{
    try {
        json jsonGuilds = key == "" ? jsonObj : jsonObj.at(key);
        std::vector<Guild *> *guilds = new std::vector<Guild *>;

        for (unsigned int i = 0 ; i < jsonGuilds.size() ; i++) {
            guilds->push_back(getGuildFromJson(jsonGuilds[i], ""));
        }

        return guilds;
    } catch(json::out_of_range& e) {
        return nullptr;
    }
}

CustomStatus *getCustomStatusFromJson(const json& jsonObj, const std::string& key)
{
    try {
        json jsonCustomStatus = key == "" ? jsonObj : jsonObj.at(key);

        if (jsonCustomStatus.is_null()) return nullptr;

        return new CustomStatus {
            valueNoNull(jsonCustomStatus, "text"     ),
            valueNoNull(jsonCustomStatus, "expiresAt"),
            valueNoNull(jsonCustomStatus, "emojiName"),
            valueNoNull(jsonCustomStatus, "emojiId"  )
        };
    } catch(json::out_of_range& e) {
        return nullptr;
    }
}

FriendSourceFlags *getFriendSourceFlagsFromJson(const json& jsonObj, const std::string& key)
{
    try {
        json jsonFriendSourceFlags = key == "" ? jsonObj : jsonObj.at(key);

        return new FriendSourceFlags {
            jsonFriendSourceFlags.value("all"           , false),
            jsonFriendSourceFlags.value("mutual_friends", false),
            jsonFriendSourceFlags.value("mutual_guilds" , false)
        };
    } catch(json::out_of_range& e) {
        return nullptr;
    }
}

GuildFolder *getGuildFolderFromJson(const json& jsonObj, const std::string& key)
{
    try {
        json jsonGuildFolder = key == "" ? jsonObj : jsonObj.at(key);

        return new GuildFolder {
            getStringsFromJson(jsonGuildFolder, "guild_ids"),

            valueNoNull(jsonGuildFolder, "name"),

            jsonGuildFolder.value("id" , -1),
            //jsonGuildFolder.value("color" , -1)
            -1
        };
    } catch(json::out_of_range& e) {
        return nullptr;
    }
}

std::vector<GuildFolder *> *getGuildFoldersFromJson(const json& jsonObj, const std::string& key)
{
    try {
        json jsonGuildFolders = key == "" ? jsonObj : jsonObj.at(key);
        std::vector<GuildFolder *> *guildFolders = new std::vector<GuildFolder *>;

        for (unsigned int i = 0 ; i < jsonGuildFolders.size() ; i++) {
            guildFolders->push_back(getGuildFolderFromJson(jsonGuildFolders[i], ""));
        }

        return guildFolders;
    } catch(json::out_of_range& e) {
        return nullptr;
    }
}

ClientSettings *getClientSettingsFromJson(const json& jsonObj, const std::string& key)
{
    try {
        json jsonClientSettings = key == "" ? jsonObj : jsonObj.at(key);

        return new ClientSettings {
            getCustomStatusFromJson(jsonClientSettings, "custom_status"),
            getFriendSourceFlagsFromJson(jsonClientSettings, "friend_source_flags"),
            getGuildFoldersFromJson(jsonClientSettings, "guild_folders"),
            getStringsFromJson(jsonClientSettings, "guild_positions"),
            getStringsFromJson(jsonClientSettings, "restricted_guilds"),

            valueNoNull(jsonClientSettings, "locale"),
            valueNoNull(jsonClientSettings, "status"),
            valueNoNull(jsonClientSettings, "theme"),

            jsonClientSettings.value("afk_timeout"            , -1),
            jsonClientSettings.value("animate_stickers"       , -1),
            jsonClientSettings.value("explicit_content_filter", -1),
            jsonClientSettings.value("friend_discovery_flags" , -1),
            jsonClientSettings.value("timezone_offset"        , -1),

            jsonClientSettings.value("allow_accessibility_detection"   , false),
            jsonClientSettings.value("animate_emoji"                   , false),
            jsonClientSettings.value("contact_sync_enabled"            , false),
            jsonClientSettings.value("convert_emoticons"               , false),
            jsonClientSettings.value("default_guilds_restricted"       , false),
            jsonClientSettings.value("detect_platform_accounts"        , false),
            jsonClientSettings.value("developer_mode"                  , false),
            jsonClientSettings.value("disable_games_tab"               , false),
            jsonClientSettings.value("enable_tts_command"              , false),
            jsonClientSettings.value("gif_auto_play"                   , false),
            jsonClientSettings.value("inline_attachment_media"         , false),
            jsonClientSettings.value("inline_embed_media"              , false),
            jsonClientSettings.value("message_display_compact"         , false),
            jsonClientSettings.value("native_phone_integration_enabled", false),
            jsonClientSettings.value("render_embeds"                   , false),
            jsonClientSettings.value("render_reactions"                , false),
            jsonClientSettings.value("show_current_game"               , false),
            jsonClientSettings.value("stream_notifications_enabled"    , false),
            jsonClientSettings.value("view_nsfw_guilds"                , false)
        };
    } catch(json::out_of_range& e) {
        return nullptr;
    }
}

Client *getClientFromJson(const json& jsonObj, const std::string& key)
{
    try {
        json jsonClient = key == "" ? jsonObj : jsonObj.at(key);

        return new Client {
            valueNoNull(jsonClient, "id"           ),
            valueNoNull(jsonClient, "username"     ),
            valueNoNull(jsonClient, "avatar"       ),
            valueNoNull(jsonClient, "discriminator"),
            valueNoNull(jsonClient, "banner"       ),
            valueNoNull(jsonClient, "bio"          ),
            valueNoNull(jsonClient, "locale"       ),
            valueNoNull(jsonClient, "email"        ),
            valueNoNull(jsonClient, "phone"        ),

            jsonClient.value("public_flags"   , -1),
            jsonClient.value("flags"          , -1),
            jsonClient.value("purchased_flags", -1),
            //jsonClient.value("banner_color"   , -1),
            //jsonClient.value("accent_color"   , -1),
            -1,
            -1,

            jsonClient.value("nsfw_allowed", false),
            jsonClient.value("mfa_enabled" , false),
            jsonClient.value("verified"    , false)
        };
    } catch(json::out_of_range& e) {
        return nullptr;
    }
}

} // namespace Api
