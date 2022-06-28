#include "ui/settings/menubutton.h"

namespace Ui {

MenuButton::MenuButton(MenuButton::ButtonType typep, QWidget *parent, bool activep) 
    : Label(parent)
{
    active = activep;
    type = typep;
    pressed = false;

    char *text;
    switch (type) {
        case ButtonType::MyAccount:      text = (char *)"My Account";       pressed = true; break;
        case ButtonType::UserProfile:    text = (char *)"User Profile";     break;
        case ButtonType::PrivacySafety:  text = (char *)"Privacy & Safety"; break;
        case ButtonType::AuthorizedApps: text = (char *)"Authorized Apps";  break;
        case ButtonType::Connections:    text = (char *)"Connections";      break;
        case ButtonType::Nitro:          text = (char *)"Nitro";            break;
        case ButtonType::ServerBoost:    text = (char *)"Server Boost";     break;
        case ButtonType::Subscriptions:  text = (char *)"Subscriptions";    break;
        case ButtonType::GiftInventory:  text = (char *)"Gift Inventory";   break;
        case ButtonType::Billing:        text = (char *)"Billing";          break;
        case ButtonType::Appearance:     text = (char *)"Appearance";       break;
        case ButtonType::Accessibility:  text = (char *)"Accessibility";    break;
        case ButtonType::VoiceVideo:     text = (char *)"Voice & Video";    break;
        case ButtonType::TextImages:     text = (char *)"Text & Images";    break;
        case ButtonType::Notifications:  text = (char *)"Notifications";    break;
        case ButtonType::Keybinds:       text = (char *)"Keybinds";         break;
        case ButtonType::Language:       text = (char *)"Language";         break;
        case ButtonType::StreamerMode:   text = (char *)"Streamer Mode";    break;
        case ButtonType::Advanced:       text = (char *)"Advanced";         break;
        case ButtonType::ActivityStatus: text = (char *)"Activity Status";  break;
        case ButtonType::WhatsNew:       text = (char *)"What's New";       break;
        case ButtonType::HypeSquad:      text = (char *)"HypeSquad";        break;
        case ButtonType::LogOut:         text = (char *)"Log Out";          break;
        default:                         text = (char *)"";
    }
    QFont font;
    font.setPixelSize(16);
    font.setFamily("whitney");
    this->setFont(font);
    this->setText(text);

    this->setFixedSize(192, 32);
    this->setBorderRadius(4);
    this->setBackgroundColor(Settings::BackgroundSecondary);
    this->setTextColor(Settings::InteractiveNormal);
}

void MenuButton::unclicked()
{
    pressed = false;
    if (active) {
        this->setBackgroundColor(Settings::BackgroundSecondary);
        this->setTextColor(Settings::InteractiveNormal);
    }
}

void MenuButton::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        emit clicked(type);
    }
}

void MenuButton::mousePressEvent(QMouseEvent *)
{
    pressed = true;
    if (active) {
        this->setBackgroundColor(Settings::BackgroundModifierSelected);
        this->setTextColor(Settings::InteractiveActive);
    }
}

void MenuButton::enterEvent(QEvent *)
{
    if (!pressed) {
        if (active) {
            this->setBackgroundColor(Settings::BackgroundModifierHover);
            this->setTextColor(Settings::InteractiveNormal);
        }
    }
}

void MenuButton::leaveEvent(QEvent *)
{
    if (!pressed) {
        if (active) {
            this->setBackgroundColor(Settings::BackgroundSecondary);
            this->setTextColor(Settings::InteractiveNormal);
        }
    }
}

} // namespace Ui