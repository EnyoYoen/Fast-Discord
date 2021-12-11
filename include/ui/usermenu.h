#pragma once
#pragma once

#include "ui/usermenubutton.h"
#include "ui/roundedimage.h"
#include "api/client.h"
#include "api/request.h"

#include <QWidget>

namespace Ui {

// The menu containing some user infos at the bottom of the middle column
class UserMenu : public QWidget
{
    Q_OBJECT
public:
    UserMenu(Api::Requester *requester, const Api::Client *client, QWidget *parent);

private slots:
    void clicButton(int type, bool active);

private:
    void setIcon(const std::string& iconFileName);

    Api::Requester *requester; // To request the API

    RoundedImage   *avatar;     // The avatar of the user
    UserMenuButton *muteButton; // Used when deafen button is clicked
};

} // namespace Ui
