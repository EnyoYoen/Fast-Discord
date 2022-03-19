#pragma once

#include "api/gateway.h"
#include "api/request.h"
#include "api/jsonutils.h"
#include "api/objects/presence.h"
#include "api/objects/client.h"

#include <QObject>
#include <QVector>
#include <QMap>

namespace Api {

class RessourceManager : public QObject
{
    Q_OBJECT
public:
    RessourceManager(const QString& token);
    ~RessourceManager();

    void getGuilds(const std::function<void(void *)>& callback);
    void getGuildChannel(const std::function<void(void *)>& callback, const Snowflake& guildId, const Snowflake& id);
    void getGuildChannels(const std::function<void(void *)>& callback, const Snowflake& id);
    void getPrivateChannel(const std::function<void(void *)>& callback, const Snowflake& id);
    void getPrivateChannels(const std::function<void(void *)>& callback);
    void getMessages(const std::function<void(void *)>& callback, const Snowflake& channelId, unsigned int limit, bool newMessages);
    void getClient(const std::function<void(void *)>& callback);
    void getClientSettings(const std::function<void(void *)>& callback);
    void getImage(const std::function<void(void *)>& callback, const QString& url, const QString& fileName);
    void getUser(const std::function<void(void *)>& callback, const Snowflake& userId);
    void getPresences(const std::function<void(void *)>& callback);

    QVector<Api::Message *> const getAllMessages(const Snowflake& channelId);
    bool const hasMessages(const Snowflake& channelId);
    
    Requester *requester;

signals:
    void typingReceived();
    void guildsReceived(const QVector<Api::Guild *>&);
    void presencesReceived(const QVector<Api::Presence *>&);
    void privateChannelsReceived(const QVector<Api::PrivateChannel *>&);
    void unreadUpdateReceived(const Api::Snowflake&);
    void presenceReceived(const Api::Presence&);
    void messageReceived(const Api::Message&);
    void channelCreated(const Api::Channel *, const Api::PrivateChannel *);
    void channelUpdated(const Api::Channel *, const Api::PrivateChannel *);
    void channelDeleted(const Api::Snowflake&, const Api::Snowflake&, int);

private:
    void gatewayDispatchHandler(QString& eventName, json& data);
        // Event handler for the gateway

    Gateway *gw;

    QMap<Snowflake, QVector<Message *>> messages;
    QMap<Snowflake, QVector<Channel *>> guildsChannels;

    QVector<User *>           users;
    QVector<Guild *>          guilds;
    QVector<PrivateChannel *> privateChannels;
    QVector<Presence *>       presences;

    QVector<Snowflake> openedDMChannels;
    QMap<Snowflake, QMap<Snowflake, QVector<QVector<int>>>> openedGuildsChannels;
    //       ^ guild ID  ^ channels  ^ channel ID  ^ indexes of messages that we have

    Client         *client = nullptr;
    ClientSettings *clientSettings = nullptr;
};

} // namespace Api
