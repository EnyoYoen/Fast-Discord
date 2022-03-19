#pragma once

#include "ui/privatechannel.h"
#include "ui/guildchannelwidget.h"
#include "api/ressourcemanager.h"
#include "api/objects/client.h"
#include "api/objects/channel.h"
#include "api/objects/guild.h"
#include "api/objects/presence.h"

#include <QWidget>
#include <QVBoxLayout>
#include <QScrollArea>

namespace Ui {

// The middle column of the UI (with channels widgets)
class MiddleColumn : public QWidget
{
    Q_OBJECT
public:
    MiddleColumn(Api::RessourceManager *rm, const Api::Client *client, QWidget *parent);

signals:
    void guildChannelsReceived(const QVector<Api::Channel *> channels);
    void guildChannelClicked(const Api::Snowflake& guildId, const Api::Snowflake& id);
    void privateChannelClicked(const Api::Snowflake& id);

public slots:
    void setPresences(const QVector<Api::Presence *>& presences);
    void setPrivateChannels(const QVector<Api::PrivateChannel *>& channels);
    void updatePresence(const Api::Presence& presence);
    void displayPrivateChannels();
    void openGuild(const Api::Snowflake& id);
    void updateChannel(const Api::Channel *channel, const Api::PrivateChannel *privateChannel);
    void createChannel(const Api::Channel *channel, const Api::PrivateChannel *privateChannel);
    void deleteChannel(const Api::Snowflake& id, const Api::Snowflake& guildId, int type);
    void putChannelFirst(const Api::Snowflake& id);

private slots:
    void setGuildChannels(const QVector<Api::Channel *>& channels);
    void clicGuildChannel(const Api::Snowflake& id);
    void clicPrivateChannel(const Api::Snowflake& id);

private:
    // Main widgets
    QVBoxLayout *layout;
    QScrollArea *channelList;

    Api::RessourceManager *rm; // To request the API

    // Storing channels that we already gathered
    QVector<PrivateChannelWidget *> privateChannelWidgets;
    QVector<GuildChannelWidget *>   guildChannelWidgets;

    Api::Snowflake openedGuildId;
};

} // namespace Ui
