#pragma once

#include "privatechannel.h"
#include "guildchannelwidget.h"
#include "api/client.h"
#include "api/channel.h"
#include "api/guild.h"

#include <QWidget>
#include <QVBoxLayout>
#include <QScrollArea>

namespace Ui {

// The middle column of the UI (with channels widgets)
class MiddleColumn : public QWidget
{
    Q_OBJECT
public:
    MiddleColumn(const Api::Client *client, QWidget *parent);

signals:
    void channelClicked(Api::Channel& channel);

public slots:
    void openGuild(Api::Guild& guild);
    void displayPrivateChannels();

private slots:
    void clicChannel(Api::Channel& channel, unsigned int id);

private:
    // Main widgets
    QVBoxLayout *layout;
    QScrollArea *channelList;

    // Storing channels that we already gathered
    std::vector<Api::Channel *>      *privateChannels;
    std::vector<PrivateChannel *>     privateChannelWidgets;
    std::vector<GuildChannelWidget *> guildChannelWidgets;
};

} // namespace Ui
