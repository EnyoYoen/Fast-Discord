#pragma once

#include "ui/common/basicwidgets.h"

#include <QLabel>
#include <QString>
#include <QMouseEvent>

namespace Ui {

enum UserMenuButtonTypes {
    Mute,
    Deafen,
    SettingsButton
};

// A button in the user menu (un/deafen, un/mute, settings)
class UserMenuButton : public Widget
{
    Q_OBJECT
public:
    UserMenuButton(int typep, QWidget *parent);
    void setClicked(bool active);

    bool clicked;       // Whether the button is clicked or not

signals:
    void leftClicked(int, bool);
    void rightClicked();

private:
    void mouseReleaseEvent(QMouseEvent *event) override;
    void enterEvent(QEvent *) override;
    void leaveEvent(QEvent *) override;

    int type;           // The type of the button
    bool blocked;       // For mute button, if blocked by deafen button
};

} // namespace Ui
