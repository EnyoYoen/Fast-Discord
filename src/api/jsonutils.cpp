#include "api/jsonutils.h"

#include "api/message.h"
#include "api/attachment.h"
#include "api/user.h"
#include "api/overwrite.h"
#include "api/channel.h"
#include "api/thread.h"
#include "api/team.h"
#include "api/application.h"
#include "api/guildmember.h"
#include "api/voice.h"
#include "api/guild.h"
#include "api/client.h"

#include <string>

namespace Api {

// Get a std::string at the specified key, or nullptr if there's nothing
std::string *valueNoNull(json jsonObj, const std::string& key)
{
    try {
        // Getting the JSON object at 'key', if not empty
        json attribute = key == static_cast<std::string>("") ? jsonObj : jsonObj.at(key);
        if (attribute.is_null())
            return nullptr; // The attribute at the key specified is null
        return new std::string(attribute.get<std::string>());
    } catch(std::exception&) {
        return nullptr; // There is nothing at the key specified
    }
}

std::vector<std::string> *getStringsFromJson(json jsonObj, const std::string& key)
{
    try {
        // Getting the JSON object at 'key', if not empty
        json jsonstrings = key == static_cast<std::string>("") ? jsonObj : jsonObj.at(key);
        std::vector<std::string> *strings = new std::vector<std::string>;

        // Filling the vector
        for (unsigned int i = 0 ; i < jsonstrings.size() ; i++) {
            strings->push_back(*valueNoNull(jsonstrings[i], ""));
        }

        return strings;
    } catch(std::exception& e) {
        return nullptr; // There is nothing at the key specified
    }
}

// All the specialization of 'unmarshalObj'

template <>
void unmarshalObj<User>(json jsonObj, User **object)
{
    *object = new User {
        valueNoNull(jsonObj, "username"     ),
        valueNoNull(jsonObj, "discriminator"),
        valueNoNull(jsonObj, "avatar"       ),
        valueNoNull(jsonObj, "locale"       ),
        valueNoNull(jsonObj, "email"        ),
        valueNoNull(jsonObj, "id"           ),

        jsonObj.value("flags"       , -1),
        jsonObj.value("premium_type", -1),
        jsonObj.value("public_flags", -1),

        jsonObj.value("bot"        , false),
        jsonObj.value("system"     , false),
        jsonObj.value("mfa_enabled", false),
        jsonObj.value("verified"   , false)
    };
}

template <>
void unmarshalObj<Overwrite>(json jsonObj, Overwrite **object)
{
    *object = new Overwrite {
        valueNoNull(jsonObj, "id"   ),
        valueNoNull(jsonObj, "allow"),
        valueNoNull(jsonObj, "deny" ),

        jsonObj.value("type", -1)
    };
}

template <>
void unmarshalObj<ThreadMember>(json jsonObj, ThreadMember **object)
{
    *object = new ThreadMember {
        valueNoNull(jsonObj, "join_timestamp"),
        valueNoNull(jsonObj, "id"            ),
        valueNoNull(jsonObj, "user_id"       ),

        jsonObj.value("flags", -1)
    };
}

template <>
void unmarshalObj<ThreadMetadata>(json jsonObj, ThreadMetadata **object)
{
    *object = new ThreadMetadata {
        valueNoNull(jsonObj, "archive_timestamp"),

        jsonObj.value("auto_archive_duration", -1),

        jsonObj.value("archived", false),
        jsonObj.value("locked"  , false)
    };
}

template <>
void unmarshalObj<Channel>(json jsonObj, Channel **object)
{
    std::vector<User *> *recipients = new std::vector<User *>;
    std::vector<Overwrite *> *permissionOverwrites = new std::vector<Overwrite *>;
    ThreadMember *member = new ThreadMember;
    ThreadMetadata *threadMetadata = new ThreadMetadata;

    unmarshalMultiple<User>(jsonObj, "recipients", &recipients);
    unmarshalMultiple<Overwrite>(jsonObj, "permission_overwrites", &permissionOverwrites);
    unmarshal<ThreadMember>(jsonObj, "member", &member);
    unmarshal<ThreadMetadata>(jsonObj, "thread_metadata", &threadMetadata);

    *object = new Channel {
        recipients,
        permissionOverwrites,
        member,
        threadMetadata,
        valueNoNull(jsonObj, "id"                ),
        valueNoNull(jsonObj, "name"              ),
        valueNoNull(jsonObj, "topic"             ),
        valueNoNull(jsonObj, "icon"              ),
        valueNoNull(jsonObj, "last_pin_timestamp"),
        valueNoNull(jsonObj, "rtc_region"        ),
        valueNoNull(jsonObj, "permissions"       ),
        valueNoNull(jsonObj, "guild_id"          ),
        valueNoNull(jsonObj, "last_message_id"   ),
        valueNoNull(jsonObj, "owner_id"          ),
        valueNoNull(jsonObj, "application_id"    ),
        valueNoNull(jsonObj, "parent_id"         ),

        jsonObj.value("type"                         , -1),
        jsonObj.value("position"                     , -1),
        jsonObj.value("bitrate"                      , -1),
        jsonObj.value("user_limit"                   , -1),
        jsonObj.value("rate_limit_per_user"          , -1),
        jsonObj.value("video_quality_mode"           , -1),
        jsonObj.value("message_count"                , -1),
        jsonObj.value("member_count"                 , -1),
        jsonObj.value("default_auto_archive_duration", -1),

        jsonObj.value("nsfw", false)
    };
}

template <>
void unmarshalObj<TeamMember>(json jsonObj, TeamMember **object)
{
    User *user = new User;

    unmarshal<User>(jsonObj, "user", &user);

    *object = new TeamMember {
        user,
        getStringsFromJson(jsonObj, "permissions"),

        valueNoNull(jsonObj, "team_id"),

        jsonObj.value("member_ship_state", -1)
    };
}

template <>
void unmarshalObj<Team>(json jsonObj, Team **object)
{
    std::vector<TeamMember *> *members = new std::vector<TeamMember *>;

    unmarshalMultiple<TeamMember>(jsonObj, "members", &members);

    *object = new Team {
        members,
        valueNoNull(jsonObj, "icon"         ),
        valueNoNull(jsonObj, "id"           ),
        valueNoNull(jsonObj, "name"         ),
        valueNoNull(jsonObj, "owner_user_id")
    };
}

template <>
void unmarshalObj<Application>(json jsonObj, Application **object)
{
    User *owner = new User;
    Team *team = new Team;

    unmarshal<User>(jsonObj, "owner", &owner);
    unmarshal<Team>(jsonObj, "team", &team);

    *object = new Application {
        owner,
        team,
        getStringsFromJson(jsonObj, "rpc_origins"),

        valueNoNull(jsonObj, "id"                  ),
        valueNoNull(jsonObj, "name"                ),
        valueNoNull(jsonObj, "icon"                ),
        valueNoNull(jsonObj, "description"         ),
        valueNoNull(jsonObj, "terms_of_service_url"),
        valueNoNull(jsonObj, "privacy_policy_url"  ),
        valueNoNull(jsonObj, "summary"             ),
        valueNoNull(jsonObj, "verify_key"          ),
        valueNoNull(jsonObj, "guild_id"            ),
        valueNoNull(jsonObj, "primary_sku_id"      ),
        valueNoNull(jsonObj, "slug"                ),
        valueNoNull(jsonObj, "cover_image"         ),

        jsonObj.value("flags", -1),

        jsonObj.value("bot_public"            , false),
        jsonObj.value("bot_require_code_grant", false)
    };
}

template <>
void unmarshalObj<MessageActivity>(json jsonObj, MessageActivity **object)
{
    *object = new MessageActivity {
        valueNoNull(jsonObj, "party_id"),

        jsonObj.value("type", -1)
    };
}

template <>
void unmarshalObj<GuildMessageMember>(json jsonObj, GuildMessageMember **object)
{
    *object = new GuildMessageMember {
        getStringsFromJson(jsonObj, "roles"),

        valueNoNull(jsonObj, "nick"         ),
        valueNoNull(jsonObj, "joined_at"    ),
        valueNoNull(jsonObj, "premium_since"),
        valueNoNull(jsonObj, "permissions"  ),

        jsonObj.value("deaf"   , false),
        jsonObj.value("mute"   , false),
        jsonObj.value("pending", false)
    };
}

template <>
void unmarshalObj<MessageInteraction>(json jsonObj, MessageInteraction **object)
{
    User *user = new User;

    unmarshal<User>(jsonObj, "user", &user);

    *object = new MessageInteraction {
        user,

        valueNoNull(jsonObj, "id"  ),
        valueNoNull(jsonObj, "name"),

        jsonObj.value("type", -1)
    };
}

template <>
void unmarshalObj<Emoji>(json jsonObj, Emoji **object)
{
    User *user = new User;

    unmarshal<User>(jsonObj, "user", &user);

    *object = new Emoji {
        user,
        getStringsFromJson(jsonObj, "roles"),

        valueNoNull(jsonObj, "id"),
        valueNoNull(jsonObj, "name"),

        jsonObj.value("requireColons", false),
        jsonObj.value("managed"      , false),
        jsonObj.value("animated"     , false),
        jsonObj.value("available"    , false)
    };
}

template <>
void unmarshalObj<Reaction>(json jsonObj, Reaction **object)
{
    Emoji *emoji = new Emoji;

    unmarshal<Emoji>(jsonObj, "emoji", &emoji);

    *object = new Reaction {
        emoji,

        jsonObj.value("count", -1),

        jsonObj.value("me", false)
    };
}

template <>
void unmarshalObj<EmbedField>(json jsonObj, EmbedField **object)
{
    *object = new EmbedField {
        valueNoNull(jsonObj, "name"),
        valueNoNull(jsonObj, "value"),

        jsonObj.value("inline", false)
    };
}

template <>
void unmarshalObj<EmbedFooter>(json jsonObj, EmbedFooter **object)
{
    *object = new EmbedFooter {
        valueNoNull(jsonObj, "text"          ),
        valueNoNull(jsonObj, "icon_url"      ),
        valueNoNull(jsonObj, "proxy_icon_url")
    };
}

template <>
void unmarshalObj<EmbedTVI>(json jsonObj, EmbedTVI **object)
{
    *object = new EmbedTVI {
        valueNoNull(jsonObj, "url"      ),
        valueNoNull(jsonObj, "proxy_url"),

        jsonObj.value("height", -1),
        jsonObj.value("width" , -1)
    };
}

template <>
void unmarshalObj<EmbedProvider>(json jsonObj, EmbedProvider **object)
{
    *object = new EmbedProvider {
        valueNoNull(jsonObj, "name"),
        valueNoNull(jsonObj, "url" )
    };
}

template <>
void unmarshalObj<EmbedAuthor>(json jsonObj, EmbedAuthor **object)
{
    *object = new EmbedAuthor {
        valueNoNull(jsonObj, "name"          ),
        valueNoNull(jsonObj, "url"           ),
        valueNoNull(jsonObj, "icon_url"      ),
        valueNoNull(jsonObj, "proxy_icon_url")
    };
}

template <>
void unmarshalObj<Embed>(json jsonObj, Embed **object)
{
    std::vector<EmbedField *> *fields = new std::vector<EmbedField *>;
    EmbedFooter *footer = new EmbedFooter;
    EmbedTVI *image = new EmbedTVI;
    EmbedTVI *thumbnail = new EmbedTVI;
    EmbedTVI *video = new EmbedTVI;
    EmbedProvider *provider = new EmbedProvider;
    EmbedAuthor *author = new EmbedAuthor;

    unmarshalMultiple<EmbedField>(jsonObj, "fields", &fields);
    unmarshal<EmbedFooter>(jsonObj, "footer", &footer);
    unmarshal<EmbedTVI>(jsonObj, "image", &image);
    unmarshal<EmbedTVI>(jsonObj, "thumbnail", &thumbnail);
    unmarshal<EmbedTVI>(jsonObj, "video", &video);
    unmarshal<EmbedProvider>(jsonObj, "provider", &provider);
    unmarshal<EmbedAuthor>(jsonObj, "author", &author);

    *object = new Embed {
        fields,
        footer,
        image,
        thumbnail,
        video,
        provider,
        author,

        valueNoNull(jsonObj, "title"      ),
        valueNoNull(jsonObj, "type"       ),
        valueNoNull(jsonObj, "description"),
        valueNoNull(jsonObj, "url"        ),
        valueNoNull(jsonObj, "timestamp"  ),

        jsonObj.value("color", -1)
    };
}

template <>
void unmarshalObj<Attachment>(json jsonObj, Attachment **object)
{
    *object = new Attachment {
        valueNoNull(jsonObj, "id"          ),
        valueNoNull(jsonObj, "filename"    ),
        valueNoNull(jsonObj, "content_type"),
        valueNoNull(jsonObj, "url"         ),
        valueNoNull(jsonObj, "proxy_url"   ),

        jsonObj.value("size"  , -1),
        jsonObj.value("height", -1),
        jsonObj.value("width" , -1)
    };
}

template <>
void unmarshalObj<ChannelMention>(json jsonObj, ChannelMention **object)
{
    *object = new ChannelMention {
        valueNoNull(jsonObj, "id"      ),
        valueNoNull(jsonObj, "guild_id"),
        valueNoNull(jsonObj, "name"    ),

        jsonObj.value("type", -1)
    };
}

template <>
void unmarshalObj<SelectOption>(json jsonObj, SelectOption **object)
{
    Emoji *emoji = new Emoji;

    unmarshal<Emoji>(jsonObj, "emoji", &emoji);

    *object = new SelectOption {
        emoji,

        valueNoNull(jsonObj, "label"      ),
        valueNoNull(jsonObj, "value"      ),
        valueNoNull(jsonObj, "description"),

        jsonObj.value("default", false)
    };
}

template <>
void unmarshalObj<MessageComponent>(json jsonObj, MessageComponent **object)
{
    Emoji *emoji = new Emoji;
    std::vector<SelectOption *> *components = new std::vector<SelectOption *>;

    unmarshal<Emoji>(jsonObj, "emoji", &emoji);
    unmarshalMultiple<SelectOption>(jsonObj, "components", &components);

    *object = new MessageComponent {
        emoji,
        components,
        nullptr,

        valueNoNull(jsonObj, "custom_id"  ),
        valueNoNull(jsonObj, "label"      ),
        valueNoNull(jsonObj, "url"        ),
        valueNoNull(jsonObj, "placeholder"),

        jsonObj.value("type"      , -1),
        jsonObj.value("style"     , -1),
        jsonObj.value("min_values", -1),
        jsonObj.value("max_values", -1),

        jsonObj.value("disabled", false)
    };
}

template <>
void unmarshalObj<StickerItem>(json jsonObj, StickerItem **object)
{
    *object = new StickerItem {
        valueNoNull(jsonObj, "id"  ),
        valueNoNull(jsonObj, "name"),

        jsonObj.value("format_type", -1)
    };
}

template <>
void unmarshalObj<Sticker>(json jsonObj, Sticker **object)
{
    User *user = new User;

    unmarshal<User>(jsonObj, "user", &user),

    *object = new Sticker {
        user,

        valueNoNull(jsonObj, "id"         ),
        valueNoNull(jsonObj, "pack_id"    ),
        valueNoNull(jsonObj, "name"       ),
        valueNoNull(jsonObj, "description"),
        valueNoNull(jsonObj, "tags"       ),
        valueNoNull(jsonObj, "asset"      ),
        valueNoNull(jsonObj, "guild_id"   ),

        jsonObj.value("type"       , -1),
        jsonObj.value("format_type", -1),
        jsonObj.value("sort_value" , -1),

        jsonObj.value("available", false)
    };
}

Message *getPartialMessage(json jsonObj, const std::string& key)
{
    Application *application = new Application;
    User *author = new User;
    MessageActivity *activity = new MessageActivity;
    GuildMessageMember *member = new GuildMessageMember;
    Channel *thread = new Channel;
    MessageInteraction *interaction = new MessageInteraction;
    std::vector<Reaction *> *reactions = new std::vector<Reaction *>;
    std::vector<User *> *mentions = new std::vector<User *>;
    std::vector<Attachment *> *attachments = new std::vector<Attachment *>;
    std::vector<ChannelMention *> *mentionChannels = new std::vector<ChannelMention *>;
    std::vector<MessageComponent *> *components = new std::vector<MessageComponent *>;
    std::vector<StickerItem *> *stickerItems = new std::vector<StickerItem *>;
    std::vector<Sticker *> *stickers = new std::vector<Sticker *>;

    unmarshal<Application>(jsonObj, "application", &application);
    unmarshal<User>(jsonObj, "author", &author);
    unmarshal<MessageActivity>(jsonObj, "activity", &activity);
    unmarshal<GuildMessageMember>(jsonObj, "member", &member);
    unmarshal<Channel>(jsonObj, "thread", &thread);
    unmarshal<MessageInteraction>(jsonObj, "interaction", &interaction);
    unmarshalMultiple<Reaction>(jsonObj, "reactions", &reactions);
    unmarshalMultiple<User>(jsonObj, "mentions", &mentions);
    unmarshalMultiple<Attachment>(jsonObj, "attachments", &attachments);
    unmarshalMultiple<ChannelMention>(jsonObj, "mention_channels", &mentionChannels);
    unmarshalMultiple<MessageComponent>(jsonObj, "components", &components);
    unmarshalMultiple<StickerItem>(jsonObj, "sticker_items", &stickerItems);
    unmarshalMultiple<Sticker>(jsonObj, "stickers", &stickers);

    try {
        jsonObj = key == static_cast<std::string>("") ? jsonObj : jsonObj.at(key);

        return new Message {
            application,
            author,
            activity,
            member,
            nullptr,
            thread,
            interaction,

            reactions,
            nullptr,
            mentions,
            attachments,
            mentionChannels,
            getStringsFromJson(jsonObj, "mention_roles"),
            components,
            stickerItems,
            stickers,

            valueNoNull(jsonObj, "id"              ),
            valueNoNull(jsonObj, "channel_id"      ),
            valueNoNull(jsonObj, "guild_id"        ),
            valueNoNull(jsonObj, "content"         ),
            valueNoNull(jsonObj, "timestamp"       ),
            valueNoNull(jsonObj, "edited_timestamp"),
            valueNoNull(jsonObj, "webhook_id"      ),
            valueNoNull(jsonObj, "application_id"  ),
            valueNoNull(jsonObj, "nonce"           ),

            jsonObj.value("nonce"              , -1),
            jsonObj.value("author_public_flags", -1),
            jsonObj.value("type"               , -1),
            jsonObj.value("flags"              , -1),

            jsonObj.value("tts"             , false),
            jsonObj.value("pinned"          , false),
            jsonObj.value("mention_everyone", false)
        };
    } catch(json::out_of_range& e) {
        return nullptr;
    }
}

template <>
void unmarshalObj<Message>(json jsonObj, Message **object)
{
    Application *application = new Application;
    User *author = new User;
    MessageActivity *activity = new MessageActivity;
    GuildMessageMember *member = new GuildMessageMember;
    Channel *thread = new Channel;
    MessageInteraction *interaction = new MessageInteraction;
    std::vector<Reaction *> *reactions = new std::vector<Reaction *>;
    std::vector<User *> *mentions = new std::vector<User *>;
    std::vector<Attachment *> *attachments = new std::vector<Attachment *>;
    std::vector<ChannelMention *> *mentionChannels = new std::vector<ChannelMention *>;
    std::vector<MessageComponent *> *components = new std::vector<MessageComponent *>;
    std::vector<StickerItem *> *stickerItems = new std::vector<StickerItem *>;
    std::vector<Sticker *> *stickers = new std::vector<Sticker *>;

    unmarshal<Application>(jsonObj, "application", &application);
    unmarshal<User>(jsonObj, "author", &author);
    unmarshal<MessageActivity>(jsonObj, "activity", &activity);
    unmarshal<GuildMessageMember>(jsonObj, "member", &member);
    unmarshal<Channel>(jsonObj, "thread", &thread);
    unmarshal<MessageInteraction>(jsonObj, "interaction", &interaction);
    unmarshalMultiple<Reaction>(jsonObj, "reactions", &reactions);
    unmarshalMultiple<User>(jsonObj, "mentions", &mentions);
    unmarshalMultiple<Attachment>(jsonObj, "attachments", &attachments);
    unmarshalMultiple<ChannelMention>(jsonObj, "mention_channels", &mentionChannels);
    unmarshalMultiple<MessageComponent>(jsonObj, "components", &components);
    unmarshalMultiple<StickerItem>(jsonObj, "sticker_items", &stickerItems);
    unmarshalMultiple<Sticker>(jsonObj, "stickers", &stickers);

    *object = new Message {
        application,
        author,
        activity,
        member,
        getPartialMessage(jsonObj, "referenced_message"),
        thread,
        interaction,

        reactions,
        nullptr,
        mentions,
        attachments,
        mentionChannels,
        getStringsFromJson(jsonObj, "mention_roles"),
        components,
        stickerItems,
        stickers,

        valueNoNull(jsonObj, "id"              ),
        valueNoNull(jsonObj, "channel_id"      ),
        valueNoNull(jsonObj, "guild_id"        ),
        valueNoNull(jsonObj, "content"         ),
        valueNoNull(jsonObj, "timestamp"       ),
        valueNoNull(jsonObj, "edited_timestamp"),
        valueNoNull(jsonObj, "webhook_id"      ),
        valueNoNull(jsonObj, "application_id"  ),
        valueNoNull(jsonObj, "nonce"           ),

        jsonObj.value("nonce"              , -1),
        jsonObj.value("author_public_flags", -1),
        jsonObj.value("type"               , -1),
        jsonObj.value("flags"              , -1),

        jsonObj.value("tts"             , false),
        jsonObj.value("pinned"          , false),
        jsonObj.value("mention_everyone", false)
    };
}

template <>
void unmarshalObj<GuildMember>(json jsonObj, GuildMember **object)
{
    User *user = new User;

    unmarshal<User>(jsonObj, "user", &user);

    *object = new GuildMember {
        user,
        getStringsFromJson(jsonObj, "roles"),

        valueNoNull(jsonObj, "nick"         ),
        valueNoNull(jsonObj, "avatar"       ),
        valueNoNull(jsonObj, "joined_at"    ),
        valueNoNull(jsonObj, "premium_since"),
        valueNoNull(jsonObj, "permissions"  ),

        jsonObj.value("deaf"   , false),
        jsonObj.value("mute"   , false),
        jsonObj.value("pending", false)
    };
}

template <>
void unmarshalObj<VoiceState>(json jsonObj, VoiceState **object)
{
    GuildMember *member = new GuildMember;

    unmarshal<GuildMember>(jsonObj, "member", &member);

    *object = new VoiceState {
        member,

        valueNoNull(jsonObj, "guild_id"                  ),
        valueNoNull(jsonObj, "channel_id"                ),
        valueNoNull(jsonObj, "user_id"                   ),
        valueNoNull(jsonObj, "session_id"                ),
        valueNoNull(jsonObj, "request_to_speak_timestamp"),

        jsonObj.value("deaf"       , false),
        jsonObj.value("mute"       , false),
        jsonObj.value("self_deaf"  , false),
        jsonObj.value("self_mute"  , false),
        jsonObj.value("self_stream", false),
        jsonObj.value("self_video" , false),
        jsonObj.value("suppress"   , false)
    };
}

template <>
void unmarshalObj<WelcomeScreenChannel>(json jsonObj, WelcomeScreenChannel **object)
{
    *object = new WelcomeScreenChannel {
        valueNoNull(jsonObj, "channel_id" ),
        valueNoNull(jsonObj, "description"),
        valueNoNull(jsonObj, "emoji_id"   ),
        valueNoNull(jsonObj, "emoji_name" )
    };
}

template <>
void unmarshalObj<WelcomeScreen>(json jsonObj, WelcomeScreen **object)
{
    std::vector<WelcomeScreenChannel *> *welcomeChannels = new std::vector<WelcomeScreenChannel *>;

    unmarshalMultiple<WelcomeScreenChannel>(jsonObj, "welcome_channels", &welcomeChannels);

    *object = new WelcomeScreen {
        welcomeChannels,

        valueNoNull(jsonObj, "description")
    };
}

template <>
void unmarshalObj<StageInstance>(json jsonObj, StageInstance **object)
{
    *object = new StageInstance {
        valueNoNull(jsonObj, "id"        ),
        valueNoNull(jsonObj, "guild_id"  ),
        valueNoNull(jsonObj, "channel_id"),
        valueNoNull(jsonObj, "topic"     ),

        jsonObj.value("privacy_level" , -1),

        jsonObj.value("discoverable_disabled" , false)
    };
}

template <>
void unmarshalObj<Guild>(json jsonObj, Guild **object)
{
    WelcomeScreen *welcomeScreen = new WelcomeScreen;
    std::vector<VoiceState *> *voiceStates = new std::vector<VoiceState *>;
    std::vector<GuildMember *> *members = new std::vector<GuildMember *>;
    std::vector<Channel *> *channels = new std::vector<Channel *>;
    std::vector<Channel *> *threads = new std::vector<Channel *>;
    std::vector<StageInstance *> *stageInstances = new std::vector<StageInstance *>;
    std::vector<Sticker *> *stickers = new std::vector<Sticker *>;

    unmarshal<WelcomeScreen>(jsonObj, "welcome_screen", &welcomeScreen);
    unmarshalMultiple<VoiceState>(jsonObj, "voice_states", &voiceStates);
    unmarshalMultiple<GuildMember>(jsonObj, "members", &members);
    unmarshalMultiple<Channel>(jsonObj, "channels", &channels);
    unmarshalMultiple<Channel>(jsonObj, "threads", &threads);
    unmarshalMultiple<StageInstance>(jsonObj, "stage_instances", &stageInstances);
    unmarshalMultiple<Sticker>(jsonObj, "stickers", &stickers);

    *object = new Guild {
        welcomeScreen,
        getStringsFromJson(jsonObj, "guild_features"),
        voiceStates,
        members,
        channels,
        threads,
        nullptr,
        stageInstances,
        stickers,

        valueNoNull(jsonObj, "id"                       ),
        valueNoNull(jsonObj, "name"                     ),
        valueNoNull(jsonObj, "icon"                     ),
        valueNoNull(jsonObj, "icon_hash"                ),
        valueNoNull(jsonObj, "splash"                   ),
        valueNoNull(jsonObj, "discovery_splash"         ),
        valueNoNull(jsonObj, "owner_id"                 ),
        valueNoNull(jsonObj, "permissions"              ),
        valueNoNull(jsonObj, "region"                   ),
        valueNoNull(jsonObj, "afk_channel_id"           ),
        valueNoNull(jsonObj, "widget_channel_id"        ),
        valueNoNull(jsonObj, "application_id"           ),
        valueNoNull(jsonObj, "system_channel_id"        ),
        valueNoNull(jsonObj, "rules_channel_id"         ),
        valueNoNull(jsonObj, "joined_at"                ),
        valueNoNull(jsonObj, "vanity_url_code"          ),
        valueNoNull(jsonObj, "description"              ),
        valueNoNull(jsonObj, "banner"                   ),
        valueNoNull(jsonObj, "preferred_locale"         ),
        valueNoNull(jsonObj, "public_updates_channel_id"),

        jsonObj.value("afk_timeout"                  , -1),
        jsonObj.value("verification_level"           , -1),
        jsonObj.value("default_message_notifications", -1),
        jsonObj.value("explicit_content_filter"      , -1),
        jsonObj.value("mfa_level"                    , -1),
        jsonObj.value("system_channel_flags"         , -1),
        jsonObj.value("member_count"                 , -1),
        jsonObj.value("max_presences"                , -1),
        jsonObj.value("max_members"                  , -1),
        jsonObj.value("premium_tier"                 , -1),
        jsonObj.value("premium_subscription_count"   , -1),
        jsonObj.value("max_video_channel_users"      , -1),
        jsonObj.value("approximate_member_count"     , -1),
        jsonObj.value("approximate_presence_count"   , -1),
        jsonObj.value("nsfw_level"                   , -1),

        jsonObj.value("owner"         , false),
        jsonObj.value("widget_enabled", false),
        jsonObj.value("large"         , false),
        jsonObj.value("unavailable"   , false)
    };
}

template <>
void unmarshalObj<CustomStatus>(json jsonObj, CustomStatus **object)
{
    *object = new CustomStatus {
        valueNoNull(jsonObj, "text"     ),
        valueNoNull(jsonObj, "expiresAt"),
        valueNoNull(jsonObj, "emojiName"),
        valueNoNull(jsonObj, "emojiId"  )
    };
}

template <>
void unmarshalObj<FriendSourceFlags>(json jsonObj, FriendSourceFlags **object)
{
    *object = new FriendSourceFlags {
        jsonObj.value("all"           , false),
        jsonObj.value("mutual_friends", false),
        jsonObj.value("mutual_guilds" , false)
    };
}

template <>
void unmarshalObj<GuildFolder>(json jsonObj, GuildFolder **object)
{
    *object = new GuildFolder {
        getStringsFromJson(jsonObj, "guild_ids"),

        valueNoNull(jsonObj, "name"),

        jsonObj.at("id").is_null() ? -1 : jsonObj.value("id" , -1),
        //jsonObj.value("color" , -1)
        -1
    };
}

template <>
void unmarshalObj<ClientSettings>(json jsonObj, ClientSettings **object)
{
    CustomStatus *customStatus = new CustomStatus;
    FriendSourceFlags *friendSourceFlags = new FriendSourceFlags;
    std::vector<GuildFolder *> *guildFolders = new std::vector<GuildFolder *>;

    unmarshal<CustomStatus>(jsonObj, "custom_status", &customStatus);
    unmarshal<FriendSourceFlags>(jsonObj, "friend_source_flags", &friendSourceFlags);
    unmarshalMultiple<GuildFolder>(jsonObj, "guild_folders", &guildFolders);

    *object = new ClientSettings {
        customStatus,
        friendSourceFlags,
        guildFolders,
        getStringsFromJson(jsonObj, "guild_positions"),
        getStringsFromJson(jsonObj, "restricted_guilds"),

        valueNoNull(jsonObj, "locale"),
        valueNoNull(jsonObj, "status"),
        valueNoNull(jsonObj, "theme"),

        jsonObj.value("afk_timeout"            , -1),
        jsonObj.value("animate_stickers"       , -1),
        jsonObj.value("explicit_content_filter", -1),
        jsonObj.value("friend_discovery_flags" , -1),
        jsonObj.value("timezone_offset"        , -1),

        jsonObj.value("allow_accessibility_detection"   , false),
        jsonObj.value("animate_emoji"                   , false),
        jsonObj.value("contact_sync_enabled"            , false),
        jsonObj.value("convert_emoticons"               , false),
        jsonObj.value("default_guilds_restricted"       , false),
        jsonObj.value("detect_platform_accounts"        , false),
        jsonObj.value("developer_mode"                  , false),
        jsonObj.value("disable_games_tab"               , false),
        jsonObj.value("enable_tts_command"              , false),
        jsonObj.value("gif_auto_play"                   , false),
        jsonObj.value("inline_attachment_media"         , false),
        jsonObj.value("inline_embed_media"              , false),
        jsonObj.value("message_display_compact"         , false),
        jsonObj.value("native_phone_integration_enabled", false),
        jsonObj.value("render_embeds"                   , false),
        jsonObj.value("render_reactions"                , false),
        jsonObj.value("show_current_game"               , false),
        jsonObj.value("stream_notifications_enabled"    , false),
        jsonObj.value("view_nsfw_guilds"                , false)
    };
}

template <>
void unmarshalObj<Client>(json jsonObj, Client **object)
{
    *object = new Client {
        valueNoNull(jsonObj, "id"           ),
        valueNoNull(jsonObj, "username"     ),
        valueNoNull(jsonObj, "avatar"       ),
        valueNoNull(jsonObj, "discriminator"),
        valueNoNull(jsonObj, "banner"       ),
        valueNoNull(jsonObj, "bio"          ),
        valueNoNull(jsonObj, "locale"       ),
        valueNoNull(jsonObj, "email"        ),
        valueNoNull(jsonObj, "phone"        ),

        jsonObj.value("public_flags"   , -1),
        jsonObj.value("flags"          , -1),
        jsonObj.value("purchased_flags", -1),
        //jsonObj.value("banner_color"   , -1),
        //jsonObj.value("accent_color"   , -1),
        -1,
        -1,

        jsonObj.value("nsfw_allowed", false),
        jsonObj.value("mfa_enabled" , false),
        jsonObj.value("verified"    , false)
    };
}

} // namespace Api
