#include "ui/middlecolumn.h"

#include "ui/usermenu.h"

#include <algorithm>
#include <fstream>

namespace Ui {

MiddleColumn::MiddleColumn(Api::RessourceManager *rmp, const Api::Client *client, QWidget *parent)
    : QWidget(parent)
{
    // Set the requester
    rm = rmp;

    // Create the layout
    layout = new QVBoxLayout(this);

    // Create and style the channel list
    channelList = new QScrollArea(this);
    channelList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // Add the widget and style the main layout
    layout->addWidget(channelList);
    layout->addWidget(new UserMenu(rm, client, this));
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);

    QObject::connect(this, SIGNAL(guildChannelsReceived(const QVector<Api::Channel *>)), this, SLOT(setGuildChannels(const QVector<Api::Channel *>)));

    // Style this column
    this->setFixedWidth(240);
    this->setStyleSheet("background-color: #2F3136;"
                        "border: none;");
}

void MiddleColumn::setPresences(const QVector<Api::Presence *>& presences)
{
    rm->getPrivateChannels([&](const void *channelsPtr){
        const QVector<Api::PrivateChannel *> *privateChannels = reinterpret_cast<const QVector<Api::PrivateChannel *> *>(channelsPtr);
        for (unsigned int i = 0 ; i < presences.size() ; i++) {
            bool found = false;
            for (unsigned int j = 0 ; j < privateChannels->size() ; j++) {
                if ((*privateChannels)[j]->type == Api::DM && ((*privateChannels)[j]->recipientIds)[0] == presences[i]->userId) {
                    privateChannelWidgets[j]->setStatus(presences[i]->status);
                    found = true;
                    break;
                }
            }
            if (found) continue;
        }
    });
}

void MiddleColumn::updatePresence(const Api::Presence& presence)
{
    rm->getPrivateChannels([&](const void *channelsPtr){
        const QVector<Api::PrivateChannel *> *privateChannels = reinterpret_cast<const QVector<Api::PrivateChannel *> *>(channelsPtr);
        for (unsigned int i = 0 ; i < privateChannels->size() ; i++) {
            if ((*privateChannels)[i]->type == Api::DM && ((*privateChannels)[i]->recipientIds)[0] == presence.user->id) {
                privateChannelWidgets[i]->setStatus(presence.status);
                break;
            }
        }
    });
}

void MiddleColumn::setPrivateChannels(QVector<Api::PrivateChannel *> privateChannels)
{
    // Create the widgets
    QWidget *privateChannelList = new QWidget(this);
    QVBoxLayout *privateChannelListLayout = new QVBoxLayout(privateChannelList);

    // Create and display the private channels
    for (unsigned int i = 0 ; i < privateChannels.size() ; i++) {
        PrivateChannelWidget *privateChannelWidget = new PrivateChannelWidget(rm, *privateChannels[i], privateChannelList);
        privateChannelWidgets.push_back(privateChannelWidget);
        privateChannelListLayout->insertWidget(i, privateChannelWidget);
        QObject::connect(privateChannelWidget, SIGNAL(leftClicked(const Api::Snowflake&)), this, SLOT(clicPrivateChannel(const Api::Snowflake&)));
        QObject::connect(privateChannelWidget, SIGNAL(closeButtonClicked(const Api::Snowflake&, const Api::Snowflake&, int)), this, SLOT(deleteChannel(const Api::Snowflake&, const Api::Snowflake&, int)));
    }
    privateChannelListLayout->insertStretch(-1, 1);
    privateChannelListLayout->setSpacing(2);
    privateChannelListLayout->setContentsMargins(8, 8, 8, 0);

    // Set the channels to the column
    channelList->setWidget(privateChannelList);
    channelList->setStyleSheet("* {background-color: #2f3136; border: none;}"
                               "QScrollBar::handle:vertical {border: none; border-radius: 2px; background-color: #202225;}"
                               "QScrollBar:vertical {border: none; background-color: #2F3136; border-radius: 8px; width: 3px;}"
                               "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {border:none; background: none; height: 0;}");
}

