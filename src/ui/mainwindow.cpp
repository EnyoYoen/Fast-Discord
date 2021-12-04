#include "ui/mainwindow.h"

#include "api/request.h"

#include <QInputDialog>
#include <QLineEdit>

#include <thread>

namespace Ui {

MainWindow::MainWindow() : QWidget()
{
    // Style the window
    // this->setWindowFlags(Qt::CustomizeWindowHint);
    this->setGeometry(0, 0, 940, 728);
    this->setStyleSheet("background-color: #202225;"
                        "padding: 0px;"
                        "border: none;");

    // Ask the token to the user
    Api::Request::token = QInputDialog::getText(nullptr, "Token", "Enter your Discord token", QLineEdit::Normal, QString(), nullptr).toUtf8().constData();

    // Start the request loop
    Api::Request::startLoop();

    // Connect the signal for the setup
    QObject::connect(this, SIGNAL(clientSettingsRecieved()), this, SLOT(setup()));

    // Get user settings
    Api::Request::getClient([this](void *clientp){
        client = static_cast<Api::Client *>(clientp);
    });
    Api::Request::getClientSettings([this](void *clientSettingsp){
        clientSettings = static_cast<Api::ClientSettings *>(clientSettingsp);
        emit clientSettingsRecieved();
    });
}

void MainWindow::setup()
{
    // Create the UI
    setupInterface();

    // Set the gateway event callback
    gw.onDispatch([&](std::string& eventName, json& data){gatewayDispatchHandler(eventName, data);});
}

void MainWindow::setupInterface()
{
    // Create all the widgets
    mainLayout = new QHBoxLayout(this);
    leftColumn = new LeftColumn(this);
    middleColumn = new MiddleColumn(client, this);
    rightColumn = new RightColumn(client, this);

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
    QObject::connect(this, SIGNAL(messageRecieved(Api::Message)), rightColumn, SLOT(addMessage(Api::Message)));
}

void MainWindow::gatewayDispatchHandler(std::string& eventName, json& data)
{
    // Process gateway events
    if (eventName == "MESSAGE_CREATE") {
        // We recieved a message
        Api::Message *message;
        Api::unmarshal<Api::Message>(data, "", &message);
        emit messageRecieved(*message);
    } else if (eventName == "TYPING_START") {
        // Someone is typing
        std::thread typingThread = std::thread(&RightColumn::userTyping, rightColumn, data);
        typingThread.detach();
    }
}

} // namespace Ui
