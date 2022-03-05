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
#include "api/presence.h"

#include <string>

#include <QJsonObject>

namespace Api {

std::string *getString(QJsonObject jsonObj, const char *key)
{
    std::string str = jsonObj[QString(key)].toString().toUtf8().constData();
    return new std::string(str);
}

std::vector<std::string> *getStringsFromJson(QJsonArray jsonArray)
{
    std::vector<std::string> *strings = new std::vector<std::string>;

    // Filling the vector
    for (int i = 0 ; i < jsonArray.size() ; i++) {
        strings->push_back(jsonArray[i].toString().toUtf8().constData());
    }

    return strings;
}

// All the specialization of 'unmarshal'

template <>
void unmarshal<User>(QJsonObject jsonObj, User **object)
{
    *object = new User {
        getString(jsonObj, "username"),
        getString(jsonObj, "discriminator"),
        getString(jsonObj, "avatar"),
        getString(jsonObj, "locale"),
        getString(jsonObj, "email"),
        getString(jsonObj, "id"),

        jsonObj["flags"].toInt(-1),
        jsonObj["premium_type"].toInt(-1),
        jsonObj["public_flags"].toInt(-1),

        jsonObj["bot"].toBool(),
        jsonObj["system"].toBool(),
        jsonObj["mfa_enabled"].toBool(),
        jsonObj["verified"].toBool()
    };
}

template <>
void unmarshal<Overwrite>(QJsonObject jsonObj, Overwrite **object)
{
    *object = new Overwrite {
        getString(jsonObj, "id"),
        getString(jsonObj, "allow"),
        getString(jsonObj, "deny"),

        jsonObj["type"].toInt(-1)
    };
}

template <>
void unmarshal<ThreadMember>(QJsonObject jsonObj, ThreadMember **object)
{
    *object = new ThreadMember {
        getString(jsonObj, "join_timestamp"),
        getString(jsonObj, "id"),
        getString(jsonObj, "user_id"),

        jsonObj["flags"].toInt(-1)
    };
}

template <>
void unmarshal<ThreadMetadata>(QJsonObject jsonObj, ThreadMetadata **object)
{
    *object = new ThreadMetadata {
        getString(jsonObj, "archive_timestamp"),

        jsonObj["auto_archive_duration"].toInt(-1),

        jsonObj["archived"].toBool(),
        jsonObj["locked"].toBool()
    };
}

template <>
void unmarshal<Channel>(QJsonObject jsonObj, Channel **object)
{
    std::vector<User *> *recipients = new std::vector<User *>;
    std::vector<Overwrite *> *permissionOverwrites = new std::vector<Overwrite *>;
    ThreadMember *member = new ThreadMember;
    ThreadMetadata *threadMetadata = new ThreadMetadata;

    unmarshalMultiple<User>(jsonObj["recipients"].toArray(), &recipients);
    unmarshalMultiple<Overwrite>(jsonObj["permission_overwrites"].toArray(), &permissionOverwrites);
    unmarshal<ThreadMember>(jsonObj["member"].toObject(), &member);
    unmarshal<ThreadMetadata>(jsonObj["thread_metadata"].toObject(), &threadMetadata);

    *object = new Channel {
        recipients,
        permissionOverwrites,
        member,
        threadMetadata,

        getString(jsonObj, "id"),
        getString(jsonObj, "name"),
        getString(jsonObj, "topic"),
        getString(jsonObj, "icon"),
        getString(jsonObj, "last_pin_timestamp"),
        getString(jsonObj, "rtc_region"),
        getString(jsonObj, "permissions"),
        getString(jsonObj, "guild_id"),
        getString(jsonObj, "last_message_id"),
        getString(jsonObj, "owner_id"),
        getString(jsonObj, "application_id"),
        getString(jsonObj, "parent_id"),

        jsonObj["type"].toInt(-1),
        jsonObj["position"].toInt(-1),
        jsonObj["bitrate"].toInt(-1),
        jsonObj["user_limit"].toInt(-1),
        jsonObj["rate_limit_per_user"].toInt(-1),
        jsonObj["video_quality_mode"].toInt(-1),
        jsonObj["message_count"].toInt(-1),
        jsonObj["member_count"].toInt(-1),
        jsonObj["default_auto_archive_duration"].toInt(-1),

        jsonObj["nsfw"].toBool()
    };
}

template <>
void unmarshal<PrivateChannel>(QJsonObject jsonObj, PrivateChannel **object)
{
    *object = new PrivateChannel {
        getStringsFromJson(jsonObj["recipient_ids"].toArray()),

        getString(jsonObj, "icon"),
        getString(jsonObj, "id"),
        getString(jsonObj, "last_message_id"),
        getString(jsonObj, "name"),
        getString(jsonObj, "owner_id"),

        jsonObj["type"].toInt(-1)
    };
}

template <>
void unmarshal<TeamMember>(QJsonObject jsonObj, TeamMember **object)
{
    User *user = new User;

    unmarshal<User>(jsonObj, "user", &user);

    *object = new TeamMember {
        user,
        getStringsFromJson(jsonObj["permissions"].toArray()),

        getString(jsonObj, "team_id"),

        jsonObj["member_ship_state"].toInt(-1)
    };
}

template <>
void unmarshal<Team>(QJsonObject jsonObj, Team **object)
{
    std::vector<TeamMember *> *members = new std::vector<TeamMember *>;

    unmarshalMultiple<TeamMember>(jsonObj, "members", &members);

    *object = new Team {
        members,

        getString(jsonObj, "icon"),
        getString(jsonObj, "id"),
        getString(jsonObj, "name"),
        getString(jsonObj, "owner_user_id")
    };
}

template <>
void unmarshal<Application>(QJsonObject jsonObj, Application **object)
{
    User *owner = new User;
    Team *team = new Team;

    unmarshal<User>(jsonObj, "owner", &owner);
    unmarshal<Team>(jsonObj, "team", &team);

    *object = new Application {
        owner,
        team,
        getStringsFromJson(jsonObj["rpc_origins"].toArray()),

        getString(jsonObj, "id"),
        getString(jsonObj, "name"),
        getString(jsonObj, "icon"),
        getString(jsonObj, "description"),
        getString(jsonObj, "terms_of_service_url"),
        getString(jsonObj, "privacy_policy_url"),
        getString(jsonObj, "summary"),
        getString(jsonObj, "verify_key"),
        getString(jsonObj, "guild_id"),
        getString(jsonObj, "primary_sku_id"),
        getString(jsonObj, "slug"),
        getString(jsonObj, "cover_image"),

        jsonObj["flags"].toInt(-1),

        jsonObj["bot_public"].toBool(),
        jsonObj["bot_require_code_grant"].toBool()
    };
}

template <>
void unmarshal<MessageActivity>(QJsonObject jsonObj, MessageActivity **object)
{
    *object = new MessageActivity {
        getString(jsonObj, "party_id"),

        jsonObj["type"].toInt(-1)
    };
}

template <>
void unmarshal<GuildMessageMember>(QJsonObject jsonObj, GuildMessageMember **object)
{
    *object = new GuildMessageMember {
        getStringsFromJson(jsonObj["roles"].toArray()),

        getString(jsonObj, "nick"),
        getString(jsonObj, "joined_at"),
        getString(jsonObj, "premium_since"),
        getString(jsonObj, "permissions"),

        jsonObj["deaf"].toBool(),
        jsonObj["mute"].toBool(),
        jsonObj["pending"].toBool()
    };
}

template <>
void unmarshal<MessageInteraction>(QJsonObject jsonObj, MessageInteraction **object)
{
    User *user = new User;

    unmarshal<User>(jsonObj, "user", &user);

    *object = new MessageInteraction {
        user,

        getString(jsonObj, "id"),
        getString(jsonObj, "name"),

        jsonObj["type"].toInt(-1)
    };
}

template <>
void unmarshal<Emoji>(QJsonObject jsonObj, Emoji **object)
{
    User *user = new User;

    unmarshal<User>(jsonObj, "user", &user);

    *object = new Emoji {
        user,
        getStringsFromJson(jsonObj["roles"].toArray()),

        getString(jsonObj, "id"),
        getString(jsonObj, "name"),

        jsonObj["requireColons"].toBool(),
        jsonObj["managed"].toBool(),
        jsonObj["animated"].toBool(),
        jsonObj["available"].toBool()
    };
}

template <>
void unmarshal<Reaction>(QJsonObject jsonObj, Reaction **object)
{
    Emoji *emoji = new Emoji;

    unmarshal<Emoji>(jsonObj, "emoji", &emoji);

    *object = new Reaction {
        emoji,

        jsonObj["count"].toInt(-1),

        jsonObj["me"].toBool()
    };
}

template <>
void unmarshal<EmbedField>(QJsonObject jsonObj, EmbedField **object)
{
    *object = new EmbedField {
        getString(jsonObj, "name"),
        getString(jsonObj, "value"),

        jsonObj["inline"].toBool()
    };
}

template <>
void unmarshal<EmbedFooter>(QJsonObject jsonObj, EmbedFooter **object)
{
    *object = new EmbedFooter {
        getString(jsonObj, "text"),
        getString(jsonObj, "icon_url"),
        getString(jsonObj, "proxy_icon_url"),
    };
}

template <>
void unmarshal<EmbedTVI>(QJsonObject jsonObj, EmbedTVI **object)
{
    *object = new EmbedTVI {
        getString(jsonObj, "url"),
        getString(jsonObj, "proxy_url"),

        jsonObj["height"].toInt(-1),
        jsonObj["width"].toInt(-1)
    };
}

template <>
void unmarshal<EmbedProvider>(QJsonObject jsonObj, EmbedProvider **object)
{
    *object = new EmbedProvider {
        getString(jsonObj, "name"),
        getString(jsonObj, "url"),
    };
}

template <>
void unmarshal<EmbedAuthor>(QJsonObject jsonObj, EmbedAuthor **object)
{
    *object = new EmbedAuthor {
        getString(jsonObj, "name"),
        getString(jsonObj, "url"),
        getString(jsonObj, "icon_url"),
        getString(jsonObj, "proxy_icon_url")
    };
}

template <>
void unmarshal<Embed>(QJsonObject jsonObj, Embed **object)
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

