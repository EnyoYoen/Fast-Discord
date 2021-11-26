#include "ui/usermenubutton.h"

namespace Ui {

UserMenuButton::UserMenuButton(int typep, QWidget *parent)
    : QLabel(parent)
{
    // Attributes initialization
    type = typep;
    styleSheet = "";
    clicked = false;
    blocked = false;

    // Determine the icon to use
    iconStyle = "background-image: url(res/images/svg/";
    switch (type) {
        case Mute:
            iconStyle += "mute";
            break;
        case Deafen:
            iconStyle += "deafen";
            break;
        case Settings:
            iconStyle += "settings";
            break;
    }
    iconStyle += "-icon.svg);"
                 "background-repeat: no-repeat;"
                 "background-position: center;"
                 "border-radius: 4px;";

    // Style
    this->setFixedSize(32, 32);
    this->setStyleSheet(iconStyle);
}

void UserMenuButton::setClicked(bool active)
{
    if (type == Mute) {
        if (clicked && active) {
            // Block the widget
            blocked = true;
        } else if (clicked && !active) {
            // Unblock the widget
            blocked = false;
        } else if (!clicked && active) {
            // Put the unmute icon because deafen has been pressed
            iconStyle = "background-image: url(res/images/svg/unmute-icon.svg);"
                        "background-repeat: no-repeat;"
                        "background-position: center;"
                        "border-radius: 4px;";
            this->setStyleSheet(styleSheet + iconStyle);
            blocked = true;
        } else if (!clicked && !active) {
            // Put the mute icon because it was not muted before being blocked
            iconStyle = "background-image: url(res/images/svg/mute-icon.svg);"
                        "background-repeat: no-repeat;"
                        "background-position: center;"
                        "border-radius: 4px;";
            this->setStyleSheet(styleSheet + iconStyle);
            blocked = false;
        }
    }
}

void UserMenuButton::mouseReleaseEvent(QMouseEvent *event)
{
    // Emit signals when clicked
    if (event->button() == Qt::LeftButton) {
       if (!blocked) {
            // Change the icon if not blocked
            iconStyle = "background-image: url(res/images/svg/";
            switch (type) {
                case Mute:
                    iconStyle += clicked ? "mute" : "unmute";
                    break;
                case Deafen:
                    iconStyle += clicked ? "deafen" : "undeafen";
                    break;
                case Settings:
                    iconStyle += "settings";
                    break;
            }
            iconStyle += "-icon.svg);"
                         "background-repeat: no-repeat;"
                         "background-position: center;"
                         "border-radius: 4px;";
            this->setStyleSheet(styleSheet + iconStyle);
            clicked = !clicked;

            // Emit signals
            emit leftClicked(type, clicked);
        }
    } else if (event->button() == Qt::RightButton) {
        emit rightClicked();
    }
}

void UserMenuButton::enterEvent(QEvent *)
{
    // Mouse hover : change the stylesheet
    styleSheet = "background-color: #393D43;";
    this->setStyleSheet(styleSheet + iconStyle);
}

void UserMenuButton::leaveEvent(QEvent *)
{
    // Reset the stylesheet
    styleSheet = "";
    this->setStyleSheet("background-color: none;" + iconStyle);
}

} // namespace Ui
