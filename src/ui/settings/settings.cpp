#include "ui/settings/settings.h"

#include "ui/settings/scrollmenu.h"
#include "ui/settings/closebutton.h"
#include "ui/settings/myaccount.h"
#include "ui/settings/userprofile.h"

namespace Ui {

Settings::Settings(Api::RessourceManager *rmp, QWidget *parent)
    : QWidget(parent)
{
    rm = rmp;

    layout = new QHBoxLayout(this);
    CloseButton *close = new CloseButton(this);
    QWidget *closeContainer = new QWidget(this);
    QVBoxLayout *closeLayout = new QVBoxLayout(closeContainer);
    closeContainer->setFixedWidth(57);
    closeLayout->setContentsMargins(0, 0, 0, 0);
    closeLayout->addWidget(close);
    closeLayout->addWidget(new QWidget(closeContainer));

    ScrollMenu *scrollMenu = new ScrollMenu(this);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(scrollMenu);
    layout->addWidget(new MyAccount(rm, this));
    layout->addWidget(closeContainer);
    
    QObject::connect(close, &CloseButton::clicked, [this](){emit closeClicked();});
    QObject::connect(scrollMenu, &ScrollMenu::buttonClicked, [this](MenuButton::ButtonType type){
        QWidget *menu = nullptr;
        switch (type) {
            case MenuButton::ButtonType::MyAccount:
                menu = new MyAccount(rm, this);
                break;
            case MenuButton::ButtonType::UserProfile:
                menu = new UserProfile(rm, this);
                break;
        }

        if (menu != nullptr) {
            layout->replaceWidget(layout->itemAt(1)->widget(), menu);
        }
    });
    
    this->setStyleSheet("background-color: #36393F;");
}

} // namespace Ui