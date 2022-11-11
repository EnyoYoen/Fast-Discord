#include "api/ressourcemanager.h"

#include <QFile>
#include <QJsonDocument>

#include <algorithm>

namespace Api {

RessourceManager::RessourceManager(const QString& token)
{
    requester = new Requester(token, this);
    gw = new Gateway(requester, token);
    vs = new VoiceSocket();

    // Set the gateway event callback
    gw->onDispatch([this](QString& eventName, json& data){gatewayDispatchHandler(eventName, data);});
}

void RessourceManager::call(const Snowflake& guildId, const Snowflake& channelId, bool selfMute, bool selfDeaf)
{
    gw->sendCall([&](QString voiceSessionId, QString voiceEndpoint, QString voiceToken) {
        vs->start(voiceEndpoint, guildId, client->id, voiceSessionId, voiceToken);
    }, guildId, channelId, selfMute, selfDeaf);
}

void RessourceManager::stopCall()
{
    gw->sendVoiceStateUpdate(Snowflake(), Snowflake(), vs->mute, vs->deaf);
}

void const RessourceManager::setToken(QString token)
{
    gw->setToken(token);
}

void RessourceManager::setClient(Client *clientp)
{
    client = clientp;
}

void RessourceManager::gatewayDispatchHandler(QString& eventName, json& data)
{
    // Process gateway events
    if (eventName == "READY") {
        guilds = Api::unmarshalMultiple<Api::Guild>(data["guilds"].toArray());
        emit guildsReceived(guilds);

        privateChannels = Api::unmarshalMultiple<Api::PrivateChannel>(data["private_channels"].toArray());
        std::sort(privateChannels.begin(), privateChannels.end(),
            [](const Api::PrivateChannel *a, const Api::PrivateChannel *b) -> bool {
                if (a->lastMessageId == 0 && b->lastMessageId == 0)
                    return a->id < b->id;
                if (a->lastMessageId == 0) 
                    return a->id < a->lastMessageId;
                if (b->lastMessageId == 0)
                    return a->lastMessageId < b->id;
                return a->lastMessageId < b->lastMessageId;
            });
        std::reverse(privateChannels.begin(), privateChannels.end());

        users = Api::unmarshalMultiple<Api::User>(data["users"].toArray());
        emit privateChannelsReceived(privateChannels);
    } else if (eventName == "READY_SUPPLEMENTAL") {
        presences = Api::unmarshalMultiple<Api::Presence>(data["merged_presences"]["friends"].toArray());
        emit presencesReceived(presences);
    } else if (eventName == "GUILD_CREATE") {
        Api::GuildGateway *guild;
        Api::unmarshal<Api::GuildGateway>(data.toObject(), &guild);
        guilds.append(guild->guild);
        emit guildCreated(guild);
    } else if (eventName == "GUILD_UPDATE") {
        Api::Guild *guild;
        Api::unmarshal<Api::Guild>(data.toObject(), &guild);
        for (int i = 0 ; i < guilds.size() ; i++) {
            if (guilds[i]->id == guild->id) {
                guilds.replace(i, guild);
            }
        }
        emit guildUpdated(guild);
    } else if (eventName == "GUILD_DELETE") {
        Api::Guild *guild;
        Api::unmarshal<Api::Guild>(data.toObject(), &guild);
        for (int i = 0 ; i < guilds.size() ; i++) {
            if (guilds[i]->id == guild->id) {
                guilds.remove(i);
            }
        }
        emit guildDeleted(guild);
    } else if (eventName == "CHANNEL_CREATE") {
        Api::Channel *channel;
        Api::unmarshal<Api::Channel>(data.toObject(), &channel);
        if (channel->type == DM || channel->type == GroupDM) {
            QVector<Snowflake> recipients;
            if (!channel->recipients.empty()) {
                for (int i = 0 ; i < channel->recipients.size() ; i++) {
                    users.push_back(channel->recipients[i]);
                    recipients.push_back(channel->recipients[i]->id);
                }
            }

            PrivateChannel *privateChannel = new PrivateChannel {
                recipients,
                channel->icon,
                channel->name,
                channel->id,
                channel->lastMessageId,
                channel->ownerId,
                channel->type
            };
            privateChannels.insert(std::upper_bound(privateChannels.begin(), privateChannels.end(), privateChannel,
            [](const Api::PrivateChannel *a, const Api::PrivateChannel *b) {
                if (a->lastMessageId == 0 && b->lastMessageId == 0)
                    return a->id > b->id;
                if (a->lastMessageId == 0) 
                    return a->id > a->lastMessageId;
                if (b->lastMessageId == 0)
                    return a->lastMessageId > b->id;
                return a->lastMessageId > b->lastMessageId;
            }), privateChannel);

            emit channelCreated(nullptr, privateChannel);
        } else {
            guildsChannels[channel->guildId].push_back(channel);

            emit channelCreated(channel, nullptr);
        }
    } else if (eventName == "CHANNEL_UPDATE") {
        Api::Channel *channel;
        Api::unmarshal<Api::Channel>(data.toObject(), &channel);
        if (channel->type == DM || channel->type == GroupDM) {
            QVector<Snowflake> recipients;
            if (!channel->recipients.empty()) {
                for (int i = 0 ; i < channel->recipients.size() ; i++) {
                    users.push_back(channel->recipients[i]);
                    recipients.push_back(channel->recipients[i]->id);
                }
            }

            for (auto it = privateChannels.begin() ; it != privateChannels.end() ; it++) {
                if ((*it)->id == channel->id) {
                    privateChannels.erase(it);
                    break;
                }
            }

            PrivateChannel *privateChannel = new PrivateChannel {
                recipients,
                channel->icon,
                channel->name,
                channel->id,
                channel->lastMessageId,
                channel->ownerId,
                channel->type
            };
            privateChannels.insert(std::upper_bound(privateChannels.begin(), privateChannels.end(), privateChannel,
            [](const Api::PrivateChannel *a, const Api::PrivateChannel *b) {
                if (a->lastMessageId == 0 && b->lastMessageId == 0)
                    return a->id > b->id;
                if (a->lastMessageId == 0) 
                    return a->id > a->lastMessageId;
                if (b->lastMessageId == 0)
                    return a->lastMessageId > b->id;
                return a->lastMessageId > b->lastMessageId;
            }), privateChannel);

            emit channelUpdated(nullptr, privateChannel);
        } else {
            for (auto it = guildsChannels[channel->guildId].begin() ; it != guildsChannels[channel->guildId].end() ; it++) {
                if ((*it)->id == channel->id) {
                    guildsChannels[channel->guildId].erase(it);
                    break;
                }
            }
            guildsChannels[channel->guildId].push_back(channel);

            emit channelUpdated(channel, nullptr);
        }
    } else if (eventName == "CHANNEL_DELETE") {
        Api::Channel *channel;
        Api::unmarshal<Api::Channel>(data.toObject(), &channel);
        if (channel->type == DM || channel->type == GroupDM) {
            for (auto it = privateChannels.begin() ; it != privateChannels.end() ; it++) {
                if ((*it)->id == channel->id) {
                    privateChannels.erase(it);
                    break;
                }
            }

            emit channelDeleted(channel->id, 0, channel->type);
        } else {
            for (auto it = guildsChannels[channel->guildId].begin() ; it != guildsChannels[channel->guildId].end() ; it++) {
                if ((*it)->id == channel->id) {
                    guildsChannels[channel->guildId].erase(it);
                    break;
                }
            }

            emit channelDeleted(channel->id, channel->guildId, channel->type);
        }
    } else if (eventName == "CHANNEL_UNREAD_UPDATE") {
        emit unreadUpdateReceived(Snowflake(data["guild_id"].toVariant().toULongLong()));
    } else if (eventName == "MESSAGE_CREATE") {
        Api::Message *message;
        Api::unmarshal<Api::Message>(data.toObject(), &message);
        if (messages.find(message->channelId) != messages.end())
            messages[message->channelId].insert(messages[message->channelId].begin(), message);
        emit messageReceived(*message);
    } else if (eventName == "PRESENCE_UPDATE") {
        Api::Presence *presence;
        Api::unmarshal<Api::Presence>(data.toObject(), &presence);
        emit presenceReceived(*presence);
    } else if (eventName == "GUILD_MEMBER_LIST_UPDATE") {
        Api::GuildMemberGateway *members;
        Api::unmarshal<Api::GuildMemberGateway>(data.toObject(), &members);

        for (int i = 0 ; i < members->structs.size() ; i++) {
            if (members->ops[i] == Api::GuildMemberOp::Sync) {
                Api::GuildMemberSync *sync = reinterpret_cast<Api::GuildMemberSync *>(members->structs[i]);
                for (int j = 0 ; j < sync->groupsMembers.size() ; j++) {
                    guildsMembers[members->guildId].append(sync->groupsMembers.values()[j]);
                }
                groupsCount[members->guildId] = sync->groupsCount;
            } else if (members->ops[i] == Api::GuildMemberOp::Update) {

            } else if (members->ops[i] == Api::GuildMemberOp::Delete) {

            } else {

            }
        }

        emit memberUpdateReceived(*members);
    } else if (eventName == "MESSAGE_REACTION_ADD") {
        Api::GuildMember *member = nullptr;
        Api::Emoji *emoji = nullptr;
        unmarshal<Api::GuildMember>(data, "member", &member);
        unmarshal<Api::Emoji>(data, "emoji", &emoji);
        emit reactionAdded(data["user_id"].toVariant().toULongLong(),
                           data["channel_id"].toVariant().toULongLong(),
                           data["message_id"].toVariant().toULongLong(),
                           data["guild_id"].toVariant().toULongLong(),
                           member,
                           emoji);
    } else if (eventName == "MESSAGE_REACTION_REMOVE") {
        Api::Emoji *emoji = nullptr;
        unmarshal<Api::Emoji>(data, "emoji", &emoji);
        emit reactionRemoved(data["user_id"].toVariant().toULongLong(),
                             data["channel_id"].toVariant().toULongLong(),
                             data["message_id"].toVariant().toULongLong(),
                             data["guild_id"].toVariant().toULongLong(),
                             emoji);
    } else if (eventName == "MESSAGE_REACTION_REMOVE_ALL") {
        emit reactionRemoved(0,
                             data["channel_id"].toVariant().toULongLong(),
                             data["message_id"].toVariant().toULongLong(),
                             data["guild_id"].toVariant().toULongLong(),
                             nullptr);
    } else if (eventName == "MESSAGE_REACTION_REMOVE_ALL") {
        Api::Emoji *emoji = nullptr;
        unmarshal<Api::Emoji>(data, "emoji", &emoji);
        emit reactionRemoved(0,
                             data["channel_id"].toVariant().toULongLong(),
                             data["message_id"].toVariant().toULongLong(),
                             data["guild_id"].toVariant().toULongLong(),
                             emoji);
    }
}

void RessourceManager::getGuildMembers(Callback callback, Snowflake channelId)
{
    bool found = false;
    for (auto it = openedGuildsChannels.begin() ; it != openedGuildsChannels.end() ; it++) {
        QMap<Snowflake, QVector<QVector<int>>> channels = it.value();
        if (channels.find(channelId) != channels.end()) {
            found = true;
            
            int index = channels[channelId].size();

            if (index == 0) {
                for (auto it2 = openedGuildsChannels.begin() ; it2 != openedGuildsChannels.end() ; it2++) {
                    for (auto it3 = it2.value().begin() ; it3 != it2.value().end() ; it3++) {
                        if (it3.key() != channelId)
                            it3.value().clear();
                    }
                }
                
                QVector<int> indexes;
                indexes.push_back(0);
                indexes.push_back(99);
                channels[channelId].push_back(indexes);
                openedGuildsChannels[it.key()] = channels;
            } else {
                
            }
            
            gw->sendGuildChannelOpened(channels, it.key(), !(bool)index, !(bool)index, !(bool)index);
            break;
        }
    }
    if (!found) {
        gw->sendDMChannelOpened(channelId);
    }

    for (auto it = openedGuildsChannels.begin() ; it != openedGuildsChannels.end() ; it++) {
        QMap<Snowflake, QVector<QVector<int>>> channels = it.value();
        if (channels.find(channelId) != channels.end()) {
            int index = channels[channelId].size() * 100;
            QVector<int> indexes;
            indexes.push_back(index);
            indexes.push_back(index + 99);
            channels[channelId].push_back(indexes);
            gw->sendGuildChannelOpened(channels, it.key(), false, false, false);
            break;
        }
    }
}

void RessourceManager::getGuildMember(Callback callback, const Snowflake& guildId, const Snowflake& userId)
{
    if (guildsMembers.keys().contains(guildId)) {
        QVector<Api::GuildMember *> members = guildsMembers[guildId];
        int index = -1;
        for (int i = 0 ; i < members.size() ; i++) {
            if (members[i]->user->id == userId) index = i;
        }
        if (index != -1) {
            callback(reinterpret_cast<void *>(members[index]));
        } else {
            requester->getGuildMember([this, callback, guildId, userId](CallbackStruct cb){
                guildsMembers[guildId].append(reinterpret_cast<GuildMember *>(cb.data));
                callback(cb);
            }, guildId, userId);
        }
    } else {
        requester->getGuildMember([this, callback, guildId, userId](CallbackStruct cb){
            guildsMembers[guildId].append(reinterpret_cast<GuildMember *>(cb.data));
            callback(cb);
        }, guildId, userId);
    }
}

void RessourceManager::getGuilds(Callback callback)
{
    if (guilds.empty())
        requester->getGuilds([&, callback](CallbackStruct cb) {
            guilds = *reinterpret_cast<QVector<Guild *> *>(cb.data);
            callback(cb);
        });
    else
        callback(CallbackStruct(reinterpret_cast<void *>(&guilds)));
}

void RessourceManager::getGuildChannels(Callback callback, const Snowflake& id)
{
    requester->getGuildChannels([&, callback](CallbackStruct cb) {
        guildsChannels[id] = *reinterpret_cast<QVector<Channel *> *>(cb.data);
        callback(cb);
    }, id);
}

void RessourceManager::getGuildChannel(Callback callback, const Snowflake& guildId, const Snowflake& id)
{
    openedGuildsChannels[guildId][id];

    if (guildsChannels.find(guildId) == guildsChannels.end()) {
        requester->getGuildChannels([&, callback](CallbackStruct cb) {
            guildsChannels[guildId] = *reinterpret_cast<QVector<Channel *> *>(cb.data);
            callback(cb);
        }, guildId);
    } else {
        for (int i = 0 ; i < guildsChannels[guildId].size() ; i++) {
            if (guildsChannels[guildId][i]->id == id) callback(reinterpret_cast<void *>(guildsChannels[guildId][i]));
        }
    }
}

void RessourceManager::getPrivateChannel(Callback callback, const Snowflake& id)
{
    if (privateChannels.empty()) {
        requester->getPrivateChannels([&, callback](CallbackStruct cb) {
            privateChannels = *reinterpret_cast<QVector<PrivateChannel *> *>(cb.data);
            for (int i = 0 ; i < privateChannels.size() ; i++) {
                if (privateChannels[i]->id == id) callback(CallbackStruct(reinterpret_cast<void *>(privateChannels[i])));
            }
        });
        return;
    }
    for (int i = 0 ; i < privateChannels.size() ; i++) {
        if (privateChannels[i]->id == id) callback(CallbackStruct(reinterpret_cast<void *>(privateChannels[i])));
    }
}

void RessourceManager::getPrivateChannels(Callback callback)
{
    if (privateChannels.empty())
        requester->getPrivateChannels([&, callback](CallbackStruct cb) {
            privateChannels = *reinterpret_cast<QVector<PrivateChannel *> *>(cb.data);
            callback(cb);
        });
    else
        callback(reinterpret_cast<void *>(&privateChannels));
}

void RessourceManager::getMessages(Callback callback, const Snowflake& channelId, int limit, bool newMessages)
{
    if (newMessages) {
        if (messages[channelId].size() >= 50)
            requester->getMessages([&, callback](CallbackStruct cb) {
                QVector<Message *> messagesVector = *reinterpret_cast<QVector<Message *> *>(cb.data);
                if (messagesVector.size() > 0) {
                    for (int i = 0 ; i < messagesVector.size() ; i++)
                        messages[channelId].push_back(messagesVector[i]);
                    callback(cb);
                }
            }, channelId, messages[channelId].back()->id, limit);
    } else if (messages.find(channelId) == messages.end() || messages[channelId].size() == 0) {
        requester->getMessages([&, callback](CallbackStruct cb) {
            messages[channelId] = *reinterpret_cast<QVector<Message *> *>(cb.data);
            callback(cb);
        }, channelId, 0, limit);
    } else if (messages[channelId].size() < 50) {
        callback(reinterpret_cast<void *>(&messages[channelId]));
    } else if (messages[channelId].size() < limit) {
        requester->getMessages([&, callback](CallbackStruct cb) {
            QVector<Message *> messagesVector = *reinterpret_cast<QVector<Message *> *>(cb.data);
            for (int i = 0 ; i < messagesVector.size() ; i++)
                messages[channelId].push_back(messagesVector[i]);
            callback(CallbackStruct(reinterpret_cast<void *>(&messages[channelId])));
        }, channelId, messages[channelId].back()->id, limit - messages[channelId].size());
    } else {
        callback(reinterpret_cast<void *>(&messages[channelId]));
    }
}

void RessourceManager::getClient(Callback callback)
{
    if (client == nullptr)
        requester->getClient([&, callback](CallbackStruct cb) {
            client = reinterpret_cast<Client *>(cb.data);
            callback(cb);
        });
    else
        callback(CallbackStruct(reinterpret_cast<void *>(client)));
}

void RessourceManager::getClientSettings(Callback callback)
{
    if (clientSettings == nullptr)
        requester->getClientSettings([&, callback](CallbackStruct cb) {
            clientSettings = reinterpret_cast<ClientSettings *>(cb.data);
            callback(cb);
       });
    else
        callback(CallbackStruct(reinterpret_cast<void *>(clientSettings)));
}

void RessourceManager::getImage(Callback callback, const QString& url, const QString& fileName)
{
    if (!QFile::exists("cache/" + fileName)) {
        requester->getImage(callback, url, fileName);}
    else {
        QString path = "cache/" + fileName;
        callback(CallbackStruct(reinterpret_cast<void *>(const_cast<QString *>(&path))));
    }
}

void RessourceManager::getUser(Callback callback, const Snowflake& userId)
{
    for (int i = 0 ; i < users.size() ; i++) {
        if (users[i]->id == userId) {
            callback(reinterpret_cast<void *>(users[i]));
            return;
        }
    }
    requester->getUser([&, callback](CallbackStruct cb) {
       users.push_back(reinterpret_cast<User *>(cb.data));
       callback(cb);
   }, userId);
}

void RessourceManager::getPresences(Callback callback)
{
    callback(reinterpret_cast<void *>(&presences));
}


QVector<Api::Message *> const RessourceManager::getAllMessages(const Snowflake& channelId)
{
    return messages[channelId];
}

bool const RessourceManager::hasMessages(const Snowflake& channelId)
{
    return messages.find(channelId) == messages.end();
}


RessourceManager::~RessourceManager()
{
    delete requester;
    delete gw;
}

} // namespace Api