        getString(jsonObj, "title"),
        getString(jsonObj, "type"),
        getString(jsonObj, "description"),
        getString(jsonObj, "url"),
        getString(jsonObj, "timestamp"),

        jsonObj["color"].toInt(-1)
    };
}

template <>
void unmarshal<Attachment>(QJsonObject jsonObj, Attachment **object)
{
    *object = new Attachment {
        getString(jsonObj, "id"),
        getString(jsonObj, "filename"),
        getString(jsonObj, "content_type"),
        getString(jsonObj, "url"),
        getString(jsonObj, "proxy_url"),

        jsonObj["size"].toInt(-1),
        jsonObj["height"].toInt(-1),
        jsonObj["width"].toInt(-1)
    };
}

template <>
void unmarshal<ChannelMention>(QJsonObject jsonObj, ChannelMention **object)
{
    *object = new ChannelMention {
        getString(jsonObj, "id"),
        getString(jsonObj, "guild_id"),
        getString(jsonObj, "name"),

        jsonObj["type"].toInt(-1)
    };
}

template <>
void unmarshal<SelectOption>(QJsonObject jsonObj, SelectOption **object)
{
    Emoji *emoji = new Emoji;

    unmarshal<Emoji>(jsonObj, "emoji", &emoji);

    *object = new SelectOption {
        emoji,

        getString(jsonObj, "label"),
        getString(jsonObj, "value"),
        getString(jsonObj, "description"),

        jsonObj["default"].toBool()
    };
}

