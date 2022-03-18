#pragma once

#include "ui/usermenubutton.h"
#include "ui/roundedimage.h"
#include "ui/statusicon.h"
#include "api/ressourcemanager.h"
#include "api/objects/client.h"

#include <QWidget>

namespace Ui {

// The menu containing some user infos at the bottom of the middle column
class UserMenu : public QWidget
{
    Q_OBJECT
public:
    UserMenu(Api::RessourceManager *rm, const Api::Client *client, QWidget *parent);

private slots:
    void clicButton(int type, bool active);

private:
    void setIcon(const QString& iconFileName);

    Api::RessourceManager *rm; // To request the API

    RoundedImage   *avatar;     // The avatar of the user
    QLabel         *statusIcon; // The status icon
    UserMenuButton *muteButton; // Used when deafen button is clicked
};

} // namespace Ui
