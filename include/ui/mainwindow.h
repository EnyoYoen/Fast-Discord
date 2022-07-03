#pragma once

#include "ui/common/basicwidgets.h"
#include "ui/leftcolumn/leftcolumn.h"
#include "ui/middlecolumn/middlecolumn.h"
#include "ui/rightcolumn/rightcolumn.h"
#include "ui/settings/settingsmenu.h"
#include "api/ressourcemanager.h"

#include <QWidget>
#include <QHBoxLayout>

namespace Ui {

// The main window of the app
class MainWindow : public Widget
{
    Q_OBJECT

public:
    MainWindow();

signals:
    void clientAndSettingsReceived();

private slots:
    void openSettingsMenu();
    void closeSettingsMenu();

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
    SettingsMenu *settings;

    Api::RessourceManager *rm;           // Manage all the data we receive
};

} // namespace Ui