template <>
void unmarshal<MessageComponent>(QJsonObject jsonObj, MessageComponent **object)
{
    Emoji *emoji = new Emoji;
    std::vector<SelectOption *> *components = new std::vector<SelectOption *>;

    unmarshal<Emoji>(jsonObj, "emoji", &emoji);
    unmarshalMultiple<SelectOption>(jsonObj, "components", &components);

    *object = new MessageComponent {
        emoji,
        components,
        nullptr,

        getString(jsonObj, "custom_id"),
        getString(jsonObj, "label"),
        getString(jsonObj, "url"),
        getString(jsonObj, "placeholder"),

        jsonObj["type"].toInt(-1),
        jsonObj["style"].toInt(-1),
        jsonObj["min_values"].toInt(-1),
        jsonObj["max_values"].toInt(-1),

        jsonObj["disabled"].toBool()
    };
}

template <>
void unmarshal<StickerItem>(QJsonObject jsonObj, StickerItem **object)
{
    *object = new StickerItem {
        getString(jsonObj, "id"),
        getString(jsonObj, "name"),

        jsonObj["format_type"].toInt(-1)
    };
}

template <>
void unmarshal<Sticker>(QJsonObject jsonObj, Sticker **object)
{
    User *user = new User;

    unmarshal<User>(jsonObj, "user", &user);

    *object = new Sticker {
        user,

        getString(jsonObj, "id"),
        getString(jsonObj, "pack_id"),
        getString(jsonObj, "name"),
        getString(jsonObj, "description"),
        getString(jsonObj, "tags"),
        getString(jsonObj, "asset"),
        getString(jsonObj, "guild_id"),

        jsonObj["type"].toInt(-1),
        jsonObj["format_type"].toInt(-1),
        jsonObj["sort_value"].toInt(-1),

        jsonObj["available"].toBool()
    };
}

