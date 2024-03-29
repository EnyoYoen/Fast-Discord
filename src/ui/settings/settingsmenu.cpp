#include "ui/settings/settingsmenu.h"

#include "ui/settings/components/closebutton.h"
#include "ui/settings/components/popup.h"
#include "ui/settings/menus/myaccount.h"
#include "ui/settings/menus/userprofile.h"
#include "ui/settings/menus/privacysafety.h"
#include "ui/settings/menus/authorizedapps.h"
#include "ui/settings/menus/connections.h"
#include "ui/settings/menus/appearance.h"
#include "ui/settings/menus/accessibility.h"
#include "ui/mainwindow.h"

namespace Ui {

SettingsMenu::SettingsMenu(Api::RessourceManager *rmp, QWidget *parent)
    : Widget(parent)
{
    rm = rmp;

    layout = new QHBoxLayout(this);
    CloseButton *close = new CloseButton(this);
    Widget *closeContainer = new Widget(this);
    QVBoxLayout *closeLayout = new QVBoxLayout(closeContainer);
    closeContainer->setFixedWidth(Settings::scale(57));
    closeContainer->setBackgroundColor(Settings::BackgroundPrimary);
    closeLayout->setContentsMargins(0, 0, 0, 0);
    closeLayout->addWidget(close);
    closeLayout->addWidget(new QWidget(closeContainer));

    scrollMenu = new ScrollMenu(this);
    menu = new MyAccount(rm, this);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(scrollMenu);
    layout->addWidget(menu);
    layout->addWidget(closeContainer);
    
    QObject::connect(close, &CloseButton::clicked, [this](){emit closeClicked();});
    QObject::connect(scrollMenu, &ScrollMenu::buttonClicked, [this](MenuButton::ButtonType type){
        if (menu != nullptr && (type == MenuButton::ButtonType::MyAccount
                             || type == MenuButton::ButtonType::UserProfile
                             || type == MenuButton::ButtonType::PrivacySafety
                             || type == MenuButton::ButtonType::AuthorizedApps
                             || type == MenuButton::ButtonType::Connections
                             || type == MenuButton::ButtonType::Appearance
                             || type == MenuButton::ButtonType::Accessibility)) {
            menu->deleteLater();
            menu = nullptr;
        }

        switch (type) {
            case MenuButton::ButtonType::MyAccount:
                menu = new MyAccount(rm, this);
                break;
            case MenuButton::ButtonType::UserProfile:
                menu = new UserProfile(rm, this);
                break;
            case MenuButton::ButtonType::PrivacySafety:
                menu = new PrivacySafety(rm, this);
                break;
            case MenuButton::ButtonType::AuthorizedApps:
                menu = new AuthorizedApps(rm, this);
                break;
            case MenuButton::ButtonType::Connections:
                menu = new Connections(rm, this);
                break;
            case MenuButton::ButtonType::Appearance:
                menu = new Appearance(rm, this);
                break;
            case MenuButton::ButtonType::Accessibility:
                menu = new Accessibility(rm, this);
                break;
            case MenuButton::ButtonType::LogOut:
                {
                    QWidget *parentWidget = this;
                    while (parentWidget->parent()) parentWidget = (Widget *)parentWidget->parent();
                    PopUp *logOutPopUp = new PopUp(new Widget(), 440, 200, QString(), "Log Out", false, false, "Are you sure tou want to log out?", "Cancel", "Log Out", true, true, parentWidget->size(), parentWidget);
                    QObject::connect(logOutPopUp, &PopUp::cancelled, [logOutPopUp](){logOutPopUp->deleteLater();});
                    QObject::connect(logOutPopUp, &PopUp::done, [this, logOutPopUp](){
                        logOutPopUp->deleteLater();
                        MainWindow *mainWindow = reinterpret_cast<MainWindow *>(this->parentWidget());
                        mainWindow->logout = true;
                        mainWindow->reinit();
                    });
                    break;
                }
            default:
                break;
        }

        if (menu != nullptr) {
            layout->replaceWidget(layout->itemAt(1)->widget(), menu);
        }
    });
}


void SettingsMenu::updateTheme()
{
    scrollMenu->setStyleSheet("* {border: none; background-color: " + Settings::colors[Settings::BackgroundSecondary].name() + "}"
                        "QScrollBar::handle:vertical {border: none; border-radius: " + QString::number(Settings::scale(2)) + "px; background-color: " + Settings::colors[Settings::BackgroundTertiary].name() + ";}"
                        "QScrollBar:vertical {border: none; background-color: " + Settings::colors[Settings::BackgroundSecondary].name() + "; border-radius: " + QString::number(Settings::scale(8)) + "px; width: " + QString::number(Settings::scale(3)) + "px;}"
                        "QScrollBar::add-line, QScrollBar::sub-line {border:none; background: none; height: 0;}"
                        "QScrollBar:left-arrow:vertical, QScrollBar::right-arrow:vertical {background: none;}"
                        "QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {background: none;}");
    reinterpret_cast<MainWindow *>(this->parent())->updateTheme();
}

} // namespace Ui