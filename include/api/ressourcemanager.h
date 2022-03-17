#pragma once

#include "gateway.h"
#include "request.h"
#include "jsonutils.h"
#include "presence.h"
#include "client.h"

#include <QObject>

#include <map>
#include <QVector>

namespace Api {

class RessourceManager : public QObject
{
    Q_OBJECT
public:
    RessourceManager(const QString& token);
    ~RessourceManager();

    void getGuilds(std::function<void(void *)> callback);
    void getGuildChannel(std::function<void(void *)> callback, const Snowflake& guildId, const Snowflake& id);
    void getGuildChannels(std::function<void(void *)> callback, const Snowflake& id);
    void getPrivateChannel(std::function<void(void *)> callback, const Snowflake& id);
    void getPrivateChannels(std::function<void(void *)> callback);
    void getMessages(std::function<void(void *)> callback, const Snowflake& channelId, unsigned int limit, bool newMessages);
    void getClient(std::function<void(void *)> callback);
    void getClientSettings(std::function<void(void *)> callback);
    void getImage(std::function<void(void *)> callback, const QString& url, const QString& fileName);
    void getUser(std::function<void(void *)> callback, const Snowflake& userId);
    void getPresences(std::function<void(void *)> callback);

    QVector<Api::Message *> getAllMessages(Snowflake& channelId);
    bool hasMessages(const Snowflake& channelId);
    
    Requester *requester;

signals:
    void typingReceived();
    void guildsReceived(const QVector<Api::Guild *>&);
    void presencesReceived(const QVector<Api::Presence *>&);
    void privateChannelsReceived(QVector<Api::PrivateChannel *>);
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

    std::map<Snowflake, QVector<Message *>> *messages;
    std::map<Snowflake, QVector<Channel *>> *guildsChannels;

    QVector<User *>           users;
    QVector<Guild *>          guilds;
    QVector<PrivateChannel *> privateChannels;
    QVector<Presence *>       presences;

    QVector<Snowflake> openedDMChannels;
    std::map<Snowflake, std::map<Snowflake, QVector<QVector<int>>>> openedGuildsChannels;
    //       ^ guild ID  ^ channels  ^ channel ID  ^ indexes of messages that we have

    Client         *client = nullptr;
    ClientSettings *clientSettings = nullptr;
};

} // namespace Api