Message *getPartialMessage(QJsonObject jsonObj, const QString& key)
{
    if (key == QString("") && jsonObj[key].type() == QJsonValue::Undefined) {
        return nullptr;
    }
    jsonObj = key == QString("") ? jsonObj : jsonObj[key].toObject();

    Application *application = new Application;
    User *author = new User;
    MessageActivity *activity = new MessageActivity;
    GuildMessageMember *member = new GuildMessageMember;
    Channel *thread = new Channel;
    MessageInteraction *interaction = new MessageInteraction;
    Call *call = new Call;
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
    unmarshal<Call>(jsonObj, "call", &call);
    unmarshalMultiple<Reaction>(jsonObj, "reactions", &reactions);
    unmarshalMultiple<User>(jsonObj, "mentions", &mentions);
    unmarshalMultiple<Attachment>(jsonObj, "attachments", &attachments);
    unmarshalMultiple<ChannelMention>(jsonObj, "mention_channels", &mentionChannels);
    unmarshalMultiple<MessageComponent>(jsonObj, "components", &components);
    unmarshalMultiple<StickerItem>(jsonObj, "sticker_items", &stickerItems);
    unmarshalMultiple<Sticker>(jsonObj, "stickers", &stickers);

    return new Message {
        application,
        author,
        activity,
        member,
        nullptr,
        thread,
        interaction,
        call,

        reactions,
        nullptr,
        mentions,
        attachments,
        mentionChannels,
        getStringsFromJson(jsonObj["mention_roles"].toArray()),
        components,
        stickerItems,
        stickers,

        getString(jsonObj, "id"),
        getString(jsonObj, "channel_id"),
        getString(jsonObj, "guild_id"),
        getString(jsonObj, "content"),
        getString(jsonObj, "timestamp"),
        getString(jsonObj, "edited_timestamp"),
        getString(jsonObj, "webhook_id"),
        getString(jsonObj, "application_id"),
        getString(jsonObj, "nonce"),

        jsonObj["nonce"].toInt(-1),
        jsonObj["author_public_flags"].toInt(-1),
        jsonObj["type"].toInt(-1),
        jsonObj["flags"].toInt(-1),

        jsonObj["tts"].toBool(),
        jsonObj["pinned"].toBool(),
        jsonObj["mention_everyone"].toBool()
    };
}

