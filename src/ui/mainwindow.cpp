#include "../../include/ui/mainwindow.h"

#include "../../include/ui/messagewidget.h"
#include "../../include/ui/messagetextinput.h"
#include "../../include/api/request.h"

#include <QInputDialog>
#include <QScrollBar>
#include <QLineEdit>
#include <QIcon>

#include <ctime>
#include <thread>
#include <chrono>

namespace Ui {

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    homePageShown = false;
    setup();

    QObject::connect(this, SIGNAL(messageRecieved(Api::Message)), this, SLOT(addMessage(Api::Message)));
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
    if (currentMessages == channelsMessages.end() or currentMessages->second->size() < 100) {
        messages = Api::Request::getMessages(channelId, 100 - currentMessages->second->size());
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
        if (currentMessages == channelsMessages.end() or currentMessages->second->size() < 100) {
            messages = Api::Request::getMessages(channelId, 100 - currentMessages->second->size());
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

    middleColumn->takeWidget();
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
    middleColumn->setWidget(guildChannelList);

    middleColumn->setStyleSheet("* {background-color: #2f3136; border: none;}"
                                "QScrollBar::handle:vertical {border: none; border-radius: 2px; background-color: #202225;}"
                                "QScrollBar:vertical {border: none; background-color: #2F3136; border-radius: 8px; width: 3px;}"
                                "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {border:none; background: none; height: 0;}");
}

void MainWindow::addMessage(Api::Message message)
{
    if (*message.author->username != static_cast<std::string>("Enyo")) {
        std::string channelId = *message.channelId;
        if (channelsMessages.find(channelId) == channelsMessages.end()) {
            std::vector<Api::Message *> messageVector;
            channelsMessages[channelId] = &messageVector;
        }
        channelsMessages[channelId]->push_back(&message);

        if (channelId == currentOpenedChannel) {
            messageArea->widget()->layout()->addWidget(new MessageWidget(message));
        }
    }
}

void MainWindow::sendMessage(const std::string& content)
{
    Api::Request::sendMessage(content, currentOpenedChannel);
    Api::Message message = Api::Message {nullptr, new Api::User{ new std::string("Todo"), new std::string("0000"), new std::string(""), nullptr, nullptr, new std::string(""), -1, -1, -1, false, false, false, false}, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, const_cast<std::string *>(&content), nullptr, nullptr, nullptr, nullptr, nullptr, -1, -1, -1, -1, false, false, false};
    messageArea->widget()->layout()->addWidget(new MessageWidget(message));
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
        middleColumn->setWidget(privateChannelList);
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
    this->setGeometry(0, 0, 940, 728);

    mainLayout = new QHBoxLayout(this);
    leftColumn = new QGroupBox();
    middleColumn = new QScrollArea();
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

    middleColumn->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    mainLayout->addWidget(leftColumn);
    mainLayout->addWidget(middleColumn);
    mainLayout->addWidget(rightColumn);

    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    this->setLayout(mainLayout);

    this->setStyleSheet("background-color: #202225;");
    leftColumn->setStyleSheet("background-color: #202225;");
    middleColumn->setStyleSheet("background-color: #2f3136;"
                                "border: none;");
    rightColumn->setStyleSheet("background-color: #36393f;");

    QObject::connect(homeButton, SIGNAL(clicked()), this, SLOT(displayPrivateChannels()));
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
    setupInterface();
    setupGateway();
    displayPrivateChannels();
    displayGuilds();
}

} // namespace Ui
