#include "ui/mainwindow.h"

#include "api/request.h"

#include <QThread>
#include <QInputDialog>
#include <QLineEdit>

namespace Ui {

MainWindow::MainWindow() : QWidget()
{
    // Style the window
    // this->setWindowFlags(Qt::CustomizeWindowHint); Soon
    this->setGeometry(0, 0, 940, 728);
    this->setStyleSheet("background-color: #202225;"
                        "padding: 0px;"
                        "border: none;");

    // Create the requester and the gateway
    std::string token = QInputDialog::getText(nullptr, "Token", "Enter your Discord token", QLineEdit::Normal, QString(), nullptr).toUtf8().constData();
    requester = new Api::Requester(token);
    gw = new Api::Gateway(requester, token);

    // Connect the signal for the setup
    QObject::connect(this, SIGNAL(clientSettingsReceived()), this, SLOT(setup()));

    // Get user settings
    requester->getClient([this](void *clientp){
        client = static_cast<Api::Client *>(clientp);
    });
    requester->getClientSettings([this](void *clientSettingsp){
        clientSettings = static_cast<Api::ClientSettings *>(clientSettingsp);
        emit clientSettingsReceived();
    });
}

void MainWindow::setup()
{
    // Create the UI
    setupInterface();

    // Set the gateway event callback
    gw->onDispatch([&](std::string& eventName, json& data){gatewayDispatchHandler(eventName, data);});
}

void MainWindow::setupInterface()
{
    // Create all the widgets
    mainLayout = new QHBoxLayout(this);
    leftColumn = new LeftColumn(requester, this);
    middleColumn = new MiddleColumn(requester, client, this);
    rightColumn = new RightColumn(requester, client, this);

    // Add the column to the layout
    mainLayout->addWidget(leftColumn);
    mainLayout->addWidget(middleColumn);
    mainLayout->addWidget(rightColumn);

    // Style the layout
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    // Connect signals to slots of the columns
    QObject::connect(leftColumn, SIGNAL(guildClicked(Api::Guild&)), middleColumn, SLOT(openGuild(Api::Guild&)));
    QObject::connect(leftColumn, SIGNAL(homeButtonClicked()), middleColumn, SLOT(displayPrivateChannels()));
    QObject::connect(leftColumn, SIGNAL(cleanRightColumn()), rightColumn, SLOT(clean()));
    QObject::connect(middleColumn, SIGNAL(channelClicked(Api::Channel&)), rightColumn, SLOT(openChannel(Api::Channel&)));
    QObject::connect(this, SIGNAL(unreadUpdateReceived(const std::string&)), leftColumn, SLOT(setUnreadGuild(const std::string&)));
    QObject::connect(this, SIGNAL(messageReceived(Api::Message)), rightColumn, SLOT(addMessage(Api::Message)));
}

void MainWindow::gatewayDispatchHandler(std::string& eventName, json& data)
{
    // Process gateway events
    if (eventName == "READY") {
        std::vector<Api::Guild *> *guilds;
        Api::unmarshalMultiple<Api::Guild>(data.at("guilds"), "", &guilds);
        leftColumn->displayGuilds(guilds);
    } else if (eventName == "CHANNEL_UNREAD_UPDATE") {
        emit unreadUpdateReceived(data.value("guild_id", ""));
    } else if (eventName == "MESSAGE_CREATE") {
        // We recieved a message
        Api::Message *message;
        Api::unmarshal<Api::Message>(data, "", &message);
        emit messageReceived(*message);
    } else if (eventName == "TYPING_START") {
        // Someone is typing
        gatewayData = data;
        QThread *typingThread = QThread::create([this](){rightColumn->userTyping(gatewayData);});
        typingThread->start();
    }
}

} // namespace Ui