template <>
void unmarshal<Message>(QJsonObject jsonObj, Message **object)
{
    Application *application = new Application;
    User *author = new User;
    MessageActivity *activity = new MessageActivity;
    GuildMessageMember *member = new GuildMessageMember;
    Channel *thread = new Channel;
    MessageInteraction *interaction = new MessageInteraction;
    Call *call = new Call;
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
    unmarshal<Call>(jsonObj, "call", &call);
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
        call,

        reactions,
        nullptr,
        mentions,
        attachments,
        mentionChannels,
        getStringsFromJson(jsonObj["mention_roles"].toArray()),
        components,
        stickerItems,
        stickers,

        getString(jsonObj, "id"),
        getString(jsonObj, "channel_id"),
        getString(jsonObj, "guild_id"),
        getString(jsonObj, "content"),
        getString(jsonObj, "timestamp"),
        getString(jsonObj, "edited_timestamp"),
        getString(jsonObj, "webhook_id"),
        getString(jsonObj, "application_id"),
        getString(jsonObj, "nonce"),

        jsonObj["nonce"].toInt(-1),
        jsonObj["author_public_flags"].toInt(-1),
        jsonObj["type"].toInt(-1),
        jsonObj["flags"].toInt(-1),

        jsonObj["tts"].toBool(),
        jsonObj["pinned"].toBool(),
        jsonObj["mention_everyone"].toBool()
    };
}

template <>
void unmarshal<GuildMember>(QJsonObject jsonObj, GuildMember **object)
{
    User *user = new User;

    unmarshal<User>(jsonObj, "user", &user);

    *object = new GuildMember {
        user,
        getStringsFromJson(jsonObj["roles"].toArray()),

        getString(jsonObj, "nick"),
        getString(jsonObj, "avatar"),
        getString(jsonObj, "joined_at"),
        getString(jsonObj, "premium_since"),
        getString(jsonObj, "permissions"),

        jsonObj["deaf"].toBool(),
        jsonObj["mute"].toBool(),
        jsonObj["pending"].toBool()
    };
}

template <>
void unmarshal<VoiceState>(QJsonObject jsonObj, VoiceState **object)
{
    GuildMember *member = new GuildMember;

    unmarshal<GuildMember>(jsonObj, "member", &member);

    *object = new VoiceState {
        member,

        getString(jsonObj, "guild_id"),
        getString(jsonObj, "channel_id"),
        getString(jsonObj, "user_id"),
        getString(jsonObj, "session_id"),
        getString(jsonObj, "request_to_speak_timestamp"),

        jsonObj["deaf"].toBool(),
        jsonObj["mute"].toBool(),
        jsonObj["self_deaf"].toBool(),
        jsonObj["self_mute"].toBool(),
        jsonObj["self_stream"].toBool(),
        jsonObj["self_video"].toBool(),
        jsonObj["suppress"].toBool()
    };
}

template <>
void unmarshal<WelcomeScreenChannel>(QJsonObject jsonObj, WelcomeScreenChannel **object)
{
    *object = new WelcomeScreenChannel {
        getString(jsonObj, "channel_id"),
        getString(jsonObj, "description"),
        getString(jsonObj, "emoji_id"),
        getString(jsonObj, "emoji_name"),
    };
}

