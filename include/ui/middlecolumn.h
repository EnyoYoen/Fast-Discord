#pragma once

#include "privatechannel.h"
#include "guildchannelwidget.h"
#include "api/client.h"
#include "api/channel.h"
#include "api/guild.h"
#include "api/ressourcemanager.h"
#include "api/presence.h"

#include <QWidget>
#include <QVBoxLayout>
#include <QScrollArea>

#include <vector>

namespace Ui {

// The middle column of the UI (with channels widgets)
class MiddleColumn : public QWidget
{
    Q_OBJECT
public:
    MiddleColumn(Api::RessourceManager *rm, const Api::Client *client, QWidget *parent);

signals:
    void guildChannelsReceived(const std::vector<Api::Channel *> *channels);
    void guildChannelClicked(const std::string& guildId, const std::string& id);
    void privateChannelClicked(const std::string& id);

public slots:
    void setPresences(const std::vector<Api::Presence *>& presences);
    void setPrivateChannels(const std::vector<Api::PrivateChannel *>& channels);
    void updatePresence(const Api::Presence& presence);
    void displayPrivateChannels();
    void openGuild(const std::string&);
    void updateChannel(const Api::Channel *channel, const Api::PrivateChannel *privateChannel);
    void createChannel(const Api::Channel *channel, const Api::PrivateChannel *privateChannel);
    void deleteChannel(const std::string& id, const std::string& guildId, int type);

private slots:
    void setGuildChannels(const std::vector<Api::Channel *> *channels);
    void clicGuildChannel(const std::string& id);
    void clicPrivateChannel(const std::string& id);

private:
    // Main widgets
    QVBoxLayout *layout;
    QScrollArea *channelList;

    Api::RessourceManager *rm; // To request the API

    // Storing channels that we already gathered
    std::vector<PrivateChannelWidget *> privateChannelWidgets;
    std::vector<GuildChannelWidget *>   guildChannelWidgets;

    std::string openedGuildId;
};

} // namespace Ui
