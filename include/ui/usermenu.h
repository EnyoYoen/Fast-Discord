#pragma once
#pragma once

#include "ui/usermenubutton.h"
#include "api/client.h"

#include <QWidget>

namespace Ui {

// The menu containing some user infos at the bottom of the middle column
class UserMenu : public QWidget
{
    Q_OBJECT
public:
    UserMenu(const Api::Client *client, QWidget *parent);

private slots:
    void clicButton(int type, bool active);

private:
    UserMenuButton *muteButton; // Used when deafen button is clicked
};

} // namespace Ui
