#pragma once

#include "messagearea.h"
#include "guildwidget.h"
#include "privatechannel.h"
#include "homebutton.h"
#include "guildchannelwidget.h"
#include "../api/channel.h"
#include "../api/message.h"
#include "../api/gateway.h"
#include "../api/jsonutils.h"

#include <QWidget>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QScrollArea>

#include <string>
#include <vector>

namespace Ui {

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

signals:
    void messageRecieved(Api::Message);

private slots:
    void displayPrivateChannels();
    void displayGuilds();
    void openPrivateChannel(Api::Channel& channel, unsigned int);
    void openGuildChannel(Api::Channel& channel, unsigned int);
    void openGuild(Api::Guild& guild, unsigned int);
    void addMessage(Api::Message message);
    void sendMessage(const std::string& content);
    void sendTyping();

private:
    void setup();
    void setupInterface();
    void setupGateway(); //Creating and lauching the gateway
    void cleanRightColumn();
    void cleanMiddleColumn();
    void gatewayDispatchHandler(std::string& eventName, json& data); //Event handler for the gateway
    void userTyping(const json& data); //Used when we recieve TYPING_START event from the gateway

    //All the widget that need to be accessed from different functions
    QHBoxLayout *mainLayout;
    QGroupBox   *leftColumn;
    QScrollArea *middleColumn;
    QGroupBox   *rightColumn;
    QVBoxLayout *leftColumnLayout;
    QHBoxLayout *rightColumnLayout;
    QScrollArea *leftScrollArea;
    QWidget     *leftScrollAreaContent;
    QGroupBox   *home;
    QVBoxLayout *homeLayout;
    HomeButton  *homeButton;
    MessageArea *messageArea;
    QLabel      *typingLabel;

    Api::Gateway gw; //For the websocket connection

    //Storing channels and messages that we already gathered
    std::vector<Api::Channel *> *privateChannels;
    std::vector<PrivateChannel *> privateChannelWidgets;
    std::map<std::string, std::vector<Api::Message *> *> channelsMessages;

    //Store guilds
    std::vector<Api::Guild *>        *guilds;
    std::vector<GuildWidget *>        guildWidgets;
    std::vector<GuildChannelWidget *> guildChannelWidgets;

    std::string currentOpenedChannel; //Current channel ID
    bool homePageShown;
};

} // namespace Ui
