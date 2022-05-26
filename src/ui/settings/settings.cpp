#include "ui/settings/settings.h"

#include "ui/settings/scrollmenu.h"
#include "ui/settings/closebutton.h"
#include "ui/settings/myaccount.h"

#include <QHBoxLayout>

namespace Ui {

Settings::Settings(Api::RessourceManager *rmp, QWidget *parent)
    : QWidget(parent)
{
    rm = rmp;

    QHBoxLayout *layout = new QHBoxLayout(this);
    CloseButton *close = new CloseButton(this);
    QWidget *closeContainer = new QWidget(this);
    QVBoxLayout *closeLayout = new QVBoxLayout(closeContainer);
    closeContainer->setFixedWidth(57);
    closeLayout->setContentsMargins(0, 0, 0, 0);
    closeLayout->addWidget(close);
    closeLayout->addWidget(new QWidget(closeContainer));

    layout->addWidget(new ScrollMenu(this));
    layout->addWidget(new MyAccount(rm, this));
    layout->addWidget(closeContainer);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);
    
    QObject::connect(close, &CloseButton::clicked, [this](){emit closeClicked();});
    
    this->setStyleSheet("background-color: #36393F;");
}

} // namespace Ui