template <>
void unmarshal<WelcomeScreen>(QJsonObject jsonObj, WelcomeScreen **object)
{
    std::vector<WelcomeScreenChannel *> *welcomeChannels = new std::vector<WelcomeScreenChannel *>;

    unmarshalMultiple<WelcomeScreenChannel>(jsonObj, "welcome_channels", &welcomeChannels);

    *object = new WelcomeScreen {
        welcomeChannels,

        getString(jsonObj, "description"),
    };
}

template <>
void unmarshal<StageInstance>(QJsonObject jsonObj, StageInstance **object)
{
    *object = new StageInstance {
        getString(jsonObj, "id"),
        getString(jsonObj, "guild_id"),
        getString(jsonObj, "channel_id"),
        getString(jsonObj, "topic"),

        jsonObj["privacy_level"].toInt(-1),

        jsonObj["discoverable_disabled"].toBool()
    };
}

template <>
void unmarshal<Guild>(QJsonObject jsonObj, Guild **object)
{
    WelcomeScreen *welcomeScreen = new WelcomeScreen;
    std::vector<VoiceState *> *voiceStates = new std::vector<VoiceState *>;
    std::vector<GuildMember *> *members = new std::vector<GuildMember *>;
    std::vector<Channel *> *channels = new std::vector<Channel *>;
    std::vector<Channel *> *threads = new std::vector<Channel *>;
    std::vector<StageInstance *> *stageInstances = new std::vector<StageInstance *>;
    std::vector<Sticker *> *stickers = new std::vector<Sticker *>;

    unmarshal<WelcomeScreen>(jsonObj["welcome_screen"].toObject(), &welcomeScreen);
    unmarshalMultiple<VoiceState>(jsonObj["voice_states"].toArray(), &voiceStates);
    unmarshalMultiple<GuildMember>(jsonObj["members"].toArray(), &members);
    unmarshalMultiple<Channel>(jsonObj["channels"].toArray(), &channels);
    unmarshalMultiple<Channel>(jsonObj["threads"].toArray(), &threads);
    unmarshalMultiple<StageInstance>(jsonObj["stage_instances"].toArray(), &stageInstances);
    unmarshalMultiple<Sticker>(jsonObj["stickers"].toArray(), &stickers);

    *object = new Guild {
        welcomeScreen,
        getStringsFromJson(jsonObj["guild_features"].toArray()),
        voiceStates,
        members,
        channels,
        threads,
        nullptr,
        stageInstances,
        stickers,

        getString(jsonObj, "id"),
        getString(jsonObj, "name"),
        getString(jsonObj, "icon"),
        getString(jsonObj, "icon_hash"),
        getString(jsonObj, "splash"),
        getString(jsonObj, "discovery_splash"),
        getString(jsonObj, "owner_id"),
        getString(jsonObj, "permissions"),
        getString(jsonObj, "region"),
        getString(jsonObj, "afk_channel_id"),
        getString(jsonObj, "widget_channel_id"),
        getString(jsonObj, "application_id"),
        getString(jsonObj, "system_channel_id"),
        getString(jsonObj, "rules_channel_id"),
        getString(jsonObj, "joined_at"),
        getString(jsonObj, "vanity_url_code"),
        getString(jsonObj, "description"),
        getString(jsonObj, "banner"),
        getString(jsonObj, "preferred_locale"),
        getString(jsonObj, "public_updates_channel_id"),

        jsonObj["afk_timeout"].toInt(-1),
        jsonObj["verification_level"].toInt(-1),
        jsonObj["default_message_notifications"].toInt(-1),
        jsonObj["explicit_content_filter"].toInt(-1),
        jsonObj["mfa_level"].toInt(-1),
        jsonObj["system_channel_flags"].toInt(-1),
        jsonObj["member_count"].toInt(-1),
        jsonObj["max_presences"].toInt(-1),
        jsonObj["max_members"].toInt(-1),
        jsonObj["premium_tier"].toInt(-1),
        jsonObj["premium_subscription_count"].toInt(-1),
        jsonObj["max_video_channel_users"].toInt(-1),
        jsonObj["approximate_member_count"].toInt(-1),
        jsonObj["approximate_presence_count"].toInt(-1),
        jsonObj["nsfw_level"].toInt(-1),

        jsonObj["owner"].toBool(),
        jsonObj["widget_enabled"].toBool(),
        jsonObj["large"].toBool(),
        jsonObj["unavailable"].toBool()
    };
}

