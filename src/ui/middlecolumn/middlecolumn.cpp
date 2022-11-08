#include "ui/middlecolumn/middlecolumn.h"

#include <algorithm>

namespace Ui {

MiddleColumn::MiddleColumn(Api::RessourceManager *rmp, QWidget *parent)
    : Widget(parent)
{
    // Style this column
    this->setFixedWidth(Settings::scale(240));
    this->setBackgroundColor(Settings::BackgroundSecondary);
    
    // Set the requester
    rm = rmp;

    // Create the layout
    layout = new QVBoxLayout(this);

    // Create and style the channel list
    channelList = new QScrollArea(this);
    channelList->setStyleSheet("* {background-color:" + Settings::colors[Settings::BackgroundSecondary].name() + ";border:none;}");
    channelList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // Add the widget and style the main layout
    layout->addWidget(channelList);
    callWidget = new CallWidget(rm, this);
    layout->addWidget(callWidget);
    userMenu = new UserMenu(rm, this);
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
}

void MiddleColumn::setPresences(const QVector<Api::Presence *>& presences)
{
    rm->getPrivateChannels([&](Api::CallbackStruct cb){
        const QVector<Api::PrivateChannel *> *privateChannels = reinterpret_cast<const QVector<Api::PrivateChannel *> *>(cb.data);
        for (int i = 0 ; i < presences.size() ; i++) {
            bool found = false;
            for (int j = 0 ; j < privateChannels->size() ; j++) {
                if ((*privateChannels)[j]->type == Api::DM && ((*privateChannels)[j]->recipientIds)[0] == presences[i]->userId
                 && privateChannelWidgets.size() > 0) {
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
    rm->getPrivateChannels([&](Api::CallbackStruct cb){
        const QVector<Api::PrivateChannel *> *privateChannels = reinterpret_cast<const QVector<Api::PrivateChannel *> *>(cb.data);
        for (int i = 0 ; i < privateChannels->size() ; i++) {
            if ((*privateChannels)[i]->type == Api::DM && ((*privateChannels)[i]->recipientIds)[0] == presence.user->id
             && privateChannelWidgets.size() > 0) {
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
    for (int i = 0 ; i < privateChannels.size() ; i++) {
        PrivateChannelWidget *privateChannelWidget = new PrivateChannelWidget(rm, *privateChannels[i], privateChannelList);
        privateChannelWidgets.push_back(privateChannelWidget);
        privateChannelListLayout->insertWidget(i, privateChannelWidget);
        QObject::connect(privateChannelWidget, &PrivateChannelWidget::leftClicked, this, &MiddleColumn::clicPrivateChannel);
        QObject::connect(privateChannelWidget, &PrivateChannelWidget::closeButtonClicked, this, &MiddleColumn::deleteChannel);
    }
    privateChannelListLayout->insertStretch(-1, 1);
    privateChannelListLayout->setSpacing(Settings::scale(2));
    privateChannelListLayout->setContentsMargins(Settings::scale(8), Settings::scale(8), Settings::scale(8), 0);

    // Set the channels to the column
    channelList->setWidget(privateChannelList);
    channelList->setStyleSheet("* {background-color:" + Settings::colors[Settings::BackgroundSecondary].name() + "; border: none;}"
                               "QScrollBar::handle:vertical {border: none; border-radius: 2px; background-color: " + Settings::colors[Settings::BackgroundTertiary].name() + ";}"
                               "QScrollBar:vertical {border: none; background-color: " + Settings::colors[Settings::BackgroundSecondary].name() + "; border-radius: 8px; width: 3px;}"
                               "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {border:none; background: none; height: 0;}"
                               "QScrollBar:left-arrow:vertical, QScrollBar::right-arrow:vertical {background: none;}"
                               "QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {background: none;}");
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

    int channelsLen = channels.size();
    // Loop to find channel that are not in a category
    for (int i = 0 ; i < channelsLen ; i++) {
        if (channels[i]->type != Api::GuildCategory && channels[i]->parentId == 0) {
            // Create and add the channel widget to the list
            GuildChannelWidget *channelWidget = new GuildChannelWidget(*channels[i], guildChannelList);
            guildChannelListLayout->addWidget(channelWidget);
            guildChannelWidgets.push_back(channelWidget);
            // Connect the clicked signal to open the channel
            QObject::connect(channelWidget, &GuildChannelWidget::leftClicked, this, &MiddleColumn::clicGuildChannel);
        }
    }

    // Loop through all channels to create widgets
    QMap<Api::Snowflake, Api::Channel *> categoriesId;
    QMap<Api::Channel *, QVector<Api::Channel *>> categories;
    for (int i = 0 ; i < channelsLen ; i++) {
        if (channels[i]->type == Api::GuildCategory) {
            categoriesId[channels[i]->id] = channels[i];
            categories[channels[i]];
        }
    }
    for (int i = 0 ; i < channelsLen ; i++) {
        if (channels[i]->parentId != 0) {
            QVector<qint32> idxs;
            QVector<Api::Channel *> categoryChannels = categories[categoriesId[channels[i]->parentId]];
            for (int j = 0 ; j < categoryChannels.size() ; j++) {
                idxs.append(categoryChannels[j]->position);
            }
            idxs.append(channels[i]->position);
            std::sort(idxs.begin(), idxs.end());
            int idx = idxs.indexOf(channels[i]->position);
            categories[categoriesId[channels[i]->parentId]].insert(idx, channels[i]);
        }
    }

    int index = 0;
    QList<Api::Channel *> keys = categories.keys();
    QList<QVector<Api::Channel *>> values = categories.values();
    for (int i = 0 ; i < categories.count() ; i++) {
        for (int j = 0 ; j != categories.count() ; j++) {
            if (keys.at(j)->position == index) {
                GuildChannelWidget *channelWidget = new GuildChannelWidget(*keys.at(j), guildChannelList);
                guildChannelListLayout->addWidget(channelWidget);
                guildChannelWidgets.push_back(channelWidget);
                for (int k = 0 ; k < values.at(j).size() ; k++) {
                    GuildChannelWidget *channelWidget = new GuildChannelWidget(*values.at(j)[k], guildChannelList);
                    guildChannelListLayout->addWidget(channelWidget);
                    guildChannelWidgets.push_back(channelWidget);
                    QObject::connect(channelWidget, &GuildChannelWidget::leftClicked, this, &MiddleColumn::clicGuildChannel);
                }
            }
        }
        index++;
    }

    guildChannelListLayout->insertStretch(-1, 1);
    guildChannelListLayout->setSpacing(Settings::scale(3));
    guildChannelListLayout->setContentsMargins(0, Settings::scale(8), Settings::scale(8), Settings::scale(8));

    // Style the channel list
    channelList->setWidget(guildChannelList);
    channelList->setStyleSheet("* {background-color:" + Settings::colors[Settings::BackgroundSecondary].name() + "; border: none;}"
                               "QScrollBar::handle:vertical {border: none; border-radius: " + QString::number(Settings::scale(2)) + "px; background-color: " + Settings::colors[Settings::BackgroundTertiary].name() + ";}"
                               "QScrollBar:vertical {border: none; background-color: " + Settings::colors[Settings::BackgroundSecondary].name() + "; border-radius: " + QString::number(Settings::scale(8)) + "px; width: " + QString::number(Settings::scale(3)) + "px;}"
                               "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {border:none; background: none; height: 0;}"
                               "QScrollBar:left-arrow:vertical, QScrollBar::right-arrow:vertical {background: none;}"
                               "QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {background: none;}");
}

void MiddleColumn::clicGuildChannel(const Api::Snowflake& id)
{
    // Reset the stylesheet of the channels except the one that we just clicked
    QString name;
    for (int i = 0 ; i < guildChannelWidgets.size() ; i++) {
        if (guildChannelWidgets[i]->id != id) {
            guildChannelWidgets[i]->unclicked();
        } else {
            name = guildChannelWidgets[i]->name->text;
            if (guildChannelWidgets[i]->type == Api::GuildVoice) {
                if (callChannel != 0) {
                    rm->stopCall();
                    callWidget->hide();
                }
                emit voiceChannelClicked(openedGuildId, id, userMenu->deaf ? true : userMenu->deaf, userMenu->muted);
                callChannel = id;
                rm->getGuilds([this, name](Api::CallbackStruct cb){
                    QVector<Api::Guild *> guilds = *reinterpret_cast<QVector<Api::Guild *> *>(cb.data);
                    for (int i = 0 ; i < guilds.size() ; i++) {
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
    for (int i = 0 ; i < privateChannelWidgets.size() ; i++) {
        if (privateChannelWidgets[i]->id != id) {
            privateChannelWidgets[i]->unclicked();
        } else {
            name = privateChannelWidgets[i]->name->text;
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
    for (int i = 0 ; i < privateChannelWidgets.size() ; i++) {
        privateChannelListLayout->insertWidget(i, privateChannelWidgets[i]);
    }
    privateChannelListLayout->insertStretch(-1, 1);
    privateChannelListLayout->setSpacing(Settings::scale(2));
    privateChannelListLayout->setContentsMargins(Settings::scale(8), Settings::scale(8), Settings::scale(8), 0);

    // Set the channels to the column
    channelList->setWidget(privateChannelList);
}

void MiddleColumn::openGuild(const Api::Snowflake& guildId)
{
    openedGuildId = guildId;

    // Request the channels of the guild
    rm->getGuildChannels([this](Api::CallbackStruct cb) {emit guildChannelsReceived(*reinterpret_cast<const QVector<Api::Channel *> *>(cb.data));}, guildId);
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
        for (int i = 0 ; i < privateChannelWidgets.size() ; i++) {
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
        for (int i = 0 ; i < privateChannelWidgets.size() ; i++) {
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
