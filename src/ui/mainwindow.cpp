#include "ui/mainwindow.h"

#include "api/ressourcemanager.h"

#include <QThread>
#include <QInputDialog>
#include <QLineEdit>
#include <QSettings>

namespace Ui {

MainWindow::MainWindow() : QWidget()
{
    // Style the window
    // this->setWindowFlags(Qt::CustomizeWindowHint); Soon
    this->setGeometry(0, 0, 940, 728);
    this->setStyleSheet("background-color: #202225;"
                        "padding: 0px;"
                        "border: none;");

    // Get token from config if exists and get it from user input if not
    std::string token = getToken();

    // Create the ressource manager
    rm = new Api::RessourceManager(token);

    // Connect the signal for the setup
    QObject::connect(this, SIGNAL(clientSettingsReceived()), this, SLOT(setup()));

    // Get user settings
    rm->getClient([this](void *clientp){
        client = static_cast<Api::Client *>(clientp);
    });
    rm->getClientSettings([this](void *clientSettingsp){
        clientSettings = static_cast<Api::ClientSettings *>(clientSettingsp);
        emit clientSettingsReceived();
    });
}

std::string MainWindow::getToken() {
    QFile *file = new QFile("config.ini");
    std::string token = "";

    if (!file->exists()) {
        // Create config file if not exists
        QSettings* settings = new QSettings("config.ini", QSettings::IniFormat);

        token = QInputDialog::getText(nullptr, "Token", "Enter your Discord token", QLineEdit::Normal, QString(), nullptr).toUtf8().constData();
        token.erase(std::remove_if(token.begin(), token.end(), ::isspace), token.end());

        settings->setValue("token", token.c_str());
    }
    else {
        // Load config file and get token from it
        QSettings settings("config.ini", QSettings::IniFormat);
        token = settings.value("token", token.c_str()).toString().toStdString();
    }

    return token;
}

void MainWindow::setup()
{
    // Create all the widgets
    mainLayout = new QHBoxLayout(this);
    leftColumn = new LeftColumn(rm, this);
    middleColumn = new MiddleColumn(rm, client, this);
    rightColumn = new RightColumn(rm, client, this);

    // Add the column to the layout
    mainLayout->addWidget(leftColumn);
    mainLayout->addWidget(middleColumn);
    mainLayout->addWidget(rightColumn);

    // Style the layout
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    // Connect signals to slots of the columns
    QObject::connect(leftColumn, SIGNAL(guildClicked(const std::string&)), middleColumn, SLOT(openGuild(const std::string&)));
    QObject::connect(leftColumn, SIGNAL(homeButtonClicked()), middleColumn, SLOT(displayPrivateChannels()));
    QObject::connect(leftColumn, SIGNAL(cleanRightColumn()), rightColumn, SLOT(clean()));
    QObject::connect(middleColumn, SIGNAL(guildChannelClicked(const std::string&, const std::string&)), rightColumn, SLOT(openGuildChannel(const std::string&, const std::string&)));
    QObject::connect(middleColumn, SIGNAL(privateChannelClicked(const std::string&)), rightColumn, SLOT(openPrivateChannel(const std::string&)));
    QObject::connect(rm, SIGNAL(unreadUpdateReceived(const std::string&)), leftColumn, SLOT(setUnreadGuild(const std::string&)));
    QObject::connect(rm, SIGNAL(messageReceived(const Api::Message&)), rightColumn, SLOT(addMessage(const Api::Message&)));
    QObject::connect(rm, SIGNAL(presenceReceived(const Api::Presence&)), middleColumn, SLOT(updatePresence(const Api::Presence&)));
    QObject::connect(rm, SIGNAL(guildsReceived(const std::vector<Api::Guild *>&)), leftColumn, SLOT(displayGuilds(const std::vector<Api::Guild *>&)));
    QObject::connect(rm, SIGNAL(presencesReceived(const std::vector<Api::Presence *>&)), middleColumn, SLOT(setPresences(const std::vector<Api::Presence *>&)));
    QObject::connect(rm, SIGNAL(privateChannelsReceived(const std::vector<Api::PrivateChannel *>&)), middleColumn, SLOT(setPrivateChannels(const std::vector<Api::PrivateChannel *>&)));
    QObject::connect(rm, SIGNAL(channelCreated(const Api::Channel *, const Api::PrivateChannel *)), middleColumn, SLOT(createChannel(const Api::Channel *, const Api::PrivateChannel *)));
    QObject::connect(rm, SIGNAL(channelUpdated(const Api::Channel *, const Api::PrivateChannel *)), middleColumn, SLOT(updateChannel(const Api::Channel *, const Api::PrivateChannel *)));
    QObject::connect(rm, SIGNAL(channelDeleted(const std::string&, const std::string&, int)), middleColumn, SLOT(deleteChannel(const std::string&, const std::string&, int)));
}

} // namespace Ui
