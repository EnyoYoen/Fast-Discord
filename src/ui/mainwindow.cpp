#include "../../include/ui/mainwindow.h"

#include "../../include/ui/messagewidget.h"
#include "../../include/ui/messagetextinput.h"
#include "../../include/api/request.h"

#include <QInputDialog>
#include <QScrollBar>
#include <QLineEdit>
#include <QIcon>
#include <QDateTime>

#include <ctime>
#include <thread>
#include <chrono>

namespace Ui {

MainWindow::MainWindow(QWidget *parent) : QWidget(parent)
{
    setup();
}

void MainWindow::cleanRightColumn()
{
    QLayoutItem *item;
    while ((item = rightColumnLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }
}

void MainWindow::openPrivateChannel(Api::Channel& channel, unsigned int id)
{
    cleanRightColumn();

    for (size_t i = 0 ; i < privateChannelWidgets.size() ; i++) {
        if (i != id) {
            privateChannelWidgets[i]->unclicked();
        }
    }

    std::string channelId = *channel.id;
    std::vector<Api::Message *> *messages;

    std::map<std::string, std::vector<Api::Message *> *>::iterator currentMessages = channelsMessages.find(channelId);
    if (currentMessages == channelsMessages.end() or currentMessages->second->size() < 50) {
        messages = Api::Request::getMessages(channelId, 50 - currentMessages->second->size());
        channelsMessages[channelId] = messages;
    } else {
        messages = channelsMessages[channelId];
    }

    currentOpenedChannel = channelId;

    messageArea = new MessageArea(*messages);

    QWidget *messagesContainer = new QWidget();
    QVBoxLayout *messagesLayout = new QVBoxLayout();
    QGroupBox *inputBox = new QGroupBox();
    QHBoxLayout *inputLayout = new QHBoxLayout();
    MessageTextInput *textInput = new MessageTextInput();

    inputLayout->addWidget(textInput);
    inputLayout->setSpacing(0);
    inputBox->setLayout(inputLayout);
    inputBox->setStyleSheet("background-color: #40444B;"
                            "height: 44px;"
                            "border-radius: 8px;");

    typingLabel = new QLabel();
    typingLabel->setText("");
    typingLabel->setStyleSheet("height: 24px;"
                               "color: #DCDDDE");

    messagesLayout->addWidget(messageArea);
    messagesLayout->addWidget(inputBox);
    messagesLayout->addWidget(typingLabel);
    messagesLayout->setSpacing(0);
    messagesLayout->setContentsMargins(0, 0, 0, 0);

    messagesContainer->setLayout(messagesLayout);
    rightColumnLayout->addWidget(messagesContainer);
    rightColumnLayout->setContentsMargins(0, 0, 0, 0);

    QObject::connect(textInput, SIGNAL(returnPressed(std::string)), this, SLOT(sendMessage(const std::string&)));
    QObject::connect(textInput, SIGNAL(typing()), this, SLOT(sendTyping()));
}

void MainWindow::openGuildChannel(Api::Channel& channel, unsigned int id)
{
    for (size_t i = 0 ; i < guildChannelWidgets.size() ; i++) {
        if (i != id) {
            guildChannelWidgets[i]->unclicked();
        }
    }

    if (channel.type != 2) {
        cleanRightColumn();

        std::string channelId = *channel.id;
        std::vector<Api::Message *> *messages;

        std::map<std::string, std::vector<Api::Message *> *>::iterator currentMessages = channelsMessages.find(channelId);
        if (currentMessages == channelsMessages.end() or currentMessages->second->size() < 50) {
            messages = Api::Request::getMessages(channelId, 50 - currentMessages->second->size());
            channelsMessages[channelId] = messages;
        } else {
            messages = channelsMessages[channelId];
        }

        currentOpenedChannel = channelId;

        messageArea = new MessageArea(*messages);

        QWidget *messagesContainer = new QWidget();
        QScrollArea *userList = new QScrollArea();
        QVBoxLayout *userListLayout = new QVBoxLayout();
        QVBoxLayout *messagesLayout = new QVBoxLayout();
        QGroupBox *inputBox = new QGroupBox();
        QHBoxLayout *inputLayout = new QHBoxLayout();
        MessageTextInput *textInput = new MessageTextInput();

        inputLayout->addWidget(textInput);
        inputLayout->setSpacing(0);
        inputBox->setLayout(inputLayout);
        inputBox->setStyleSheet("background-color: #40444B;"
                                "height: 44px;"
                                "border-radius: 8px;");

        typingLabel = new QLabel();
        typingLabel->setText("");
        typingLabel->setStyleSheet("height: 24px;"
                                   "color: #DCDDDE");

        messagesLayout->addWidget(messageArea);
        messagesLayout->addWidget(inputBox);
        messagesLayout->addWidget(typingLabel);
        messagesLayout->setSpacing(0);
        messagesLayout->setContentsMargins(0, 0, 0, 0);

        messagesContainer->setLayout(messagesLayout);
        userList->setLayout(userListLayout);

        userList->setFixedWidth(240);
        userList->setStyleSheet("border: none;"
                                "background-color: #2F3136");

        rightColumnLayout->addWidget(messagesContainer);
        rightColumnLayout->addWidget(userList);

        rightColumnLayout->setSpacing(0);
        rightColumnLayout->setContentsMargins(0, 0, 0, 0);

        QObject::connect(textInput, SIGNAL(returnPressed(std::string)), this, SLOT(sendMessage(const std::string&)));
        QObject::connect(textInput, SIGNAL(typing()), this, SLOT(sendTyping()));
    }
}

void MainWindow::openGuild(Api::Guild& guild, unsigned int id)
{
    cleanRightColumn();

    homePageShown = false;
    homeButton->unclicked();
    guildChannelWidgets.clear();

    for (size_t i = 0 ; i < guildWidgets.size() ; i++) {
        if (i != id) {
            guildWidgets[i]->unclicked();
        }
    }

    middleChannelList->takeWidget();
    QWidget *guildChannelList = new QWidget();
    QVBoxLayout *guildChannelListLayout = new QVBoxLayout();

    std::vector<Api::Channel *> channels = *Api::Request::getGuildChannels(*guild.id);

    size_t channelsLen = channels.size();
    unsigned int count;
    for (size_t i = 0 ; i < channelsLen ; i++) {
        if ((*channels[i]).type == 4) {
            GuildChannelWidget *channelWidget = new GuildChannelWidget(*channels[i], count);
            guildChannelListLayout->addWidget(channelWidget);
            guildChannelWidgets.push_back(channelWidget);
            count++;
            for (size_t j = 0 ; j < channelsLen ; j++) {
                if ((*channels[j]).parentId == nullptr) continue;
                if (*(*channels[j]).parentId == *(*channels[i]).id) {
                    GuildChannelWidget *channelWidget = new GuildChannelWidget(*channels[j], count);
                    count++;
                    guildChannelWidgets.push_back(channelWidget);
                    QObject::connect(channelWidget, SIGNAL(leftClicked(Api::Channel&, unsigned int)), this, SLOT(openGuildChannel(Api::Channel&, unsigned int)));
                    guildChannelListLayout->addWidget(channelWidget);
                }
            }
        }
    }

    guildChannelListLayout->insertStretch(-1, 1);
    guildChannelList->setLayout(guildChannelListLayout);
    middleChannelList->setWidget(guildChannelList);

    middleChannelList->setStyleSheet("* {background-color: #2f3136; border: none;}"
                                     "QScrollBar::handle:vertical {border: none; border-radius: 2px; background-color: #202225;}"
                                     "QScrollBar:vertical {border: none; background-color: #2F3136; border-radius: 8px; width: 3px;}"
                                     "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {border:none; background: none; height: 0;}");
}

void MainWindow::addMessage(Api::Message message)
{
    std::string channelId = *message.channelId;
    if (channelsMessages.find(channelId) == channelsMessages.end()) {
        std::vector<Api::Message *> messageVector;
        channelsMessages[channelId] = &messageVector;
    }
    channelsMessages[channelId]->push_back(&message);

    if (channelId == currentOpenedChannel) {
        std::vector<Api::Message *> channelMessages = *channelsMessages[currentOpenedChannel];
        messageArea->addMessage(message, *(*channelsMessages[currentOpenedChannel])[0]);
    }
}

void MainWindow::sendMessage(const std::string& content)
{
    Api::Request::sendMessage(content, currentOpenedChannel);
    std::string messageTimestamp = QDateTime::currentDateTime().toString(Qt::ISODateWithMs).toUtf8().constData();
    Api::Message newMessage = Api::Message {nullptr, new Api::User{client->username, nullptr, client->avatar, nullptr, nullptr, client->id, -1, -1, -1, false, false, false, false}, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, const_cast<std::string *>(&content), &messageTimestamp, nullptr, nullptr, nullptr, nullptr, -1, -1, -1, -1, false, false, false};
    messageArea->addMessage(newMessage, *(*channelsMessages[currentOpenedChannel])[0]);
}

void MainWindow::sendTyping()
{
    Api::Request::sendTyping(currentOpenedChannel);
}

void MainWindow::displayPrivateChannels()
{
    if (!homePageShown) {
        cleanRightColumn();

        homePageShown = true;

        for (size_t i = 0 ; i < guildWidgets.size() ; i++) {
            guildWidgets[i]->unclicked();
        }

        privateChannels = Api::Request::getPrivateChannels();

        QWidget *privateChannelList = new QWidget();
        QVBoxLayout *privateChannelListLayout = new QVBoxLayout();

        for (unsigned int i = 0 ; i < privateChannels->size() ; i++) {
            PrivateChannel *privateChannel = new PrivateChannel(*(*privateChannels)[i], i);
            privateChannelWidgets.push_back(privateChannel);
            privateChannelListLayout->insertWidget(i, privateChannel);
            QObject::connect(privateChannel, SIGNAL(leftClicked(Api::Channel&, unsigned int)), this, SLOT(openPrivateChannel(Api::Channel&, unsigned int)));
        }

        privateChannelListLayout->insertStretch(-1, 1);
        privateChannelList->setLayout(privateChannelListLayout);
        middleChannelList->setWidget(privateChannelList);
    }
}

void MainWindow::displayGuilds()
{
    guilds = Api::Request::getGuilds();
    for (size_t i = 0 ; i < guilds->size() ; i++) {
        GuildWidget *guildWidget = new GuildWidget(*(*guilds)[i], i);
        guildWidgets.push_back(guildWidget);
        leftColumnLayout->insertWidget(i + 2, guildWidget);
        leftColumnLayout->setAlignment(guildWidget, Qt::AlignHCenter);
        QObject::connect(guildWidget, SIGNAL(leftClicked(Api::Guild&, unsigned int)), this, SLOT(openGuild(Api::Guild&, unsigned int)));
    }
}

void MainWindow::setupInterface()
{
    homePageShown = false;

    this->setGeometry(0, 0, 940, 728);

    mainLayout = new QHBoxLayout(this);
    leftColumn = new QScrollArea();
    middleColumn = new QWidget();
    rightColumn = new QGroupBox();

    leftColumn->setFixedWidth(72);
    middleColumn->setFixedWidth(240);

    leftColumnLayout = new QVBoxLayout();
    rightColumnLayout = new QHBoxLayout();
    rightColumn->setLayout(rightColumnLayout);

    home = new QGroupBox();
    home->setFixedSize(48, 48);
    homeLayout = new QVBoxLayout();
    homeButton = new HomeButton();
    homeButton->setFixedSize(48, 48);

    homeLayout->addWidget(homeButton);
    homeLayout->setEnabled(false);

    QWidget *guildSeparator = new QWidget();
    guildSeparator->setFixedSize(32, 2);
    guildSeparator->setStyleSheet("background-color: #444649;");

    home->setLayout(homeLayout);
    leftColumnLayout->addWidget(home);
    leftColumnLayout->addWidget(guildSeparator);
    leftColumnLayout->insertStretch(-1, 100);

    leftColumnLayout->setAlignment(home, Qt::AlignHCenter);
    leftColumnLayout->setAlignment(guildSeparator, Qt::AlignHCenter);

    leftColumnLayout->setSpacing(6);
    leftColumnLayout->setContentsMargins(0, 0, 0, 0);
    leftColumn->setLayout(leftColumnLayout);


    middleColumnLayout = new QVBoxLayout();
    middleChannelList = new QScrollArea();
    QWidget *userMenu = new QWidget();

    middleChannelList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    QHBoxLayout *userMenuLayout = new QHBoxLayout();

    std::string channelIconFileName;
    if (client->avatar == nullptr) {
        channelIconFileName = "res/images/png/user-icon-asset.png";
    } else {
        channelIconFileName = *client->id + (client->avatar->rfind("a_") == 0 ? ".gif" : ".webp");
        if (!std::ifstream(("cache/" + channelIconFileName).c_str()).good()) {
            Api::Request::requestFile("https://cdn.discordapp.com/avatars/" + *client->id + "/" + *client->avatar, "cache/" + channelIconFileName);
        }
        channelIconFileName = "cache/" + channelIconFileName;
    }

    QWidget *userInfos = new QWidget();
    QVBoxLayout *userInfosLayout = new QVBoxLayout();
    QLabel *name = new QLabel((*client->username).c_str());
    name->setFixedSize(84, 18);
    name->setStyleSheet("color: #DCDDDE;");
    QLabel *discriminator = new QLabel(("#" + *client->discriminator).c_str());
    discriminator->setFixedSize(84, 13);
    discriminator->setStyleSheet("color: #B9BBBE;");
    userInfosLayout->addWidget(name);
    userInfosLayout->addWidget(discriminator);
    userInfosLayout->setContentsMargins(0, 10, 0, 10);
    userInfos->setLayout(userInfosLayout);

    userMenuLayout->addWidget(new RoundedImage(channelIconFileName, 32, 32, 16));
    userMenuLayout->addWidget(userInfos);
    userMenuLayout->setContentsMargins(8, 0, 8, 0);

    userMenu->setFixedHeight(53);
    userMenu->setLayout(userMenuLayout);
    userMenu->setStyleSheet("background-color: #292B2F");

    middleColumnLayout->addWidget(middleChannelList);
    middleColumnLayout->addWidget(userMenu);
    middleColumnLayout->setContentsMargins(2, 0, 0, 0);

    middleColumn->setLayout(middleColumnLayout);


    mainLayout->addWidget(leftColumn);
    mainLayout->addWidget(middleColumn);
    mainLayout->addWidget(rightColumn);

    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    this->setLayout(mainLayout);

    this->setStyleSheet("background-color: #202225;"
                        "padding: 0px;"
                        "border: none;");
    leftColumn->setStyleSheet("background-color: #202225;"
                              "border: none;");
    middleColumn->setStyleSheet("background-color: #2F3136;"
                                "border: none;");
    rightColumn->setStyleSheet("background-color: #36393F;");

    QObject::connect(homeButton, SIGNAL(clicked()), this, SLOT(displayPrivateChannels()));
    QObject::connect(this, SIGNAL(messageRecieved(Api::Message)), this, SLOT(addMessage(Api::Message)));
}

void MainWindow::gatewayDispatchHandler(std::string& eventName, json& data)
{
    if (eventName == "MESSAGE_CREATE") {
        Api::Message message = *Api::getMessageFromJson(data, "");
        emit messageRecieved(message);
    } else if (eventName == "TYPING_START") {
        std::thread typingThread = std::thread(&MainWindow::userTyping, this, data);
        typingThread.detach();
    }
}

void MainWindow::userTyping(const json& data)
{
    // TODO : it's showing typing only for the current opened channel, other typing events are ignored
    std::string channelId = data.value("channel_id", "");
    if (currentOpenedChannel == channelId) {
        time_t currentTimestamp = std::time(nullptr);

        Api::MemoryStruct response;
        std::string url = std::string("https://discord.com/api/v9/users/") + data.value("user_id", "");
        Api::Request::requestJson(url, "", &response, "", "");

        std::string username = *Api::getUserFromJson(json::parse(response.memory), "")->username;
        long typingTimestamp = data.value("timestamp", -1);
        std::string text = typingLabel->text().toUtf8().constData();

        if (text != "") {
            size_t index = (text.find(" is typing") != std::string::npos) ? text.find(" is typing") : text.find(" are typing");
            text.resize(text.size() - index);
            text += " " + username + " are typing";
        } else {
            text = username + " is typing";
        }
        typingLabel->setText(QString::fromUtf8(text.c_str()));
        std::this_thread::sleep_for(std::chrono::seconds(typingTimestamp + 8 - currentTimestamp));
        typingLabel->setText("");
    }
}

void MainWindow::setupGateway()
{
    gw = Api::Gateway();
    gw.start();
    gw.onDispatch([&](std::string& eventName, json& data){gatewayDispatchHandler(eventName, data);});
}

void MainWindow::setup()
{
    client = Api::Request::getClient();
    clientSettings = Api::Request::getClientSettings();

    setupInterface();
    setupGateway();
    displayPrivateChannels();
    displayGuilds();
}

} // namespace Ui
