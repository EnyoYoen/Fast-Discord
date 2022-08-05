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
    ~MainWindow();

    void updateTheme();
    bool logout;

signals:
    void clientAndSettingsReceived();

public slots:
    void reinit();

private slots:
    void openSettingsMenu();
    void closeSettingsMenu();

private:
    void const addAccountInConfig(QSettings *settings, QMap<QString, QString> accountMap);
    QMap<QString, QString> const getAccountsMap(QSettings *settings);
    QMap<QString, QString> const getNewAccount();
    QString const getAccountToken();

    //All the widget that need to be accessed from different functions
    QHBoxLayout  *mainLayout = nullptr;
    LeftColumn   *leftColumn = nullptr;
    MiddleColumn *middleColumn = nullptr;
    RightColumn  *rightColumn = nullptr;
    SettingsMenu *settings = nullptr;

    Api::RessourceManager *rm = nullptr; // Manage all the data we receive
    QString token;
};

} // namespace Ui
