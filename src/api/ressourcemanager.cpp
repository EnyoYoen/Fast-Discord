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
            [](const Api::PrivateChannel *a, const Api::PrivateChannel *b) {
                if (a->lastMessageId == 0 && b->lastMessageId == 0)
                    return a->id > b->id;
                if (a->lastMessageId == 0) 
                    return a->id > a->lastMessageId;
                if (b->lastMessageId == 0)
                    return a->lastMessageId > b->id;
                return a->lastMessageId > b->lastMessageId;
            });

        users = Api::unmarshalMultiple<Api::User>(data["users"].toArray());
        emit privateChannelsReceived(privateChannels);
    } else if (eventName == "READY_SUPPLEMENTAL") {
        presences = Api::unmarshalMultiple<Api::Presence>(data["merged_presences"]["friends"].toArray());
        emit presencesReceived(presences);
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
        // We received a message
        Api::Message *message;
        Api::unmarshal<Api::Message>(data.toObject(), &message);
        if (messages.find(message->channelId) != messages.end())
            messages[message->channelId].insert(messages[message->channelId].begin(), message);
        emit messageReceived(*message);
    } else if (eventName == "PRESENCE_UPDATE") {
        Api::Presence *presence;
        Api::unmarshal<Api::Presence>(data.toObject(), &presence);
        emit presenceReceived(*presence);
    }
}

void RessourceManager::getGuilds(Callback callback)
{
    if (guilds.empty())
        requester->getGuilds([&, callback](void *guildsPtr) {
            guilds = *reinterpret_cast<QVector<Guild *> *>(guildsPtr);
            callback(guildsPtr);
        });
    else
        callback(reinterpret_cast<void *>(&guilds));
}

void RessourceManager::getGuildChannels(Callback callback, const Snowflake& id)
{
    requester->getGuildChannels([&, callback](void *guildChannelsPtr) {
        guildsChannels[id] = *reinterpret_cast<QVector<Channel *> *>(guildChannelsPtr);
        callback(guildChannelsPtr);
    }, id);
}

void RessourceManager::getGuildChannel(Callback callback, const Snowflake& guildId, const Snowflake& id)
{
    openedGuildsChannels[guildId][id];

    if (guildsChannels.find(guildId) == guildsChannels.end()) {
        requester->getGuildChannels([&, callback](void *guildChannelsPtr) {
            guildsChannels[guildId] = *reinterpret_cast<QVector<Channel *> *>(guildChannelsPtr);
            callback(guildChannelsPtr);
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
        requester->getPrivateChannels([&, callback](void *privateChannelsPtr) {
            privateChannels = *reinterpret_cast<QVector<PrivateChannel *> *>(privateChannelsPtr);
            for (int i = 0 ; i < privateChannels.size() ; i++) {
                if (privateChannels[i]->id == id) callback(reinterpret_cast<void *>(privateChannels[i]));
            }
        });
        return;
    }
    for (int i = 0 ; i < privateChannels.size() ; i++) {
        if (privateChannels[i]->id == id) callback(reinterpret_cast<void *>(privateChannels[i]));
    }
}

void RessourceManager::getPrivateChannels(Callback callback)
{
    if (privateChannels.empty())
        requester->getPrivateChannels([&, callback](void *privateChannelsPtr) {
            privateChannels = *reinterpret_cast<QVector<PrivateChannel *> *>(privateChannelsPtr);
            callback(privateChannelsPtr);
        });
    else
        callback(reinterpret_cast<void *>(&privateChannels));
}

void RessourceManager::getMessages(Callback callback, const Snowflake& channelId, int limit, bool newMessages)
{
    bool found = false;
    for (auto it = openedGuildsChannels.begin() ; it != openedGuildsChannels.end() ; it++) {
        QMap<Snowflake, QVector<QVector<int>>> channels = it.value();
        if (channels.find(channelId) != channels.end()) {
            found = true;
            unsigned int size = messages[channelId].size();

            QVector<int> indexes;
            indexes.push_back(size);
            indexes.push_back(size + limit);
            channels[channelId].push_back(indexes);
            gw->sendGuildChannelOpened(channels, it.key(), true, true, true);
            break;
        }
    }
    if (!found) {
        gw->sendDMChannelOpened(channelId);
    }

    if (newMessages) {
        if (messages[channelId].size() >= 50)
            requester->getMessages([&, callback](void *messagesPtr) {
                QVector<Message *> messagesVector = *reinterpret_cast<QVector<Message *> *>(messagesPtr);
                if (messagesVector.size() > 0) {
                    for (int i = 0 ; i < messagesVector.size() ; i++)
                        messages[channelId].push_back(messagesVector[i]);
                    callback(messagesPtr);
                }
            }, channelId, messages[channelId].back()->id, limit);
    } else if (messages.find(channelId) == messages.end() || messages[channelId].size() == 0) {
        requester->getMessages([&, callback](void *messagesPtr) {
            messages[channelId] = *reinterpret_cast<QVector<Message *> *>(messagesPtr);
            callback(messagesPtr);
        }, channelId, 0, limit);
    } else if (messages[channelId].size() < 50) {
        callback(reinterpret_cast<void *>(&messages[channelId]));
    } else if (messages[channelId].size() < limit) {
        requester->getMessages([&, callback](void *messagesPtr) {
            QVector<Message *> messagesVector = *reinterpret_cast<QVector<Message *> *>(messagesPtr);
            for (int i = 0 ; i < messagesVector.size() ; i++)
                messages[channelId].push_back(messagesVector[i]);
            callback(reinterpret_cast<void *>(&messages[channelId]));
        }, channelId, messages[channelId].back()->id, limit - messages[channelId].size());
    } else {
        callback(reinterpret_cast<void *>(&messages[channelId]));
    }
}

void RessourceManager::getClient(Callback callback)
{
    if (client == nullptr)
        requester->getClient([&, callback](void *clientPtr) {
            client = reinterpret_cast<Client *>(clientPtr);

            callback(clientPtr);
        });
    else
        callback(reinterpret_cast<void *>(client));
}

void RessourceManager::getClientSettings(Callback callback)
{
    if (clientSettings == nullptr)
        requester->getClientSettings([&, callback](void *clientSettingsPtr) {
            clientSettings = reinterpret_cast<ClientSettings *>(clientSettingsPtr);
            callback(clientSettingsPtr);
       });
    else
        callback(reinterpret_cast<void *>(clientSettings));
}

void RessourceManager::getImage(Callback callback, const QString& url, const QString& fileName)
{
    if (!QFile::exists("cache/" + fileName)) {
        requester->getImage(callback, url, fileName);}
    else {
        QString path = "cache/" + fileName;
        callback(reinterpret_cast<void *>(const_cast<QString *>(&path)));
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
    requester->getUser([&, callback](void *userPtr) {
       users.push_back(reinterpret_cast<User *>(userPtr));
       callback(userPtr);
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
