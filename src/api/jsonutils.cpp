#include "api/jsonutils.h"

#include "api/objects/message.h"
#include "api/objects/attachment.h"
#include "api/objects/user.h"
#include "api/objects/overwrite.h"
#include "api/objects/channel.h"
#include "api/objects/thread.h"
#include "api/objects/team.h"
#include "api/objects/application.h"
#include "api/objects/guildmember.h"
#include "api/objects/voice.h"
#include "api/objects/guild.h"
#include "api/objects/client.h"
#include "api/objects/presence.h"
#include "api/objects/optional.h"
#include "api/objects/error.h"

#include <QVariant>
#include <QJsonObject>
#include <QJsonDocument>

namespace Api {

QString getString(const QJsonObject& object, const char *key)
{
    return object[key].toString();
}

QVector<QString> getStringsFromJson(const QJsonArray& jsonArray)
{
    QVector<QString> strings;

    // Filling the vector
    for (int i = 0 ; i < jsonArray.size() ; i++) {
        strings.push_back(jsonArray[i].toString());
    }

    return strings;
}

QVector<Snowflake> getSnowflakesFromJson(const QJsonArray& jsonArray)
{
    QVector<Snowflake> snowflakes;

    // Filling the vector
    for (int i = 0 ; i < jsonArray.size() ; i++) {
        snowflakes.push_back(jsonArray[i].toVariant().toULongLong());
    }

    return snowflakes;
}

// All the specialization of 'unmarshal'

template <>
void unmarshal<User>(const QJsonObject& jsonObj, User **object)
{
    *object = new User {
        getString(jsonObj, "username"),
        getString(jsonObj, "discriminator"),
        getString(jsonObj, "avatar"),
        getString(jsonObj, "banner"),
        getString(jsonObj, "locale"),
        getString(jsonObj, "email"),

        jsonObj["id"].toVariant().toULongLong(),

        jsonObj["accent_color"].toInt(),
        jsonObj["flags"].toInt(),
        jsonObj["premium_type"].toInt(),
        jsonObj["public_flags"].toInt(),

        jsonObj["bot"].isNull() ? (optbool)jsonObj["bot"].toBool() : (optbool)Optional::None,
        jsonObj["system"].isNull() ? (optbool)jsonObj["system"].toBool() : (optbool)Optional::None,
        jsonObj["mfa_enabled"].isNull() ? (optbool)jsonObj["mfa_enabled"].toBool() : (optbool)Optional::None,
        jsonObj["verified"].isNull() ? (optbool)jsonObj["verified"].toBool() : (optbool)Optional::None
    };
}

template <>
void unmarshal<Overwrite>(const QJsonObject& jsonObj, Overwrite **object)
{
    *object = new Overwrite {
        getString(jsonObj, "allow"),
        getString(jsonObj, "deny"),

        jsonObj["id"].toVariant().toULongLong(),

        jsonObj["type"].toInt()
    };
}

template <>
void unmarshal<ThreadMember>(const QJsonObject& jsonObj, ThreadMember **object)
{
    *object = new ThreadMember {
        getString(jsonObj, "join_timestamp"),

        jsonObj["id"].toVariant().toULongLong(),
        jsonObj["user_id"].toVariant().toULongLong(),

        jsonObj["flags"].toInt()
    };
}

template <>
void unmarshal<ThreadMetadata>(const QJsonObject& jsonObj, ThreadMetadata **object)
{
    *object = new ThreadMetadata {
        getString(jsonObj, "archive_timestamp"),
        getString(jsonObj, "create_timestamp"),

        jsonObj["auto_archive_duration"].toInt(),

        (optbool)jsonObj["invitable"].isNull() ? (optbool)jsonObj["invitable"].toBool() : (optbool)Optional::None,
        jsonObj["archived"].toBool(),
        jsonObj["locked"].toBool()
    };
}

template <>
void unmarshal<Channel>(const QJsonObject& jsonObj, Channel **object)
{
    ThreadMember *member = new ThreadMember;
    ThreadMetadata *threadMetadata = new ThreadMetadata;

    unmarshal<ThreadMember>(jsonObj["member"].toObject(), &member);
    unmarshal<ThreadMetadata>(jsonObj["thread_metadata"].toObject(), &threadMetadata);

    *object = new Channel {
        unmarshalMultiple<User>(jsonObj["recipients"].toArray()),
        unmarshalMultiple<Overwrite>(jsonObj["permission_overwrites"].toArray()),
        member,
        threadMetadata,

        getString(jsonObj, "name"),
        getString(jsonObj, "topic"),
        getString(jsonObj, "icon"),
        getString(jsonObj, "last_pin_timestamp"),
        getString(jsonObj, "rtc_region"),
        getString(jsonObj, "permissions"),

        jsonObj["id"].toVariant().toULongLong(),
        jsonObj["guild_id"].toVariant().toULongLong(),
        jsonObj["last_message_id"].toVariant().toULongLong(),
        jsonObj["owner_id"].toVariant().toULongLong(),
        jsonObj["application_id"].toVariant().toULongLong(),
        jsonObj["parent_id"].toVariant().toULongLong(),

        jsonObj["type"].toInt(),
        jsonObj["position"].toInt(),
        jsonObj["bitrate"].toInt(),
        jsonObj["user_limit"].toInt(),
        jsonObj["rate_limit_per_user"].toInt(),
        jsonObj["video_quality_mode"].toInt(),
        jsonObj["message_count"].toInt(),
        jsonObj["member_count"].toInt(),
        jsonObj["default_auto_archive_duration"].toInt(),

        jsonObj["nsfw"].toBool()
    };
}

template <>
void unmarshal<PrivateChannel>(const QJsonObject& jsonObj, PrivateChannel **object)
{
    *object = new PrivateChannel {
        getSnowflakesFromJson(jsonObj["recipient_ids"].toArray()),

        getString(jsonObj, "icon"),
        getString(jsonObj, "name"),

        jsonObj["id"].toVariant().toULongLong(),
        jsonObj["last_message_id"].toVariant().toULongLong(),
        jsonObj["owner_id"].toVariant().toULongLong(),

        jsonObj["type"].toInt()
    };
}

template <>
void unmarshal<TeamMember>(const QJsonObject& jsonObj, TeamMember **object)
{
    User *user = new User;

    unmarshal<User>(jsonObj, "user", &user);

    *object = new TeamMember {
        *user,
        getStringsFromJson(jsonObj["permissions"].toArray()),

        jsonObj["team_id"].toVariant().toULongLong(),

        jsonObj["member_ship_state"].toInt()
    };
}

template <>
void unmarshal<Team>(const QJsonObject& jsonObj, Team **object)
{
    *object = new Team {
        unmarshalMultiple<TeamMember>(jsonObj, "members"),

        getString(jsonObj, "icon"),
        getString(jsonObj, "name"),

        jsonObj["id"].toVariant().toULongLong(),
        jsonObj["owner_user_id"].toVariant().toULongLong()
    };
}

template <>
void unmarshal<Application>(const QJsonObject& jsonObj, Application **object)
{
    User *owner = new User;
    Team *team = new Team;

    unmarshal<User>(jsonObj, "owner", &owner);
    unmarshal<Team>(jsonObj, "team", &team);

    *object = new Application {
        owner,
        team,
        getStringsFromJson(jsonObj["rpc_origins"].toArray()),

        getString(jsonObj, "name"),
        getString(jsonObj, "icon"),
        getString(jsonObj, "description"),
        getString(jsonObj, "terms_of_service_url"),
        getString(jsonObj, "privacy_policy_url"),
        getString(jsonObj, "summary"),
        getString(jsonObj, "verify_key"),
        getString(jsonObj, "slug"),
        getString(jsonObj, "cover_image"),

        jsonObj["id"].toVariant().toULongLong(),
        jsonObj["guild_id"].toVariant().toULongLong(),
        jsonObj["primary_sku_id"].toVariant().toULongLong(),

        jsonObj["flags"].toInt(),

        jsonObj["bot_public"].toBool(),
        jsonObj["bot_require_code_grant"].toBool()
    };
}

template <>
void unmarshal<MessageActivity>(const QJsonObject& jsonObj, MessageActivity **object)
{
    *object = new MessageActivity {
        getString(jsonObj, "party_id"),

        jsonObj["type"].toInt()
    };
}

template <>
void unmarshal<GuildMessageMember>(const QJsonObject& jsonObj, GuildMessageMember **object)
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
void unmarshal<MessageInteraction>(const QJsonObject& jsonObj, MessageInteraction **object)
{
    User *user = new User;
    GuildMember *member = new GuildMember;

    unmarshal<User>(jsonObj, "user", &user);
    unmarshal<GuildMember>(jsonObj, "member", &member);

    *object = new MessageInteraction {
        *user,
        member,

        getString(jsonObj, "name"),

        jsonObj["id"].toVariant().toULongLong(),

        jsonObj["type"].toInt()
    };
}

template <>
void unmarshal<Emoji>(const QJsonObject& jsonObj, Emoji **object)
{
    User *user = new User;

    unmarshal<User>(jsonObj, "user", &user);

    *object = new Emoji {
        user,
        getSnowflakesFromJson(jsonObj["roles"].toArray()),

        getString(jsonObj, "name"),

        jsonObj["id"].toVariant().toULongLong(),

        (optbool)jsonObj["requireColons"].isNull() ? (optbool)jsonObj["requireColons"].toBool() : (optbool)Optional::None,
        (optbool)jsonObj["managed"].isNull() ? (optbool)jsonObj["managed"].toBool() : (optbool)Optional::None,
        (optbool)jsonObj["animated"].isNull() ? (optbool)jsonObj["animated"].toBool() : (optbool)Optional::None,
        (optbool)jsonObj["available"].isNull() ? (optbool)jsonObj["available"].toBool() : (optbool)Optional::None
    };
}

template <>
void unmarshal<Reaction>(const QJsonObject& jsonObj, Reaction **object)
{
    Emoji *emoji = new Emoji;

    unmarshal<Emoji>(jsonObj, "emoji", &emoji);

    *object = new Reaction {
        *emoji,

        jsonObj["count"].toInt(),

        jsonObj["me"].toBool()
    };
}

template <>
void unmarshal<EmbedField>(const QJsonObject& jsonObj, EmbedField **object)
{
    *object = new EmbedField {
        getString(jsonObj, "name"),
        getString(jsonObj, "value"),

        (optbool)jsonObj["inline"].isNull() ? (optbool)jsonObj[""].toBool() : (optbool)Optional::None
    };
}

template <>
void unmarshal<EmbedFooter>(const QJsonObject& jsonObj, EmbedFooter **object)
{
    *object = new EmbedFooter {
        getString(jsonObj, "text"),
        getString(jsonObj, "icon_url"),
        getString(jsonObj, "proxy_icon_url"),
    };
}

template <>
void unmarshal<EmbedTVI>(const QJsonObject& jsonObj, EmbedTVI **object)
{
    *object = new EmbedTVI {
        getString(jsonObj, "url"),
        getString(jsonObj, "proxy_url"),

        jsonObj["height"].toInt(),
        jsonObj["width"].toInt()
    };
}

template <>
void unmarshal<EmbedProvider>(const QJsonObject& jsonObj, EmbedProvider **object)
{
    *object = new EmbedProvider {
        getString(jsonObj, "name"),
        getString(jsonObj, "url"),
    };
}

template <>
void unmarshal<EmbedAuthor>(const QJsonObject& jsonObj, EmbedAuthor **object)
{
    *object = new EmbedAuthor {
        getString(jsonObj, "name"),
        getString(jsonObj, "url"),
        getString(jsonObj, "icon_url"),
        getString(jsonObj, "proxy_icon_url"),
    };
}

template <>
void unmarshal<Embed>(const QJsonObject& jsonObj, Embed **object)
{
    EmbedFooter *footer = new EmbedFooter;
    EmbedTVI *image = new EmbedTVI;
    EmbedTVI *thumbnail = new EmbedTVI;
    EmbedTVI *video = new EmbedTVI;
    EmbedProvider *provider = new EmbedProvider;
    EmbedAuthor *author = new EmbedAuthor;

    unmarshal<EmbedFooter>(jsonObj, "footer", &footer);
    unmarshal<EmbedTVI>(jsonObj, "image", &image);
    unmarshal<EmbedTVI>(jsonObj, "thumbnail", &thumbnail);
    unmarshal<EmbedTVI>(jsonObj, "video", &video);
    unmarshal<EmbedProvider>(jsonObj, "provider", &provider);
    unmarshal<EmbedAuthor>(jsonObj, "author", &author);

    *object = new Embed {
        unmarshalMultiple<EmbedField>(jsonObj, "fields"),
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
void unmarshal<Attachment>(const QJsonObject& jsonObj, Attachment **object)
{
    *object = new Attachment {
        getString(jsonObj, "filename"),
        getString(jsonObj, "content_type"),
        getString(jsonObj, "description"),
        getString(jsonObj, "url"),
        getString(jsonObj, "proxy_url"),

        jsonObj["id"].toVariant().toULongLong(),

        jsonObj["size"].toInt(),
        jsonObj["height"].toInt(),
        jsonObj["width"].toInt(),

        (optbool)jsonObj["ephemeral"].isNull() ? (optbool)jsonObj[""].toBool() : (optbool)Optional::None
    };
}

template <>
void unmarshal<ChannelMention>(const QJsonObject& jsonObj, ChannelMention **object)
{
    *object = new ChannelMention {
        getString(jsonObj, "name"),

        jsonObj["id"].toVariant().toULongLong(),
        jsonObj["guild_id"].toVariant().toULongLong(),

        jsonObj["type"].toInt()
    };
}

template <>
void unmarshal<SelectOption>(const QJsonObject& jsonObj, SelectOption **object)
{
    Emoji *emoji = new Emoji;

    unmarshal<Emoji>(jsonObj, "emoji", &emoji);

    *object = new SelectOption {
        emoji,

        getString(jsonObj, "label"),
        getString(jsonObj, "value"),
        getString(jsonObj, "description"),

        (optbool)jsonObj["default"].isNull() ? (optbool)jsonObj[""].toBool() : (optbool)Optional::None
    };
}

template <>
void unmarshal<MessageComponent>(const QJsonObject& jsonObj, MessageComponent **object)
{
    Emoji *emoji = new Emoji;

    unmarshal<Emoji>(jsonObj, "emoji", &emoji);

    *object = new MessageComponent {
        emoji,
        unmarshalMultiple<SelectOption>(jsonObj, "components"),
        QVector<MessageComponent *>(),

        getString(jsonObj, "custom_id"),
        getString(jsonObj, "label"),
        getString(jsonObj, "url"),
        getString(jsonObj, "placeholder"),

        jsonObj["type"].toInt(),
        jsonObj["style"].toInt(),
        jsonObj["min_values"].toInt(),
        jsonObj["max_values"].toInt(),

        (optbool)jsonObj["disabled"].isNull() ? (optbool)jsonObj[""].toBool() : (optbool)Optional::None
    };
}

template <>
void unmarshal<StickerItem>(const QJsonObject& jsonObj, StickerItem **object)
{
    *object = new StickerItem {
        getString(jsonObj, "name"),

        jsonObj["id"].toVariant().toULongLong(),

        jsonObj["format_type"].toInt()
    };
}

template <>
void unmarshal<Sticker>(const QJsonObject& jsonObj, Sticker **object)
{
    User *user = new User;

    unmarshal<User>(jsonObj, "user", &user);

    *object = new Sticker {
        user,

        getString(jsonObj, "name"),
        getString(jsonObj, "description"),
        getString(jsonObj, "tags"),
        getString(jsonObj, "asset"),

        jsonObj["id"].toVariant().toULongLong(),
        jsonObj["pack_id"].toVariant().toULongLong(),
        jsonObj["guild_id"].toVariant().toULongLong(),

        jsonObj["type"].toInt(),
        jsonObj["format_type"].toInt(),
        jsonObj["sort_value"].toInt(),

        (optbool)jsonObj["available"].isNull() ? (optbool)jsonObj[""].toBool() : (optbool)Optional::None
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

    unmarshal<Application>(jsonObj, "application", &application);
    unmarshal<User>(jsonObj, "author", &author);
    unmarshal<MessageActivity>(jsonObj, "activity", &activity);
    unmarshal<GuildMessageMember>(jsonObj, "member", &member);
    unmarshal<Channel>(jsonObj, "thread", &thread);
    unmarshal<MessageInteraction>(jsonObj, "interaction", &interaction);
    unmarshal<Call>(jsonObj, "call", &call);

    return new Message {
        *author,
        application,
        activity,
        member,
        nullptr,
        thread,
        interaction,
        call,

        unmarshalMultiple<Reaction>(jsonObj, "reactions"),
        unmarshalMultiple<Embed>(jsonObj, "embeds"),
        unmarshalMultiple<User>(jsonObj, "mentions"),
        unmarshalMultiple<Attachment>(jsonObj, "attachments"),
        unmarshalMultiple<ChannelMention>(jsonObj, "mention_channels"),
        getStringsFromJson(jsonObj["mention_roles"].toArray()),
        unmarshalMultiple<MessageComponent>(jsonObj, "components"),
        unmarshalMultiple<StickerItem>(jsonObj, "sticker_items"),
        unmarshalMultiple<Sticker>(jsonObj, "stickers"),

        getString(jsonObj, "content"),
        getString(jsonObj, "timestamp"),
        getString(jsonObj, "edited_timestamp"),
        getString(jsonObj, "nonce"),

        jsonObj["id"].toVariant().toULongLong(),
        jsonObj["channel_id"].toVariant().toULongLong(),
        jsonObj["guild_id"].toVariant().toULongLong(),
        jsonObj["webhook_id"].toVariant().toULongLong(),
        jsonObj["application_id"].toVariant().toULongLong(),

        jsonObj["nonce"].toInt(),
        jsonObj["author_public_flags"].toInt(),
        jsonObj["type"].toInt(),
        jsonObj["flags"].toInt(),

        jsonObj["tts"].toBool(),
        jsonObj["pinned"].toBool(),
        jsonObj["mention_everyone"].toBool()
    };
}

template <>
void unmarshal<Message>(const QJsonObject& jsonObj, Message **object)
{
    Application *application = new Application;
    User *author = new User;
    MessageActivity *activity = new MessageActivity;
    GuildMessageMember *member = new GuildMessageMember;
    Channel *thread = new Channel;
    MessageInteraction *interaction = new MessageInteraction;
    Call *call = new Call;

    unmarshal<Application>(jsonObj, "application", &application);
    unmarshal<User>(jsonObj, "author", &author);
    unmarshal<MessageActivity>(jsonObj, "activity", &activity);
    unmarshal<GuildMessageMember>(jsonObj, "member", &member);
    unmarshal<Channel>(jsonObj, "thread", &thread);
    unmarshal<MessageInteraction>(jsonObj, "interaction", &interaction);
    unmarshal<Call>(jsonObj, "call", &call);

    *object = new Message {
        *author,
        application,
        activity,
        member,
        getPartialMessage(jsonObj, "referenced_message"),
        thread,
        interaction,
        call,

        unmarshalMultiple<Reaction>(jsonObj, "reactions"),
        unmarshalMultiple<Embed>(jsonObj, "embeds"),
        unmarshalMultiple<User>(jsonObj, "mentions"),
        unmarshalMultiple<Attachment>(jsonObj, "attachments"),
        unmarshalMultiple<ChannelMention>(jsonObj, "mention_channels"),
        getStringsFromJson(jsonObj["mention_roles"].toArray()),
        unmarshalMultiple<MessageComponent>(jsonObj, "components"),
        unmarshalMultiple<StickerItem>(jsonObj, "sticker_items"),
        unmarshalMultiple<Sticker>(jsonObj, "stickers"),

        getString(jsonObj, "content"),
        getString(jsonObj, "timestamp"),
        getString(jsonObj, "edited_timestamp"),
        getString(jsonObj, "nonce"),

        jsonObj["id"].toVariant().toULongLong(),
        jsonObj["channel_id"].toVariant().toULongLong(),
        jsonObj["guild_id"].toVariant().toULongLong(),
        jsonObj["webhook_id"].toVariant().toULongLong(),
        jsonObj["application_id"].toVariant().toULongLong(),

        jsonObj["nonce"].toInt(),
        jsonObj["author_public_flags"].toInt(),
        jsonObj["type"].toInt(),
        jsonObj["flags"].toInt(),

        jsonObj["tts"].toBool(),
        jsonObj["pinned"].toBool(),
        jsonObj["mention_everyone"].toBool()
    };
}

template <>
void unmarshal<GuildMember>(const QJsonObject& jsonObj, GuildMember **object)
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
        getString(jsonObj, "communication_disabled_until"),

        jsonObj["deaf"].toBool(),
        jsonObj["mute"].toBool(),
        jsonObj["pending"].toBool()
    };
}

template <>
void unmarshal<VoiceState>(const QJsonObject& jsonObj, VoiceState **object)
{
    GuildMember *member = new GuildMember;

    unmarshal<GuildMember>(jsonObj, "member", &member);

    *object = new VoiceState {
        member,

        getString(jsonObj, "session_id"),
        getString(jsonObj, "request_to_speak_timestamp"),

        jsonObj["guild_id"].toVariant().toULongLong(),
        jsonObj["channel_id"].toVariant().toULongLong(),
        jsonObj["user_id"].toVariant().toULongLong(),

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
void unmarshal<WelcomeScreenChannel>(const QJsonObject& jsonObj, WelcomeScreenChannel **object)
{
    *object = new WelcomeScreenChannel {
        getString(jsonObj, "description"),
        getString(jsonObj, "emoji_name"),

        jsonObj["channel_id"].toVariant().toULongLong(),
        jsonObj["emoji_id"].toVariant().toULongLong(),
    };
}

template <>
void unmarshal<WelcomeScreen>(const QJsonObject& jsonObj, WelcomeScreen **object)
{
    *object = new WelcomeScreen {
        unmarshalMultiple<WelcomeScreenChannel>(jsonObj, "welcome_channels"),

        getString(jsonObj, "description"),
    };
}

template <>
void unmarshal<StageInstance>(const QJsonObject& jsonObj, StageInstance **object)
{
    *object = new StageInstance {
        getString(jsonObj, "topic"),

        jsonObj["id"].toVariant().toULongLong(),
        jsonObj["guild_id"].toVariant().toULongLong(),
        jsonObj["channel_id"].toVariant().toULongLong(),
        jsonObj["guild_scheduled_event_id"].toVariant().toULongLong(),

        jsonObj["privacy_level"].toInt(),

        jsonObj["discoverable_disabled"].toBool()
    };
}

template <>
void unmarshal<Guild>(const QJsonObject& jsonObj, Guild **object)
{
    WelcomeScreen *welcomeScreen = new WelcomeScreen;

    unmarshal<WelcomeScreen>(jsonObj["welcome_screen"].toObject(), &welcomeScreen);

    *object = new Guild {
        welcomeScreen,
        getStringsFromJson(jsonObj["guild_features"].toArray()),
        unmarshalMultiple<VoiceState>(jsonObj["voice_states"].toArray()),
        unmarshalMultiple<GuildMember>(jsonObj["members"].toArray()),
        unmarshalMultiple<Channel>(jsonObj["channels"].toArray()),
        unmarshalMultiple<Channel>(jsonObj["threads"].toArray()),
        nullptr,
        unmarshalMultiple<StageInstance>(jsonObj["stage_instances"].toArray()),
        unmarshalMultiple<Sticker>(jsonObj["stickers"].toArray()),

        getString(jsonObj, "name"),
        getString(jsonObj, "icon"),
        getString(jsonObj, "icon_hash"),
        getString(jsonObj, "splash"),
        getString(jsonObj, "discovery_splash"),
        getString(jsonObj, "permissions"),
        getString(jsonObj, "region"),
        getString(jsonObj, "joined_at"),
        getString(jsonObj, "vanity_url_code"),
        getString(jsonObj, "description"),
        getString(jsonObj, "banner"),
        getString(jsonObj, "preferred_locale"),

        jsonObj["id"].toVariant().toULongLong(),
        jsonObj["owner_id"].toVariant().toULongLong(),
        jsonObj["afk_channel_id"].toVariant().toULongLong(),
        jsonObj["widget_channel_id"].toVariant().toULongLong(),
        jsonObj["application_id"].toVariant().toULongLong(),
        jsonObj["system_channel_id"].toVariant().toULongLong(),
        jsonObj["rules_channel_id"].toVariant().toULongLong(),
        jsonObj["public_updates_channel_id"].toVariant().toULongLong(),

        jsonObj["afk_timeout"].toInt(),
        jsonObj["verification_level"].toInt(),
        jsonObj["default_message_notifications"].toInt(),
        jsonObj["explicit_content_filter"].toInt(),
        jsonObj["mfa_level"].toInt(),
        jsonObj["system_channel_flags"].toInt(),
        jsonObj["member_count"].toInt(),
        jsonObj["max_presences"].toInt(),
        jsonObj["max_members"].toInt(),
        jsonObj["premium_tier"].toInt(),
        jsonObj["premium_subscription_count"].toInt(),
        jsonObj["max_video_channel_users"].toInt(),
        jsonObj["approximate_member_count"].toInt(),
        jsonObj["approximate_presence_count"].toInt(),
        jsonObj["nsfw_level"].toInt(),

        (optbool)jsonObj["owner"].isNull() ? (optbool)jsonObj[""].toBool() : (optbool)Optional::None,
        (optbool)jsonObj["widget_enabled"].isNull() ? (optbool)jsonObj[""].toBool() : (optbool)Optional::None,
        (optbool)jsonObj["large"].isNull() ? (optbool)jsonObj[""].toBool() : (optbool)Optional::None,
        (optbool)jsonObj["unavailable"].isNull() ? (optbool)jsonObj[""].toBool() : (optbool)Optional::None,
        jsonObj["premium_progress_bar_enabled"].toBool()
    };
}

template <>
void unmarshal<CustomStatus>(const QJsonObject& jsonObj, CustomStatus **object)
{
    *object = new CustomStatus {
        getString(jsonObj, "text"),
        getString(jsonObj, "expires_at"),
        getString(jsonObj, "emoji_name"),

        jsonObj["emoji_id"].toVariant().toULongLong()
    };
}

template <>
void unmarshal<FriendSourceFlags>(const QJsonObject& jsonObj, FriendSourceFlags **object)
{
    *object = new FriendSourceFlags {
        jsonObj["all"].toBool(),
        jsonObj["mutual_friends"].toBool(),
        jsonObj["mutual_guilds"].toBool()
    };
}

template <>
void unmarshal<GuildFolder>(const QJsonObject& jsonObj, GuildFolder **object)
{
    *object = new GuildFolder {
        getSnowflakesFromJson(jsonObj["guild_ids"].toArray()),

        getString(jsonObj, "name"),
        getString(jsonObj, "id"),

        jsonObj["id"].toVariant().toLongLong(),
        jsonObj["color"].toInt(0x005865F2)
    };
}

template <>
void unmarshal<ClientSettings>(const QJsonObject& jsonObj, ClientSettings **object)
{
    CustomStatus *customStatus = new CustomStatus;
    FriendSourceFlags *friendSourceFlags = new FriendSourceFlags;

    unmarshal<CustomStatus>(jsonObj, "custom_status", &customStatus);
    unmarshal<FriendSourceFlags>(jsonObj, "friend_source_flags", &friendSourceFlags);

    *object = new ClientSettings {
        customStatus,
        friendSourceFlags,
        unmarshalMultiple<GuildFolder>(jsonObj, "guild_folders"),
        getStringsFromJson(jsonObj["guild_positions"].toArray()),
        getStringsFromJson(jsonObj["restricted_guilds"].toArray()),

        getString(jsonObj, "locale"),
        getString(jsonObj, "status"),
        getString(jsonObj, "theme"),

        jsonObj["afk_timeout"].toInt(),
        jsonObj["animate_stickers"].toInt(),
        jsonObj["explicit_content_filter"].toInt(),
        jsonObj["friend_discovery_flags"].toInt(),
        jsonObj["timezone_offset"].toInt(),

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
void unmarshal<Client>(const QJsonObject& jsonObj, Client **object)
{
    *object = new Client {
        getString(jsonObj, "username"),
        getString(jsonObj, "avatar"),
        getString(jsonObj, "discriminator"),
        getString(jsonObj, "banner"),
        getString(jsonObj, "bio"),
        getString(jsonObj, "locale"),
        getString(jsonObj, "email"),
        getString(jsonObj, "phone"),

        jsonObj["id"].toVariant().toULongLong(),

        jsonObj["public_flags"].toInt(),
        jsonObj["flags"].toInt(),
        jsonObj["purchased_flags"].toInt(),
        jsonObj["banner_color"].toInt(),
        jsonObj["accent_color"].toInt(),

        jsonObj["nsfw_allowed"].toBool(),
        jsonObj["mfa_enabled"].toBool(),
        jsonObj["verified"].toBool()
    };
}

template <>
void unmarshal<ActivityTimestamps>(const QJsonObject& jsonObj, ActivityTimestamps **object)
{
    *object = new ActivityTimestamps {
        jsonObj["start"].toInt(),
        jsonObj["end"].toInt()
    };
}

template <>
void unmarshal<ActivityAssets>(const QJsonObject& jsonObj, ActivityAssets **object)
{
    *object = new ActivityAssets {
        getString(jsonObj, "large_image"),
        getString(jsonObj, "large_text"),
        getString(jsonObj, "small_image"),
        getString(jsonObj, "small_text"),
    };
}

template <>
void unmarshal<PartySize>(const QJsonObject& jsonObj, PartySize **object)
{
    *object = new PartySize {
        jsonObj["current_size"].toInt(),
        jsonObj["max_size"].toInt()
    };
}

template <>
void unmarshal<ActivityParty>(const QJsonObject& jsonObj, ActivityParty **object)
{
    PartySize *size = new PartySize;

    unmarshal<PartySize>(jsonObj, "size", &size);

    *object = new ActivityParty {
        size,

        getString(jsonObj, "id"),
    };
}

template <>
void unmarshal<ActivitySecrets>(const QJsonObject& jsonObj, ActivitySecrets **object)
{
    *object = new ActivitySecrets {
        getString(jsonObj, "match"),
        getString(jsonObj, "join"),
        getString(jsonObj, "spectate"),
    };
}

template <>
void unmarshal<Activity>(const QJsonObject& jsonObj, Activity **object)
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

        getString(jsonObj, "name"),
        getString(jsonObj, "state"),
        getString(jsonObj, "details"),

        jsonObj["application_id"].toVariant().toLongLong(),

        jsonObj["instance"].toBool(false)
    };
}

template <>
void unmarshal<ClientStatus>(const QJsonObject& jsonObj, ClientStatus **object)
{
    *object = new ClientStatus {
        getString(jsonObj, "desktop"),
        getString(jsonObj, "mobile"),
        getString(jsonObj, "web"),
    };
}

template <>
void unmarshal<Presence>(const QJsonObject& jsonObj, Presence **object)
{
    User *user = new User;
    ClientStatus *clientStatus = new ClientStatus;

    unmarshal<User>(jsonObj, "user", &user);
    unmarshal<ClientStatus>(jsonObj, "client_status", &clientStatus);

    *object = new Presence {
        user,
        clientStatus,
        unmarshalMultiple<Activity>(jsonObj, "activities"),

        getString(jsonObj, "status"),

        jsonObj["user_id"].toVariant().toULongLong(),
        jsonObj["guild_id"].toVariant().toULongLong()
    };
}

template <>
void unmarshal<Call>(const QJsonObject& jsonObj, Call **object)
{
    *object = new Call {
        getSnowflakesFromJson(jsonObj["participants"].toArray()),

        getString(jsonObj, "ended_timestamp"),
    };
}

template <>
void unmarshal<Error>(const QJsonObject& jsonObj, Error **object)
{
    *object = new Error {
        getString(jsonObj, "message"),
        getString(jsonObj, "code"),
        jsonObj["code"].toInt(),
    };
}

} // namespace Api
