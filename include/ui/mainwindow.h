#pragma once

#include "messagearea.h"
#include "guildwidget.h"
#include "privatechannel.h"
#include "homebutton.h"
#include "guildchannelwidget.h"
#include "api/channel.h"
#include "api/message.h"
#include "api/gateway.h"
#include "api/client.h"
#include "api/jsonutils.h"

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
    QScrollArea *leftColumn;
    QWidget     *middleColumn;
    QWidget     *rightColumn;
    QVBoxLayout *leftColumnLayout;
    QVBoxLayout *middleColumnLayout;
    QHBoxLayout *rightColumnLayout;
    QScrollArea *leftScrollArea;
    QWidget     *leftScrollAreaContent;
    QWidget     *home;
    QVBoxLayout *homeLayout;
    HomeButton  *homeButton;
    QScrollArea *middleChannelList;
    MessageArea *messageArea;
    QLabel      *typingLabel;

    Api::Gateway gw; //For the websocket connection
    Api::Client *client;
    Api::ClientSettings *clientSettings;

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
