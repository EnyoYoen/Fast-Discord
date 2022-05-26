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

    buttons.append(new MenuButton(MenuButton::ButtonType::MyAccount, scrollWidget));
    buttons.append(new MenuButton(MenuButton::ButtonType::UserProfile, scrollWidget));
    buttons.append(new MenuButton(MenuButton::ButtonType::PrivacySafety, scrollWidget));
    buttons.append(new MenuButton(MenuButton::ButtonType::AuthorizedApps, scrollWidget));
    buttons.append(new MenuButton(MenuButton::ButtonType::Connections, scrollWidget));
    buttons.append(new MenuButton(MenuButton::ButtonType::Appearance, scrollWidget));
    buttons.append(new MenuButton(MenuButton::ButtonType::Accessibility, scrollWidget));
    buttons.append(new MenuButton(MenuButton::ButtonType::VoiceVideo, scrollWidget));
    buttons.append(new MenuButton(MenuButton::ButtonType::TextImages, scrollWidget));
    buttons.append(new MenuButton(MenuButton::ButtonType::Notifications, scrollWidget));
    buttons.append(new MenuButton(MenuButton::ButtonType::Keybinds, scrollWidget));
    buttons.append(new MenuButton(MenuButton::ButtonType::Language, scrollWidget));
    buttons.append(new MenuButton(MenuButton::ButtonType::StreamerMode, scrollWidget));
    buttons.append(new MenuButton(MenuButton::ButtonType::Advanced, scrollWidget));
    buttons.append(new MenuButton(MenuButton::ButtonType::ActivityStatus, scrollWidget));
    buttons.append(new MenuButton(MenuButton::ButtonType::WhatsNew, scrollWidget));
    buttons.append(new MenuButton(MenuButton::ButtonType::HypeSquad, scrollWidget));
    buttons.append(new MenuButton(MenuButton::ButtonType::LogOut, scrollWidget));

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
    for (unsigned int i = 0 ; i < buttons.size() ; i++) {
        if (buttons[i]->type != type) 
            buttons[i]->unclicked();
    }
    emit buttonClicked(type);
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