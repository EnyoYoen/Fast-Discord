#pragma once

#include "ui/leftcolumn.h"
#include "ui/middlecolumn.h"
#include "ui/rightcolumn.h"
#include "api/ressourcemanager.h"

#include <QWidget>
#include <QHBoxLayout>

namespace Ui {

// The main window of the app
class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow();

signals:
    void clientSettingsReceived();

private slots:
    void setup();

private:
    void const addAccountInConfig(QSettings *settings, QMap<QString, QString> accountMap);
    QMap<QString, QString> const getAccountsMap(QSettings *settings);
    QMap<QString, QString> const getNewAccount();
    QString const getAccountToken();

    //All the widget that need to be accessed from different functions
    QHBoxLayout  *mainLayout;
    LeftColumn   *leftColumn;
    MiddleColumn *middleColumn;
    RightColumn  *rightColumn;

    Api::RessourceManager *rm;           // Manage all the data we receive

    Api::Client *client;                 // The actual client
    Api::ClientSettings *clientSettings; // and the settings
};

} // namespace Ui
