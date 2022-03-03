#include "ui/mainwindow.h"

#include "api/ressourcemanager.h"

#include <QThread>
#include <QInputDialog>
#include <QListWidget>
#include <QLineEdit>
#include <QSettings>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QDialog>

namespace Ui {

MainWindow::MainWindow() : QWidget()
{
    // Style the window
    // this->setWindowFlags(Qt::CustomizeWindowHint); Soon
    this->setGeometry(0, 0, 940, 728);
    this->setStyleSheet("background-color: #202225;"
                        "padding: 0px;"
                        "border: none;");

    // Get account token from accounts list
    std::string token = getAccountToken();

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

void MainWindow::addAccountInConfig(QSettings *settings, QMap<QString, std::string> accountMap) {
    settings->beginGroup("Accounts");
    QMap<QString, std::string>::const_iterator i = accountMap.constBegin();
    while (i != accountMap.constEnd()) {
         settings->setValue(i.key(), i.value().c_str());
         ++i;
     }
    settings->endGroup();
}

QMap<QString, std::string> MainWindow::getNewAccount() {
    QDialog dlg(nullptr);
    dlg.setWindowTitle(tr("Add new account"));
    dlg.setWindowIcon(QIcon("res/images/png/icon.png"));

    QLineEdit *ledit1 = new QLineEdit(&dlg);
    QLineEdit *ledit2 = new QLineEdit(&dlg);

    QDialogButtonBox *btn_box = new QDialogButtonBox(&dlg);
    btn_box->setStandardButtons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

    connect(btn_box, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
    connect(btn_box, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

    QFormLayout *layout = new QFormLayout();
    layout->addRow(tr("Account name:"), ledit1);
    layout->addRow(tr("Account token:"), ledit2);
    layout->addWidget(btn_box);

    dlg.setLayout(layout);

    // if user press 'ok' button
    if(dlg.exec() == QDialog::Accepted) {
        std::string token = ledit2->text().toUtf8().constData();
        token.erase(std::remove_if(token.begin(), token.end(), ::isspace), token.end());

        return { {ledit1->text().toUtf8(), token} };
    }

    return {};
}

QMap<QString, std::string> MainWindow::getAccountsMap(QSettings *settings) {
    QMap<QString, std::string> accountsMap;

    settings->beginGroup("Accounts");
    QStringList keys = settings->childKeys();
    foreach (QString key, keys) {
         accountsMap[key] = settings->value(key).toString().toStdString();
    }
    settings->endGroup();

    return accountsMap;
}

std::string MainWindow::getAccountToken() {
    QSettings* settings = new QSettings("Fast-Discord", "config");
    std::string token = "";
    QString name = "";

    QMap<QString, std::string> accountsMap = getAccountsMap(settings);

    if (accountsMap.isEmpty()) {
        // Add token if it's empty
        QMap<QString, std::string> tknmp = getNewAccount();
        name = tknmp.firstKey();
        token = tknmp.first();

        addAccountInConfig(settings, tknmp);
    }
    else {
        // Get account from list
        bool ok;
        QString accountName = QInputDialog::getItem(nullptr, "Accounts", "Choose your account", accountsMap.keys(), 0, false, &ok);

        // if user pick normal account - return account token
        if (ok && !accountName.isEmpty()) {
            return accountsMap[accountName];
        }
        // if user push 'cancel' button - add new account and then set token from new account
        else {
            QMap<QString, std::string> tknmp = getNewAccount();

            token = tknmp.first();

            addAccountInConfig(settings, tknmp);
        }
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

    this->setWindowIcon(QIcon("res/images/png/icon.png"));

    // Connect signals to slots of the columns
    QObject::connect(leftColumn, SIGNAL(guildClicked(const std::string&)), middleColumn, SLOT(openGuild(const std::string&)));
    QObject::connect(leftColumn, SIGNAL(homeButtonClicked()), middleColumn, SLOT(displayPrivateChannels()));
    QObject::connect(leftColumn, SIGNAL(cleanRightColumn()), rightColumn, SLOT(clean()));
    QObject::connect(middleColumn, SIGNAL(guildChannelClicked(const std::string&, const std::string&)), rightColumn, SLOT(openGuildChannel(const std::string&, const std::string&)));
    QObject::connect(middleColumn, SIGNAL(privateChannelClicked(const std::string&)), rightColumn, SLOT(openPrivateChannel(const std::string&)));
    QObject::connect(rm, SIGNAL(unreadUpdateReceived(const std::string&)), leftColumn, SLOT(setUnreadGuild(const std::string&)));
    QObject::connect(rm, SIGNAL(messageReceived(const Api::Message&)), rightColumn, SLOT(addMessage(const Api::Message&)));
    QObject::connect(rm, SIGNAL(presenceReceived(const Api::Presence&)), middleColumn, SLOT(updatePresence(const Api::Presence&)));
    QObject::connect(rm, SIGNAL(guildsReceived(const std::vector<Api::Guild *>&, const std::vector<std::string>&, const std::vector<Api::GuildFolder *>&)), leftColumn, SLOT(displayGuilds(const std::vector<Api::Guild *>&, const std::vector<std::string>&, const std::vector<Api::GuildFolder *>&)));
    QObject::connect(rm, SIGNAL(presencesReceived(const std::vector<Api::Presence *>&)), middleColumn, SLOT(setPresences(const std::vector<Api::Presence *>&)));
    QObject::connect(rm, SIGNAL(privateChannelsReceived(const std::vector<Api::PrivateChannel *>&)), middleColumn, SLOT(setPrivateChannels(const std::vector<Api::PrivateChannel *>&)));
    QObject::connect(rm, SIGNAL(channelCreated(const Api::Channel *, const Api::PrivateChannel *)), middleColumn, SLOT(createChannel(const Api::Channel *, const Api::PrivateChannel *)));
    QObject::connect(rm, SIGNAL(channelUpdated(const Api::Channel *, const Api::PrivateChannel *)), middleColumn, SLOT(updateChannel(const Api::Channel *, const Api::PrivateChannel *)));
    QObject::connect(rm, SIGNAL(channelDeleted(const std::string&, const std::string&, int)), middleColumn, SLOT(deleteChannel(const std::string&, const std::string&, int)));
}

} // namespace Ui
