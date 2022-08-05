#include "ui/settings/components/scrollmenu.h"

#include "ui/settings/components/menubutton.h"

#include <QVBoxLayout>

namespace Ui {

ScrollMenu::ScrollMenu(QWidget *parent)
    : QScrollArea(nullptr)
{
    scrollWidget = new Widget(this);
    scrollWidget->setBackgroundColor(Settings::BackgroundSecondary);
    QVBoxLayout *layout = new QVBoxLayout(scrollWidget);
    layout->setSpacing(Settings::scale(2));
    layout->setContentsMargins(Settings::scale(20), Settings::scale(60), Settings::scale(6), Settings::scale(60));

    buttons.append(new MenuButton(MenuButton::ButtonType::MyAccount, scrollWidget, true));
    buttons.append(new MenuButton(MenuButton::ButtonType::UserProfile, scrollWidget, true));
    buttons.append(new MenuButton(MenuButton::ButtonType::PrivacySafety, scrollWidget, true));
    buttons.append(new MenuButton(MenuButton::ButtonType::AuthorizedApps, scrollWidget, true));
    buttons.append(new MenuButton(MenuButton::ButtonType::Connections, scrollWidget, true));
    buttons.append(new MenuButton(MenuButton::ButtonType::Appearance, scrollWidget, true));
    buttons.append(new MenuButton(MenuButton::ButtonType::Accessibility, scrollWidget, true));
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
    buttons.append(new MenuButton(MenuButton::ButtonType::LogOut, scrollWidget, true));

    actualType = MenuButton::ButtonType::MyAccount;
    buttons[0]->setBackgroundColor(Settings::BackgroundModifierSelected);
    buttons[0]->setTextColor(Settings::InteractiveActive);

    for (int i = 0 ; i < buttons.size() ; i++) {
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
    this->setFixedWidth(Settings::scale(218));
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setStyleSheet("* {border: none; background-color: " + Settings::colors[Settings::BackgroundSecondary].name() + "}"
                        "QScrollBar::handle:vertical {border: none; border-radius: " + QString::number(Settings::scale(2)) + "px; background-color: " + Settings::colors[Settings::BackgroundTertiary].name() + ";}"
                        "QScrollBar:vertical {border: none; background-color: " + Settings::colors[Settings::BackgroundSecondary].name() + "; border-radius: " + QString::number(Settings::scale(8)) + "px; width: " + QString::number(Settings::scale(3)) + "px;}"
                        "QScrollBar::add-line, QScrollBar::sub-line {border:none; background: none; height: 0;}"
                        "QScrollBar:left-arrow:vertical, QScrollBar::right-arrow:vertical {background: none;}"
                        "QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {background: none;}");
}

void ScrollMenu::resetButtons(MenuButton::ButtonType type)
{
    if (actualType != type) {
        for (int i = 0 ; i < buttons.size() ; i++) {
            if (buttons[i]->type != type) 
                buttons[i]->unclicked();
        }
        actualType = type;
        emit buttonClicked(type);
    }
}

Label *ScrollMenu::createMenuTitle(char *title)
{
    Label *menuTitle = new Label(title, scrollWidget);
    QFont font;
    font.setPixelSize(Settings::scale(12));
    font.setFamily("whitney");
    font.setBold(true);
    menuTitle->setFont(font);
    menuTitle->setFixedSize(Settings::scale(192), Settings::scale(28));
    menuTitle->setTextColor(Settings::ChannelsDefault);
    return menuTitle;
}

Widget *ScrollMenu::createSeparator()
{
    Widget *separator = new Widget(scrollWidget);
    separator->setFixedSize(Settings::scale(172), Settings::scale(17));
    separator->setContentsMargins(Settings::scale(10), Settings::scale(8), Settings::scale(10), Settings::scale(8));
    Widget *lineSeparator = new Widget(separator);
    lineSeparator->move(0, Settings::scale(9));
    lineSeparator->setFixedSize(Settings::scale(172), Settings::scale(1));
    lineSeparator->setBackgroundColor(Settings::BackgroundModifierAccent);
    return separator;
}

} // namespace Ui