void MiddleColumn::setGuildChannels(const QVector<Api::Channel *> channels)
{
    // Clear the whannels
    guildChannelWidgets.clear();
    channelList->takeWidget();

    // Create widgets
    QWidget *guildChannelList = new QWidget(this);
    QVBoxLayout *guildChannelListLayout = new QVBoxLayout(guildChannelList);

    // Create the channels widgets

    size_t channelsLen = channels.size();
    unsigned int count = 0; // For the IDs of the channels
    // Loop to find channel that are not in a category
    for (size_t i = 0 ; i < channelsLen ; i++) {
        if (channels[i]->type != Api::GuildCategory && channels[i]->parentId == 0) {
            // Create and add the channel widget to the list
            GuildChannelWidget *channelWidget = new GuildChannelWidget(*channels[i], guildChannelList);
            guildChannelListLayout->addWidget(channelWidget);
            guildChannelWidgets.push_back(channelWidget);
            // Connect the clicked signal to open the channel
            QObject::connect(channelWidget, SIGNAL(leftClicked(const Api::Snowflake&)), this, SLOT(clicGuildChannel(const Api::Snowflake&)));
            count++;
        }
    }
    // Loop through all channels to create widgets
    for (size_t i = 0 ; i < channelsLen ; i++) {
        if (channels[i]->type == Api::GuildCategory) {
            // Create the category channel channel widget
            GuildChannelWidget *channelWidget = new GuildChannelWidget(*channels[i], guildChannelList);
            guildChannelListLayout->addWidget(channelWidget);
            guildChannelWidgets.push_back(channelWidget);
            count++;
            // Loop another time to find channels belonging to this category
            for (size_t j = 0 ; j < channelsLen ; j++) {
                if (channels[j]->parentId == 0) { // Category or 'orphan' channel
                    if (channels[j]->type != Api::GuildCategory) {
                        // Connect the clicked signal to open the channel
                        QObject::connect(channelWidget, SIGNAL(leftClicked(const Api::Snowflake&)), this, SLOT(clicGuildChannel(const Api::Snowflake&)));
                    }
                }
                if (channels[j]->parentId == channels[i]->id) {
                    // This channel belongs to the category
                    // Create and add the channel widget
                    GuildChannelWidget *channelWidget = new GuildChannelWidget(*channels[j], guildChannelList);
                    guildChannelWidgets.push_back(channelWidget);
                    guildChannelListLayout->addWidget(channelWidget);
                    // Connect the clicked signal to open the channel
                    QObject::connect(channelWidget, SIGNAL(leftClicked(const Api::Snowflake&)), this, SLOT(clicGuildChannel(const Api::Snowflake&)));
                    count++;
                }
            }
        }
    }
    guildChannelListLayout->insertStretch(-1, 1);
    guildChannelListLayout->setSpacing(2);
    guildChannelListLayout->setContentsMargins(8, 8, 8, 8);

    // Style the channel list
    channelList->setWidget(guildChannelList);
    channelList->setStyleSheet("* {background-color: #2f3136; border: none;}"
                               "QScrollBar::handle:vertical {border: none; border-radius: 2px; background-color: #202225;}"
                               "QScrollBar:vertical {border: none; background-color: #2F3136; border-radius: 8px; width: 3px;}"
                               "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {border:none; background: none; height: 0;}");
}

void MiddleColumn::clicGuildChannel(const Api::Snowflake& id)
{
    // Reset the stylesheet of the channels except the one that we just clicked
    for (size_t i = 0 ; i < guildChannelWidgets.size() ; i++) {
        if (guildChannelWidgets[i]->id != id) {
            guildChannelWidgets[i]->unclicked();
        }
    }

    // Emit the signal to open the channel
    emit guildChannelClicked(openedGuildId, id);
}

