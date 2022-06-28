#pragma once

#include "ui/middlecolumn/privatechannel.h"
#include "ui/middlecolumn/guildchannelwidget.h"
#include "ui/middlecolumn/usermenu.h"
#include "ui/middlecolumn/callwidget.h"
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
class MiddleColumn : public Widget
{
    Q_OBJECT
public:
    MiddleColumn(Api::RessourceManager *rm, QWidget *parent);

signals:
    void guildChannelsReceived(const QVector<Api::Channel *> channels);
    void guildChannelClicked(const QString& channelName, const Api::Snowflake& guildId, const Api::Snowflake& id);
    void privateChannelClicked(const QString& channelName, const Api::Snowflake& id);
    void voiceChannelClicked(const Api::Snowflake& guildId, const Api::Snowflake& channelId, bool selfMute, bool selfDeaf);
    void parametersClicked();

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
    CallWidget  *callWidget;
    UserMenu    *userMenu;

    Api::RessourceManager *rm; // To request the API

    // Storing channels that we already gathered
    QVector<PrivateChannelWidget *> privateChannelWidgets;
    QVector<GuildChannelWidget *>   guildChannelWidgets;

    Api::Snowflake openedGuildId;
    Api::Snowflake callChannel;
};

} // namespace Ui
