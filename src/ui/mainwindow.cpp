#include "ui/mainwindow.h"

#include "settings/settings.h"
#include "api/ressourcemanager.h"
#include "api/request.h"

#include <QThread>
#include <QInputDialog>
#include <QListWidget>
#include <QLineEdit>
#include <QSettings>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QDialog>
#include <QFontDatabase>

namespace Ui {

MainWindow::MainWindow() : Widget(nullptr)
{
    logout = false;

    // Style the window
    // this->setWindowFlags(Qt::CustomizeWindowHint); Soon
    this->setGeometry(0, 0, 940, 728);
    QFontDatabase::addApplicationFont(":whitney.otf");

    // Get account token from accounts list
    token = getAccountToken();

    // Create the ressource manager
    rm = new Api::RessourceManager(token);

    QObject::connect(rm->requester, &Api::Requester::invalidToken, this, &MainWindow::reinit);

    Settings::initSettings(rm, token);

    // Create all the widgets
    mainLayout = new QHBoxLayout(this);
    leftColumn = new LeftColumn(rm, this);
    middleColumn = new MiddleColumn(rm, this);
    rightColumn = new RightColumn(rm, this);
    settings = new SettingsMenu(rm, this);
    settings->hide();

    // Add the column to the layout
    mainLayout->addWidget(leftColumn);
    mainLayout->addWidget(middleColumn);
    mainLayout->addWidget(rightColumn);

    // Style the layout
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    this->setWindowIcon(QIcon(":icon.png"));
    this->setBackgroundColor(Settings::BackgroundPrimary);

    // Connect signals to slots of the columns
    QObject::connect(leftColumn, &LeftColumn::guildClicked, middleColumn, &MiddleColumn::openGuild);
    QObject::connect(leftColumn, &LeftColumn::homeButtonClicked, middleColumn, &MiddleColumn::displayPrivateChannels);
    QObject::connect(leftColumn, &LeftColumn::cleanRightColumn, rightColumn, &RightColumn::clean);
    QObject::connect(middleColumn, &MiddleColumn::guildChannelClicked, rightColumn, &RightColumn::openGuildChannel);
    QObject::connect(middleColumn, &MiddleColumn::privateChannelClicked, rightColumn, &RightColumn::openPrivateChannel);
    QObject::connect(middleColumn, &MiddleColumn::voiceChannelClicked, rm, &Api::RessourceManager::call);
    QObject::connect(middleColumn, &MiddleColumn::parametersClicked, this, &MainWindow::openSettingsMenu);
    QObject::connect(rightColumn, &RightColumn::messageAdded, middleColumn, &MiddleColumn::putChannelFirst);
    QObject::connect(settings, &SettingsMenu::closeClicked, this, &MainWindow::closeSettingsMenu);
    QObject::connect(rm, &Api::RessourceManager::unreadUpdateReceived, leftColumn, &LeftColumn::setUnreadGuild);
    QObject::connect(rm, &Api::RessourceManager::messageReceived, rightColumn, &RightColumn::addMessage);
    QObject::connect(rm, &Api::RessourceManager::presenceReceived, middleColumn, &MiddleColumn::updatePresence);
    QObject::connect(rm, &Api::RessourceManager::guildsReceived, leftColumn, &LeftColumn::displayGuilds);
    QObject::connect(rm, &Api::RessourceManager::presencesReceived, middleColumn, &MiddleColumn::setPresences);
    QObject::connect(rm, &Api::RessourceManager::privateChannelsReceived, middleColumn, &MiddleColumn::setPrivateChannels);
    QObject::connect(rm, &Api::RessourceManager::channelCreated, middleColumn, &MiddleColumn::createChannel);
    QObject::connect(rm, &Api::RessourceManager::channelUpdated, middleColumn, &MiddleColumn::updateChannel);
    QObject::connect(rm, &Api::RessourceManager::channelDeleted, middleColumn, &MiddleColumn::deleteChannel);
    QObject::connect(rm, &Api::RessourceManager::memberUpdateReceived, rightColumn, &RightColumn::setMembers);
}

void MainWindow::reinit()
{
    QString newToken;
    if (!logout) {
        bool ok;
        QString newToken = QInputDialog::getText(this, "Re-enter your Discord token", "New Token", QLineEdit::Normal, QString(), &ok);
        if (!ok || newToken.isEmpty()) reinit();
        else {
            Settings::changeToken(token, newToken);
            exit(0);
        }
    }

    if (mainLayout) {
        delete mainLayout;
        mainLayout = nullptr;
    }
    if (leftColumn) {
        delete leftColumn;
        leftColumn = nullptr;
    }
    if (middleColumn) {
        delete middleColumn;
        middleColumn = nullptr;
    }
    if (rightColumn) {
        delete rightColumn;
        rightColumn = nullptr;
    }
    if (settings) {
        delete settings;
        settings = nullptr;
    }
    if (rm) {
        delete rm;
        rm = nullptr;
    }

    if (logout) {
        logout = false;
        token = getAccountToken();
    } else {
        Settings::changeToken(token, newToken);
        token = newToken;
    }

    rm = new Api::RessourceManager(token);
    
    QObject::connect(rm->requester, &Api::Requester::invalidToken, this, &MainWindow::reinit);  

    Settings::initSettings(rm, token);

    // Create all the widgets
    mainLayout = new QHBoxLayout(this);
    leftColumn = new LeftColumn(rm, this);
    middleColumn = new MiddleColumn(rm, this);
    rightColumn = new RightColumn(rm, this);
    settings = new SettingsMenu(rm, this);
    settings->hide();

    // Add the column to the layout
    mainLayout->addWidget(leftColumn);
    mainLayout->addWidget(middleColumn);
    mainLayout->addWidget(rightColumn);

    // Style the layout
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    this->setWindowIcon(QIcon(":icon.png"));
    this->setBackgroundColor(Settings::BackgroundPrimary);

    // Connect signals to slots of the columns
    QObject::connect(leftColumn, &LeftColumn::guildClicked, middleColumn, &MiddleColumn::openGuild);
    QObject::connect(leftColumn, &LeftColumn::homeButtonClicked, middleColumn, &MiddleColumn::displayPrivateChannels);
    QObject::connect(leftColumn, &LeftColumn::cleanRightColumn, rightColumn, &RightColumn::clean);
    QObject::connect(middleColumn, &MiddleColumn::guildChannelClicked, rightColumn, &RightColumn::openGuildChannel);
    QObject::connect(middleColumn, &MiddleColumn::privateChannelClicked, rightColumn, &RightColumn::openPrivateChannel);
    QObject::connect(middleColumn, &MiddleColumn::voiceChannelClicked, rm, &Api::RessourceManager::call);
    QObject::connect(middleColumn, &MiddleColumn::parametersClicked, this, &MainWindow::openSettingsMenu);
    QObject::connect(rightColumn, &RightColumn::messageAdded, middleColumn, &MiddleColumn::putChannelFirst);
    QObject::connect(settings, &SettingsMenu::closeClicked, this, &MainWindow::closeSettingsMenu);
    QObject::connect(rm, &Api::RessourceManager::unreadUpdateReceived, leftColumn, &LeftColumn::setUnreadGuild);
    QObject::connect(rm, &Api::RessourceManager::messageReceived, rightColumn, &RightColumn::addMessage);
    QObject::connect(rm, &Api::RessourceManager::presenceReceived, middleColumn, &MiddleColumn::updatePresence);
    QObject::connect(rm, &Api::RessourceManager::guildsReceived, leftColumn, &LeftColumn::displayGuilds);
    QObject::connect(rm, &Api::RessourceManager::presencesReceived, middleColumn, &MiddleColumn::setPresences);
    QObject::connect(rm, &Api::RessourceManager::privateChannelsReceived, middleColumn, &MiddleColumn::setPrivateChannels);
    QObject::connect(rm, &Api::RessourceManager::channelCreated, middleColumn, &MiddleColumn::createChannel);
    QObject::connect(rm, &Api::RessourceManager::channelUpdated, middleColumn, &MiddleColumn::updateChannel);
    QObject::connect(rm, &Api::RessourceManager::channelDeleted, middleColumn, &MiddleColumn::deleteChannel);
    QObject::connect(rm, &Api::RessourceManager::memberUpdateReceived, rightColumn, &RightColumn::setMembers);
}

void const MainWindow::addAccountInConfig(QSettings *settings, const QMap<QString, QString> accountMap) {
    settings->beginGroup("Accounts");
    QMap<QString, QString>::const_iterator i = accountMap.constBegin();
    while (i != accountMap.constEnd()) {
         settings->setValue(i.key(), i.value());
         ++i;
     }
    settings->endGroup();
}

QMap<QString, QString> const MainWindow::getNewAccount() {
    QDialog dlg(nullptr);
    dlg.setWindowTitle(tr("Add new account"));
    dlg.setWindowIcon(QIcon(":icon.png"));

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

QMap<QString, QString> const MainWindow::getAccountsMap(QSettings *settings) {
    QMap<QString, QString> accountsMap;

    settings->beginGroup("Accounts");
    QStringList keys = settings->childKeys();
    foreach (QString key, keys) {
         accountsMap[key] = settings->value(key).toString();
    }
    settings->endGroup();

    return accountsMap;
}

QString const MainWindow::getAccountToken() {
    QSettings* settings = new QSettings("Fast-Discord", "config");
    QString token = "";
    QString name = "";

    QMap<QString, QString> accountsMap = getAccountsMap(settings);

    if (accountsMap.isEmpty()) {
        // Add token if it's empty
        QMap<QString, QString> tknmp = getNewAccount();
        if (tknmp.isEmpty())
            exit(0);

        name = tknmp.firstKey();
        token = tknmp.first();

        addAccountInConfig(settings, tknmp);
    } else {
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

void MainWindow::openSettingsMenu()
{
    mainLayout->removeWidget(leftColumn);
    mainLayout->removeWidget(middleColumn);
    mainLayout->removeWidget(rightColumn);
    mainLayout->addWidget(settings);
    leftColumn->hide();
    middleColumn->hide();
    rightColumn->hide();
    settings->show();
}

void MainWindow::closeSettingsMenu()
{
    mainLayout->removeWidget(settings);
    mainLayout->addWidget(leftColumn);
    mainLayout->addWidget(middleColumn);
    mainLayout->addWidget(rightColumn);
    settings->hide();
    leftColumn->show();
    middleColumn->show();
    rightColumn->show();
}

void MainWindow::updateTheme()
{
    leftColumn->setStyleSheet("border: none;"
                              "background-color:" + Settings::colors[Settings::BackgroundTertiary].name());
    middleColumn->channelList->setStyleSheet("* {background-color:" + Settings::colors[Settings::BackgroundSecondary].name() + "; border: none;}"
                               "QScrollBar::handle:vertical {border: none; border-radius: " + QString::number(Settings::scale(2)) + "px; background-color: " + Settings::colors[Settings::BackgroundTertiary].name() + ";}"
                               "QScrollBar:vertical {border: none; background-color: " + Settings::colors[Settings::BackgroundSecondary].name() + "; border-radius: " + QString::number(Settings::scale(8)) + "px; width: " + QString::number(Settings::scale(3)) + "px;}"
                               "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {border:none; background: none; height: 0;}"
                               "QScrollBar:left-arrow:vertical, QScrollBar::right-arrow:vertical {background: none;}"
                               "QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {background: none;}");
    rightColumn->updateTheme();
}

MainWindow::~MainWindow()
{
    Settings::saveSettings();
}

} // namespace Ui
