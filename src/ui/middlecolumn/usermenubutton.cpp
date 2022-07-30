#include "ui/middlecolumn/usermenubutton.h"

#include <QPainter>
#include <QPixmap>

namespace Ui {

UserMenuButton::UserMenuButton(int typep, QWidget *parent)
    : Widget(parent)
{
    // Attributes initialization
    type = typep;
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
        case SettingsButton:
            iconName += "settings";
            break;
    }
    iconName += "-icon.svg";
    QPixmap img(iconName);
    QPainter qp(&img);
    qp.setCompositionMode(QPainter::CompositionMode_SourceIn);
    qp.fillRect(img.rect(), Settings::colors[Settings::InteractiveNormal]);
    qp.end();
    this->setPixmap(img);

    // Style
    this->setFixedSize(Settings::scale(32), Settings::scale(32));
    this->setBorderRadius(Settings::scale(4));
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
            qp.fillRect(img.rect(), Settings::colors[Settings::InteractiveNormal]);
            qp.end();
            this->setPixmap(img);

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
                case SettingsButton:
                    iconName += "settings";
                    break;
            }
            iconName += "-icon.svg";
            QPixmap img(iconName);
            QPainter qp(&img);
            qp.setCompositionMode(QPainter::CompositionMode_SourceIn);
            qp.fillRect(img.rect(), Settings::colors[Settings::InteractiveNormal]);
            qp.end();
            this->setPixmap(img);
            
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
    this->setBackgroundColor(Settings::BackgroundModifierSelected);
    QString iconName = "res/images/svg/";
    switch (type) {
        case Mute:
            iconName += clicked ? "unmute" : "mute";
            break;
        case Deafen:
            iconName += clicked ? "undeafen" : "deafen";
            break;
        case SettingsButton:
            iconName += "settings";
            break;
    }
    iconName += "-icon.svg";
    QPixmap img(iconName);
    QPainter qp(&img);
    qp.setCompositionMode(QPainter::CompositionMode_SourceIn);
    qp.fillRect(img.rect(), Settings::colors[Settings::InteractiveHover]);
    qp.end();
    this->setPixmap(img);
}

void UserMenuButton::leaveEvent(QEvent *) {
    QString iconName = "res/images/svg/";
    switch (type) {
        case Mute:
            iconName += clicked ? "unmute" : "mute";
            break;
        case Deafen:
            iconName += clicked ? "undeafen" : "deafen";
            break;
        case SettingsButton:
            iconName += "settings";
            break;
    }
    iconName += "-icon.svg";
    QPixmap img(iconName);
    QPainter qp(&img);
    qp.setCompositionMode(QPainter::CompositionMode_SourceIn);
    qp.fillRect(img.rect(), Settings::colors[Settings::InteractiveNormal]);
    qp.end();
    this->setPixmap(img);
    this->setBackgroundColor(Settings::None);
}

} // namespace Ui