template <>
void unmarshal<CustomStatus>(QJsonObject jsonObj, CustomStatus **object)
{
    *object = new CustomStatus {
        getString(jsonObj, "text"),
        getString(jsonObj, "expires_at"),
        getString(jsonObj, "emoji_name"),
        getString(jsonObj, "emoji_id")
    };
}

template <>
void unmarshal<FriendSourceFlags>(QJsonObject jsonObj, FriendSourceFlags **object)
{
    *object = new FriendSourceFlags {
        jsonObj["all"].toBool(),
        jsonObj["mutual_friends"].toBool(),
        jsonObj["mutual_guilds"].toBool()
    };
}

template <>
void unmarshal<GuildFolder>(QJsonObject jsonObj, GuildFolder **object)
{
    *object = new GuildFolder {
        getStringsFromJson(jsonObj["guild_ids"].toArray()),

        getString(jsonObj, "name"),

        jsonObj["id"].toInt(-1),
        jsonObj["color"].toInt(0x005865F2)
    };
}

template <>
void unmarshal<ClientSettings>(QJsonObject jsonObj, ClientSettings **object)
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
        getStringsFromJson(jsonObj["guild_positions"].toArray()),
        getStringsFromJson(jsonObj["restricted_guilds"].toArray()),

        getString(jsonObj, "locale"),
        getString(jsonObj, "status"),
        getString(jsonObj, "theme"),

        jsonObj["afk_timeout"].toInt(-1),
        jsonObj["animate_stickers"].toInt(-1),
        jsonObj["explicit_content_filter"].toInt(-1),
        jsonObj["friend_discovery_flags"].toInt(-1),
        jsonObj["timezone_offset"].toInt(-1),

        jsonObj["allow_accessibility_detection"].toBool(),
        jsonObj["animate_emoji"].toBool(),
        jsonObj["contact_sync_enabled"].toBool(),
        jsonObj["convert_emoticons"].toBool(),
        jsonObj["default_guilds_restricted"].toBool(),
        jsonObj["detect_platform_accounts"].toBool(),
        jsonObj["developer_mode"].toBool(),
        jsonObj["disable_games_tab"].toBool(),
        jsonObj["enable_tts_command"].toBool(),
        jsonObj["gif_auto_play"].toBool(),
        jsonObj["inline_attachment_media"].toBool(),
        jsonObj["inline_embed_media"].toBool(),
        jsonObj["message_display_compact"].toBool(),
        jsonObj["native_phone_integration_enabled"].toBool(),
        jsonObj["render_embeds"].toBool(),
        jsonObj["render_reactions"].toBool(),
        jsonObj["show_current_game"].toBool(),
        jsonObj["stream_notifications_enabled"].toBool(),
        jsonObj["view_nsfw_guilds"].toBool()
    };
}

template <>
void unmarshal<Client>(QJsonObject jsonObj, Client **object)
{
    *object = new Client {
        getString(jsonObj, "id"),
        getString(jsonObj, "username"),
        getString(jsonObj, "avatar"),
        getString(jsonObj, "discriminator"),
        getString(jsonObj, "banner"),
        getString(jsonObj, "bio"),
        getString(jsonObj, "locale"),
        getString(jsonObj, "email"),
        getString(jsonObj, "phone"),

        jsonObj["public_flags"].toInt(-1),
        jsonObj["flags"].toInt(-1),
        jsonObj["purchased_flags"].toInt(-1),
        jsonObj["banner_color"].toInt(-1),
        jsonObj["accent_color"].toInt(-1),

        jsonObj["nsfw_allowed"].toBool(),
        jsonObj["mfa_enabled"].toBool(),
        jsonObj["verified"].toBool()
    };
}

