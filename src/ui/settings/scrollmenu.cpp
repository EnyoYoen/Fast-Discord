#include "ui/settings/scrollmenu.h"

#include "ui/settings/menubutton.h"

#include <QVBoxLayout>

namespace Ui {

ScrollMenu::ScrollMenu(QWidget *parent)
    : QScrollArea(nullptr)
{
    scrollWidget = new QWidget(this);
    scrollWidget->setStyleSheet("background-color: #2F3136;");
    QVBoxLayout *layout = new QVBoxLayout(scrollWidget);
    layout->setSpacing(2);
    layout->setContentsMargins(20, 60, 6, 60);

    buttons.append(new MenuButton(MenuButton::ButtonType::MyAccount, scrollWidget, true));
    buttons.append(new MenuButton(MenuButton::ButtonType::UserProfile, scrollWidget, true));
    buttons.append(new MenuButton(MenuButton::ButtonType::PrivacySafety, scrollWidget, false));
    buttons.append(new MenuButton(MenuButton::ButtonType::AuthorizedApps, scrollWidget, false));
    buttons.append(new MenuButton(MenuButton::ButtonType::Connections, scrollWidget, false));
    buttons.append(new MenuButton(MenuButton::ButtonType::Appearance, scrollWidget, false));
    buttons.append(new MenuButton(MenuButton::ButtonType::Accessibility, scrollWidget, false));
    buttons.append(new MenuButton(MenuButton::ButtonType::VoiceVideo, scrollWidget, false));
    buttons.append(new MenuButton(MenuButton::ButtonType::TextImages, scrollWidget, false));
    buttons.append(new MenuButton(MenuButton::ButtonType::Notifications, scrollWidget, false));
    buttons.append(new MenuButton(MenuButton::ButtonType::Keybinds, scrollWidget, false));
    buttons.append(new MenuButton(MenuButton::ButtonType::Language, scrollWidget, false));
    buttons.append(new MenuButton(MenuButton::ButtonType::StreamerMode, scrollWidget, false));
    buttons.append(new MenuButton(MenuButton::ButtonType::Advanced, scrollWidget, false));
    buttons.append(new MenuButton(MenuButton::ButtonType::ActivityStatus, scrollWidget, false));
    buttons.append(new MenuButton(MenuButton::ButtonType::WhatsNew, scrollWidget, false));
    buttons.append(new MenuButton(MenuButton::ButtonType::HypeSquad, scrollWidget, false));
    buttons.append(new MenuButton(MenuButton::ButtonType::LogOut, scrollWidget, false));

    actualType = MenuButton::ButtonType::MyAccount;
    buttons[0]->setStyleSheet("border-radius: 4px;"
                              "color: #FFF;"
                              "background-color: rgba(79, 84, 92, 0.6);");

    for (unsigned int i = 0 ; i < buttons.size() ; i++) {
        if (i == 0) {
            layout->addWidget(createMenuTitle((char *)"USER SETTINGS"));
        } else if (i == 5) {
            layout->addWidget(createSeparator());
            layout->addWidget(createMenuTitle((char *)"APP SETTINGS"));
        } else if (i == 14) {
            layout->addWidget(createSeparator());
            layout->addWidget(createMenuTitle((char *)"ACTIVITY SETTINGS"));
        } else if (i == 15) {
            layout->addWidget(createSeparator());
        } else if (i == 17) {
            layout->addWidget(createSeparator());
        }

        layout->addWidget(buttons[i]);
        QObject::connect(buttons[i], &MenuButton::clicked, this, &ScrollMenu::resetButtons);
    }
    layout->addWidget(createSeparator());

    this->setWidget(scrollWidget);
    this->setFixedWidth(218);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setStyleSheet("QScrollBar::handle:vertical {border: none; border-radius: 2px; background-color: #202225;}"
                        "QScrollBar:vertical {border: none; background-color: #2F3136; border-radius: 8px; width: 3px;}"
                        "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {border:none; background: none; height: 0;}");
}

void ScrollMenu::resetButtons(MenuButton::ButtonType type)
{
    if (actualType != type) {
        for (unsigned int i = 0 ; i < buttons.size() ; i++) {
            if (buttons[i]->type != type) 
                buttons[i]->unclicked();
        }
        actualType = type;
        emit buttonClicked(type);
    }
}

QLabel *ScrollMenu::createMenuTitle(char *title)
{
    QLabel *menuTitle = new QLabel(title, scrollWidget);
    QFont font;
    font.setPixelSize(12);
    font.setFamily("whitney");
    font.setBold(true);
    menuTitle->setFont(font);
    menuTitle->setFixedSize(192, 28);
    menuTitle->setStyleSheet("color: #96989D;");
    return menuTitle;
}

QWidget *ScrollMenu::createSeparator()
{
    QWidget *separator = new QWidget(scrollWidget);
    separator->setFixedSize(172, 17);
    separator->setContentsMargins(10, 8, 10, 8);
    QWidget *lineSeparator = new QWidget(separator);
    lineSeparator->move(0, 9);
    lineSeparator->setFixedSize(172, 1);
    lineSeparator->setStyleSheet("background-color: rgba(79, 84, 92, 0.48);");
    return separator;
}

} // namespace Ui