void MiddleColumn::clicPrivateChannel(const Api::Snowflake& id)
{
    // Reset the stylesheet of the channels except the one that we just clicked
    for (size_t i = 0 ; i < privateChannelWidgets.size() ; i++) {
        if (privateChannelWidgets[i]->id != id) {
            privateChannelWidgets[i]->unclicked();
        }
    }

    // Emit the signal to open the channel
    emit privateChannelClicked(id);
}

void MiddleColumn::displayPrivateChannels()
{
    openedGuildId = 0;

    // Create the widgets
    QWidget *privateChannelList = new QWidget(this);
    QVBoxLayout *privateChannelListLayout = new QVBoxLayout(privateChannelList);

    // Display the private channels
    for (unsigned int i = 0 ; i < privateChannelWidgets.size() ; i++) {
        privateChannelListLayout->insertWidget(i, privateChannelWidgets[i]);
    }
    privateChannelListLayout->insertStretch(-1, 1);
    privateChannelListLayout->setSpacing(2);
    privateChannelListLayout->setContentsMargins(8, 8, 8, 0);

    // Set the channels to the column
    channelList->setWidget(privateChannelList);
}

void MiddleColumn::openGuild(const Api::Snowflake& guildId)
{
    openedGuildId = guildId;

    // Request the channels of the guild
    rm->getGuildChannels([this](const void *channels) {emit guildChannelsReceived(*reinterpret_cast<const QVector<Api::Channel *> *>(channels));}, guildId);
}

void MiddleColumn::updateChannel(const Api::Channel *channel, const Api::PrivateChannel *privateChannel)
{
    if (channel == nullptr) {
        for (auto it = privateChannelWidgets.begin() ; it != privateChannelWidgets.end() ; it++) {
            if (privateChannel->id == (*it)->id) {
                privateChannelWidgets.erase(it);
                privateChannelWidgets.insert(it, new PrivateChannelWidget(rm, *privateChannel, this));
                break;
            }
        }
        displayPrivateChannels();
    } else {
        if (openedGuildId == channel->guildId) 
            openGuild(channel->guildId);
    }
}

void MiddleColumn::createChannel(const Api::Channel *channel, const Api::PrivateChannel *privateChannel)
{
    if (channel == nullptr) {
        for (unsigned int i = 0 ; i < privateChannelWidgets.size() ; i++) {
            if (privateChannel->id == privateChannelWidgets[i]->id) {
                privateChannelWidgets.insert(privateChannelWidgets.begin(), new PrivateChannelWidget(rm, *privateChannel, this));
            }
        }
        displayPrivateChannels();
    } else {
        if (openedGuildId == channel->guildId) 
            openGuild(channel->guildId);
    }
}

void MiddleColumn::deleteChannel(const Api::Snowflake& id, const Api::Snowflake& guildId, int type)
{
    if (type == Api::DM || type == Api::GroupDM) {
        for (unsigned int i = 0 ; i < privateChannelWidgets.size() ; i++) {
            if (id == privateChannelWidgets[i]->id) {
                delete privateChannelWidgets[i];
                privateChannelWidgets.erase(privateChannelWidgets.begin() + i);
                displayPrivateChannels();
                break;
            }
        }
    } else {
        if (openedGuildId == guildId) 
            openGuild(guildId);
    }
}

void MiddleColumn::putChannelFirst(const Api::Snowflake& id)
{
    if (openedGuildId == 0) {
        for (auto it = privateChannelWidgets.begin() ; it != privateChannelWidgets.end() ; it++) {
            if (id == (*it)->id) {
                PrivateChannelWidget *tmp = *it;
                privateChannelWidgets.erase(it);
                privateChannelWidgets.insert(privateChannelWidgets.begin(), tmp);
                break;
            }
        }
        displayPrivateChannels();
    }
}

} // namespace Ui
