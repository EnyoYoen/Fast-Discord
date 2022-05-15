#include "ui/middlecolumn/middlecolumn.h"

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
    callWidget = new CallWidget(rm, this);
    layout->addWidget(callWidget);
    userMenu = new UserMenu(rm, client, this);
    layout->addWidget(userMenu);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);

    QObject::connect(userMenu, &UserMenu::parametersClicked, [this](){emit parametersClicked();});
    QObject::connect(userMenu, &UserMenu::voiceStateChanged, [this](bool mute, bool deaf){
        rm->vs->deaf = deaf;
        rm->vs->mute = mute;
        if (callChannel != 0)
            rm->gw->sendVoiceStateUpdate(openedGuildId, callChannel, mute, deaf);
    });
    QObject::connect(this, &MiddleColumn::guildChannelsReceived, this, &MiddleColumn::setGuildChannels);

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

void MiddleColumn::setPrivateChannels(const QVector<Api::PrivateChannel *>& privateChannels)
{
    // Create the widgets
    QWidget *privateChannelList = new QWidget(this);
    QVBoxLayout *privateChannelListLayout = new QVBoxLayout(privateChannelList);

    // Create and display the private channels
    for (unsigned int i = 0 ; i < privateChannels.size() ; i++) {
        PrivateChannelWidget *privateChannelWidget = new PrivateChannelWidget(rm, *privateChannels[i], privateChannelList);
        privateChannelWidgets.push_back(privateChannelWidget);
        privateChannelListLayout->insertWidget(i, privateChannelWidget);
        QObject::connect(privateChannelWidget, &PrivateChannelWidget::leftClicked, this, &MiddleColumn::clicPrivateChannel);
        QObject::connect(privateChannelWidget, &PrivateChannelWidget::closeButtonClicked, this, &MiddleColumn::deleteChannel);
    }
    privateChannelListLayout->insertStretch(-1, 1);
    privateChannelListLayout->setSpacing(0);
    privateChannelListLayout->setContentsMargins(8, 8, 8, 0);

    // Set the channels to the column
    channelList->setWidget(privateChannelList);
    channelList->setStyleSheet("* {background-color: #2f3136; border: none;}"
                               "QScrollBar::handle:vertical {border: none; border-radius: 2px; background-color: #202225;}"
                               "QScrollBar:vertical {border: none; background-color: #2F3136; border-radius: 8px; width: 3px;}"
                               "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {border:none; background: none; height: 0;}");
}

void MiddleColumn::setGuildChannels(const QVector<Api::Channel *>& channels)
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
            QObject::connect(channelWidget, &GuildChannelWidget::leftClicked, this, &MiddleColumn::clicGuildChannel);
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
                        QObject::connect(channelWidget, &GuildChannelWidget::leftClicked, this, &MiddleColumn::clicGuildChannel);
                    }
                }
                if (channels[j]->parentId == channels[i]->id) {
                    // This channel belongs to the category
                    // Create and add the channel widget
                    GuildChannelWidget *channelWidget = new GuildChannelWidget(*channels[j], guildChannelList);
                    guildChannelWidgets.push_back(channelWidget);
                    guildChannelListLayout->addWidget(channelWidget);
                    // Connect the clicked signal to open the channel
                    QObject::connect(channelWidget, &GuildChannelWidget::leftClicked, this, &MiddleColumn::clicGuildChannel);
                    count++;
                }
            }
        }
    }
    guildChannelListLayout->insertStretch(-1, 1);
    guildChannelListLayout->setSpacing(3);
    guildChannelListLayout->setContentsMargins(0, 8, 8, 8);

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
    QString name;
    for (size_t i = 0 ; i < guildChannelWidgets.size() ; i++) {
        if (guildChannelWidgets[i]->id != id) {
            guildChannelWidgets[i]->unclicked();
        } else {
            name = guildChannelWidgets[i]->name->text();
            if (guildChannelWidgets[i]->type == Api::GuildVoice) {
                if (callChannel != 0) {
                    rm->stopCall();
                    callWidget->hide();
                }
                emit voiceChannelClicked(openedGuildId, id, userMenu->deaf ? true : userMenu->deaf, userMenu->muted);
                callChannel = id;
                rm->getGuilds([this, name](void *guildsPtr){
                    QVector<Api::Guild *> guilds = *reinterpret_cast<QVector<Api::Guild *> *>(guildsPtr);
                    for (unsigned int i = 0 ; i < guilds.size() ; i++) {
                        if (guilds[i]->id == openedGuildId)
                            callWidget->call(name, guilds[i]->name);
                    }
                });
            } else {
                callChannel.clear();
            }
        }
    }

    // Emit the signal to open the channel
    emit guildChannelClicked(name, openedGuildId, id);
}

void MiddleColumn::clicPrivateChannel(const Api::Snowflake& id)
{
    // Reset the stylesheet of the channels except the one that we just clicked
    QString name;
    for (size_t i = 0 ; i < privateChannelWidgets.size() ; i++) {
        if (privateChannelWidgets[i]->id != id) {
            privateChannelWidgets[i]->unclicked();
        } else {
            name = privateChannelWidgets[i]->name->text();
            callChannel.clear();
        }
    }

    // Emit the signal to open the channel
    emit privateChannelClicked(name, id);
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
