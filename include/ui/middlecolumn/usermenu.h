#pragma once

#include "ui/common/roundedimage.h"
#include "ui/common/statusicon.h"
#include "ui/middlecolumn/usermenubutton.h"
#include "api/ressourcemanager.h"

#include <QWidget>

namespace Ui {

// The menu containing some user infos at the bottom of the middle column
class UserMenu : public QWidget
{
    Q_OBJECT
public:
    UserMenu(Api::RessourceManager *rm, QWidget *parent);

    bool deaf;
    bool muted;

signals:
    void voiceStateChanged(bool mute, bool deaf);
    void parametersClicked();

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
