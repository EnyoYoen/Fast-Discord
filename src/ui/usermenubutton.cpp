#include "ui/usermenubutton.h"

#include <QPainter>
#include <QPixmap>

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
    QString iconName = "res/images/svg/";
    switch (type) {
        case Mute:
            iconName += "mute";
            break;
        case Deafen:
            iconName += "deafen";
            break;
        case Settings:
            iconName += "settings";
            break;
    }
    iconName += "-icon.svg";
    QPixmap img(iconName);
    QPainter qp(&img);
    qp.setCompositionMode(QPainter::CompositionMode_SourceIn);
    qp.fillRect(img.rect(),QColor(185, 187, 190));
    qp.end();
    this->setPixmap(img);

    iconStyle += "background-repeat: no-repeat;"
                 "background-position: center;"
                 "border-radius: 4px;";

    // Style
    this->setFixedSize(32, 32);
    this->setStyleSheet(iconStyle);
}

void UserMenuButton::setClicked(bool active)
{
    if (type == Mute) {
        if (clicked) {
            // Block the widget
            blocked = active;
        } else {
            QString prefix = (active ? "un" : "");
            QPixmap img("res/images/svg/" + prefix + "mute-icon.svg");
            QPainter qp(&img);
            qp.setCompositionMode(QPainter::CompositionMode_SourceIn);
            qp.fillRect(img.rect(),QColor(185, 187, 190));
            qp.end();
            this->setPixmap(img);

            // Put the unmute icon because deafen has been pressed
            iconStyle = "background-repeat: no-repeat;"
                        "background-position: center;"
                        "border-radius: 4px;";
            this->setStyleSheet(styleSheet + iconStyle);
            blocked = active;
        }
    }
}

void UserMenuButton::mouseReleaseEvent(QMouseEvent *event)
{
    // Emit signals when clicked
    if (event->button() == Qt::LeftButton) {
        if (!blocked) {
            // Change the icon if not blocked
            QString iconName = "res/images/svg/";
            switch (type) {
                case Mute:
                    iconName += clicked ? "mute" : "unmute";
                    break;
                case Deafen:
                    iconName += clicked ? "deafen" : "undeafen";
                    break;
                case Settings:
                    iconName += "settings";
                    break;
            }
            iconName += "-icon.svg";
            QPixmap img(iconName);
            QPainter qp(&img);
            qp.setCompositionMode(QPainter::CompositionMode_SourceIn);
            qp.fillRect(img.rect(),QColor(185, 187, 190));
            qp.end();
            this->setPixmap(img);

            iconStyle += "background-repeat: no-repeat;"
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
