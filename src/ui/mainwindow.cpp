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
    QString token = getAccountToken();

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

void MainWindow::addAccountInConfig(QSettings *settings, QMap<QString, QString> accountMap) {
    settings->beginGroup("Accounts");
    QMap<QString, QString>::const_iterator i = accountMap.constBegin();
    while (i != accountMap.constEnd()) {
         settings->setValue(i.key(), i.value());
         ++i;
     }
    settings->endGroup();
}

QMap<QString, QString> MainWindow::getNewAccount() {
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
        QString token = ledit2->text();
        token = token.trimmed();

        return { {ledit1->text(), token} };
    }

    return {};
}

QMap<QString, QString> MainWindow::getAccountsMap(QSettings *settings) {
    QMap<QString, QString> accountsMap;

    settings->beginGroup("Accounts");
    QStringList keys = settings->childKeys();
    foreach (QString key, keys) {
         accountsMap[key] = settings->value(key).toString();
    }
    settings->endGroup();

    return accountsMap;
}

QString MainWindow::getAccountToken() {
    QSettings* settings = new QSettings("Fast-Discord", "config");
    QString token = "";
    QString name = "";

    QMap<QString, QString> accountsMap = getAccountsMap(settings);

    if (accountsMap.isEmpty()) {
        // Add token if it's empty
        QMap<QString, QString> tknmp = getNewAccount();
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
            QMap<QString, QString> tknmp = getNewAccount();

            if (tknmp.isEmpty())
                exit(0);

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
    QObject::connect(leftColumn, SIGNAL(guildClicked(const Api::Snowflake&)), middleColumn, SLOT(openGuild(const Api::Snowflake&)));
    QObject::connect(leftColumn, SIGNAL(homeButtonClicked()), middleColumn, SLOT(displayPrivateChannels()));
    QObject::connect(leftColumn, SIGNAL(cleanRightColumn()), rightColumn, SLOT(clean()));
    QObject::connect(middleColumn, SIGNAL(guildChannelClicked(const Api::Snowflake&, const Api::Snowflake&)), rightColumn, SLOT(openGuildChannel(const Api::Snowflake&, const Api::Snowflake&)));
    QObject::connect(middleColumn, SIGNAL(privateChannelClicked(const Api::Snowflake&)), rightColumn, SLOT(openPrivateChannel(const Api::Snowflake&)));
    QObject::connect(rightColumn, SIGNAL(messageAdded(const Api::Snowflake&)), middleColumn, SLOT(putChannelFirst(const Api::Snowflake&)));
    QObject::connect(rm, SIGNAL(unreadUpdateReceived(const Api::Snowflake&)), leftColumn, SLOT(setUnreadGuild(const Api::Snowflake&)));
    QObject::connect(rm, SIGNAL(messageReceived(const Api::Message&)), rightColumn, SLOT(addMessage(const Api::Message&)));
    QObject::connect(rm, SIGNAL(presenceReceived(const Api::Presence&)), middleColumn, SLOT(updatePresence(const Api::Presence&)));
    QObject::connect(rm, SIGNAL(guildsReceived(const QVector<Api::Guild *>&)), leftColumn, SLOT(displayGuilds(const QVector<Api::Guild *>&)));
    QObject::connect(rm, SIGNAL(presencesReceived(const QVector<Api::Presence *>&)), middleColumn, SLOT(setPresences(const QVector<Api::Presence *>&)));
    QObject::connect(rm, SIGNAL(privateChannelsReceived(QVector<Api::PrivateChannel *>)), middleColumn, SLOT(setPrivateChannels(QVector<Api::PrivateChannel *>)));
    QObject::connect(rm, SIGNAL(channelCreated(const Api::Channel *, const Api::PrivateChannel *)), middleColumn, SLOT(createChannel(const Api::Channel *, const Api::PrivateChannel *)));
    QObject::connect(rm, SIGNAL(channelUpdated(const Api::Channel *, const Api::PrivateChannel *)), middleColumn, SLOT(updateChannel(const Api::Channel *, const Api::PrivateChannel *)));
    QObject::connect(rm, SIGNAL(channelDeleted(const Api::Snowflake&, const Api::Snowflake&, int)), middleColumn, SLOT(deleteChannel(const Api::Snowflake&, const Api::Snowflake&, int)));
}

} // namespace Ui
