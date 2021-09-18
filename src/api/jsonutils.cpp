#include "../../include/api/jsonutils.h"

namespace Api {

std::string *valueNoNull(const json& jsonObj, const std::string& key)
{
    try {
        json attribute = key == "" ? jsonObj : jsonObj.at(key);
        if (attribute.is_null())
            return NULL;
        return new std::string(attribute.get<std::string>());
    } catch(json::out_of_range& e) {
        return NULL;
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
        return NULL;
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
        return NULL;
    }
}

std::vector<User> *getUsersFromJson(const json& jsonObj, const std::string& key)
{
    try {
        json jsonRecipients = key == "" ? jsonObj : jsonObj.at(key);
        std::vector<User> *recipients = new std::vector<User>;

        for (unsigned int i = 0 ; i < jsonRecipients.size() ; i++) {
            recipients->push_back(*getUserFromJson(jsonRecipients[i], std::string("")));
        }

        return recipients;
    } catch(json::out_of_range& e) {
        return NULL;
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
        return NULL;
    }
}

std::vector<Overwrite> *getOverwritesFromJson(const json& jsonObj, const std::string& key)
{
    try {
        json jsonOverwrites = key == "" ? jsonObj : jsonObj.at(key);
        std::vector<Overwrite> *overwrites = new std::vector<Overwrite>;

        for (unsigned int i = 0 ; i < jsonOverwrites.size() ; i++) {
            overwrites->push_back(*getOverwriteFromJson(jsonOverwrites[i], std::string("")));
        }

        return overwrites;
    } catch(json::out_of_range& e) {
        return NULL;
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
        return NULL;
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
        return NULL;
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
        return NULL;
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
        return NULL;
    }
}

std::vector<TeamMember> *getTeamMembersFromJson(const json& jsonObj, const std::string& key)
{
    try {
        json jsonTeamMembers = key == "" ? jsonObj : jsonObj.at(key);
        std::vector<TeamMember> *teamMembers = new std::vector<TeamMember>;

        for (unsigned int i = 0 ; i < jsonTeamMembers.size() ; i++) {
            teamMembers->push_back(*getTeamMemberFromJson(jsonTeamMembers[i], std::string("")));
        }

        return teamMembers;
    } catch(json::out_of_range& e) {
        return NULL;
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
        return NULL;
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
        return NULL;
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
        return NULL;
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
        return NULL;
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
        return NULL;
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
        return NULL;
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
        return NULL;
    }
}

std::vector<Reaction> *getReactionsFromJson(const json& jsonObj, const std::string& key)
{
    try {
        json jsonReactions = key == "" ? jsonObj : jsonObj.at(key);
        std::vector<Reaction> *reactions = new std::vector<Reaction>;

        for (unsigned int i = 0 ; i < jsonReactions.size() ; i++) {
            reactions->push_back(*getReactionFromJson(jsonReactions[i], ""));
        }https://discord.com/developers/docs/topics/opcodes-and-status-codes#gateway-opcodes

        return reactions;
    } catch(json::out_of_range& e) {
        return NULL;
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
        return NULL;
    }
}

std::vector<EmbedField> *getEmbedFieldsFromJson(const json& jsonObj, const std::string& key)
{
    try {
        json jsonEmbedFields = key == "" ? jsonObj : jsonObj.at(key);
        std::vector<EmbedField> *embedFields = new std::vector<EmbedField>;

        for (unsigned int i = 0 ; i < jsonEmbedFields.size() ; i++) {
            embedFields->push_back(*getEmbedFieldFromJson(jsonEmbedFields[i], ""));
        }

        return embedFields;
    } catch(json::out_of_range& e) {
        return NULL;
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
        return NULL;
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
        return NULL;
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
        return NULL;
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
        return NULL;
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
        return NULL;
    }
}

std::vector<Embed> *getEmbedsFromJson(const json& jsonObj, const std::string& key)
{
    try {
        json jsonEmbeds = key == "" ? jsonObj : jsonObj.at(key);
        std::vector<Embed> *embeds = new std::vector<Embed>;

        for (unsigned int i = 0 ; i < jsonEmbeds.size() ; i++) {
            embeds->push_back(*getEmbedFromJson(jsonEmbeds[i], ""));
        }

        return embeds;
    } catch(json::out_of_range& e) {
        return NULL;
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
        return NULL;
    }
}

std::vector<Attachment> *getAttachmentsFromJson(const json& jsonObj, const std::string& key)
{
    try {
        json jsonAttachments = key == "" ? jsonObj : jsonObj.at(key);
        std::vector<Attachment> *attachments = new std::vector<Attachment>;

        for (unsigned int i = 0 ; i < jsonAttachments.size() ; i++) {
            attachments->push_back(*getAttachmentFromJson(jsonAttachments[i], ""));
        }

        return attachments;
    } catch(json::out_of_range& e) {
        return NULL;
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
        return NULL;
    }
}

std::vector<ChannelMention> *getChannelMentionsFromJson(const json& jsonObj, const std::string& key)
{
    try {
        json jsonChannelMentions = key == "" ? jsonObj : jsonObj.at(key);
        std::vector<ChannelMention> *channelMentions = new std::vector<ChannelMention>;

        for (unsigned int i = 0 ; i < jsonChannelMentions.size() ; i++) {
            channelMentions->push_back(*getChannelMentionFromJson(jsonChannelMentions[i], ""));
        }

        return channelMentions;
    } catch(json::out_of_range& e) {
        return NULL;
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
        return NULL;
    }
}

std::vector<SelectOption> *getSelectOptionsFromJson(const json& jsonObj, const std::string& key)
{
    try {
        json jsonSelectOptions = key == "" ? jsonObj : jsonObj.at(key);
        std::vector<SelectOption> *selectOptions = new std::vector<SelectOption>;

        for (unsigned int i = 0 ; i < jsonSelectOptions.size() ; i++) {
            selectOptions->push_back(*getSelectOptionFromJson(jsonSelectOptions[i], ""));
        }

        return selectOptions;
    } catch(json::out_of_range& e) {
        return NULL;
    }
}

MessageComponent *getMessageComponentFromJsonNR(const json& jsonObj, const std::string& key)
{
    try {
        json jsonMessageComponent = key == "" ? jsonObj : jsonObj.at(key);

        return new MessageComponent {
            getEmojiFromJson(jsonMessageComponent, "emoji"),
            getSelectOptionsFromJson(jsonMessageComponent, "components"),
            NULL,

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
        return NULL;
    }
}

std::vector<MessageComponent> *getMessageComponentsFromJson(const json& jsonObj, const std::string& key)
{
    try {
        json jsonMessageComponents = key == "" ? jsonObj : jsonObj.at(key);
        std::vector<MessageComponent> *messageComponents = new std::vector<MessageComponent>;

        for (unsigned int i = 0 ; i < jsonMessageComponents.size() ; i++) {
            messageComponents->push_back(*getMessageComponentFromJsonNR(jsonMessageComponents[i], ""));
        }

        return messageComponents;
    } catch(json::out_of_range& e) {
        return NULL;
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
        return NULL;
    }
}

std::vector<StickerItem> *getStickerItemsFromJson(const json& jsonObj, const std::string& key)
{
    try {
        json jsonStickerItems = key == "" ? jsonObj : jsonObj.at(key);
        std::vector<StickerItem> *stickerItems = new std::vector<StickerItem>;

        for (unsigned int i = 0 ; i < jsonStickerItems.size() ; i++) {
            stickerItems->push_back(*getStickerItemFromJson(jsonStickerItems[i], ""));
        }

        return stickerItems;
    } catch(json::out_of_range& e) {
        return NULL;
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
        return NULL;
    }
}

std::vector<Sticker> *getStickersFromJson(const json& jsonObj, const std::string& key)
{
    try {
        json jsonStickers = key == "" ? jsonObj : jsonObj.at(key);
        std::vector<Sticker> *stickers = new std::vector<Sticker>;

        for (unsigned int i = 0 ; i < jsonStickers.size() ; i++) {
            stickers->push_back(*getStickerFromJson(jsonStickers[i], ""));
        }

        return stickers;
    } catch(json::out_of_range& e) {
        return NULL;
    }
}

Message *getMessageFromJsonNR(const json& jsonObj, const std::string& key)
{
    try {
        json jsonMessage = key == "" ? jsonObj : jsonObj.at(key);

        return new Message {
            getApplicationFromJson(jsonMessage, "application"),
            getUserFromJson(jsonMessage, "author"),
            getMessageActivityFromJson(jsonMessage, "activity"),
            getGuildMessageMemberFromJson(jsonMessage, "member"),
            NULL,
            getChannelFromJson(jsonMessage, "thread"),
            getMessageInteractionFromJson(jsonMessage, "interaction"),

            getReactionsFromJson(jsonMessage, "reactions"),
            NULL,
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
        return NULL;
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
            getMessageFromJsonNR(jsonMessage, "referenced_message"),
            getChannelFromJson(jsonMessage, "thread"),
            getMessageInteractionFromJson(jsonMessage, "interaction"),

            getReactionsFromJson(jsonMessage, "reactions"),
            NULL,
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
        return NULL;
    }
}

} // namespace Api