template <>
void unmarshal<ActivityTimestamps>(QJsonObject jsonObj, ActivityTimestamps **object)
{
    *object = new ActivityTimestamps {
        jsonObj["start"].toInt(-1),
        jsonObj["end"].toInt(-1)
    };
}

template <>
void unmarshal<ActivityAssets>(QJsonObject jsonObj, ActivityAssets **object)
{
    *object = new ActivityAssets {
        getString(jsonObj, "large_image"),
        getString(jsonObj, "large_text"),
        getString(jsonObj, "small_image"),
        getString(jsonObj, "small_text")
    };
}

template <>
void unmarshal<PartySize>(QJsonObject jsonObj, PartySize **object)
{
    *object = new PartySize {
        jsonObj["current_size"].toInt(-1),
        jsonObj["max_size"].toInt(-1)
    };
}

template <>
void unmarshal<ActivityParty>(QJsonObject jsonObj, ActivityParty **object)
{
    PartySize *size = new PartySize;

    unmarshal<PartySize>(jsonObj, "size", &size);

    *object = new ActivityParty {
        size,

        getString(jsonObj, "id")
    };
}

template <>
void unmarshal<ActivitySecrets>(QJsonObject jsonObj, ActivitySecrets **object)
{
    *object = new ActivitySecrets {
        getString(jsonObj, "match"),
        getString(jsonObj, "join"),
        getString(jsonObj, "spectate")
    };
}

template <>
void unmarshal<Activity>(QJsonObject jsonObj, Activity **object)
{
    ActivityTimestamps *timestamps = new ActivityTimestamps;
    ActivityAssets *assets = new ActivityAssets;
    ActivityParty *party = new ActivityParty;
    ActivitySecrets *secrets = new ActivitySecrets;

    unmarshal<ActivityTimestamps>(jsonObj, "timestamps", &timestamps);
    unmarshal<ActivityAssets>(jsonObj, "assets", &assets);
    unmarshal<ActivityParty>(jsonObj, "party", &party);
    unmarshal<ActivitySecrets>(jsonObj, "secrets", &secrets);

    *object = new Activity {
        timestamps,
        assets,
        party,
        secrets,

        getString(jsonObj, "application_id"),
        getString(jsonObj, "id"),
        getString(jsonObj, "name"),
        getString(jsonObj, "state"),
        getString(jsonObj, "details"),

        jsonObj["created_at"].toInt(-1),
        jsonObj["type"].toInt(-1),

        jsonObj["instance"].toBool(false)
    };
}

template <>
void unmarshal<ClientStatus>(QJsonObject jsonObj, ClientStatus **object)
{
    *object = new ClientStatus {
        getString(jsonObj, "desktop"),
        getString(jsonObj, "mobile"),
        getString(jsonObj, "web")
    };
}

template <>
void unmarshal<Presence>(QJsonObject jsonObj, Presence **object)
{
    User *user = new User;
    ClientStatus *clientStatus = new ClientStatus;
    std::vector<Activity *> *activities = new std::vector<Activity *>;

    unmarshal<User>(jsonObj, "user", &user);
    unmarshal<ClientStatus>(jsonObj, "client_status", &clientStatus);
    unmarshalMultiple<Activity>(jsonObj, "activities", &activities);

    *object = new Presence {
        user,
        clientStatus,
        activities,

        getString(jsonObj, "user_id"),
        getString(jsonObj, "guild_id"),
        getString(jsonObj, "status")
    };
}

template <>
void unmarshal<Call>(QJsonObject jsonObj, Call **object)
{
    *object = new Call {
        getStringsFromJson(jsonObj["participants"].toArray()),
        getString(jsonObj, "ended_timestamp")
    };
}

} // namespace Api
