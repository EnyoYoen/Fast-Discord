#include "ui/settings/settings.h"

#include "ui/settings/scrollmenu.h"
#include "ui/settings/closebutton.h"

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

    QWidget *container = new QWidget(this);
    QHBoxLayout *notYetImplementedLayout = new QHBoxLayout(container);
    QLabel *notYetImplemented = new QLabel("SETTINGS ARE NOT YET IMPLEMENTED", container);
    notYetImplementedLayout->addWidget(notYetImplemented);
    notYetImplementedLayout->setAlignment(Qt::AlignCenter);
    QFont font;
    font.setPixelSize(25);
    font.setBold(true);
    font.setUnderline(true);
    font.setFamily("whitney");
    notYetImplemented->setFont(font);
    notYetImplemented->setStyleSheet("color: #96989d;");

    layout->addWidget(new ScrollMenu(this));
    layout->addWidget(container);
    layout->addWidget(closeContainer);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);
    
    QObject::connect(close, &CloseButton::clicked, [this](){emit closeClicked();});
    
    this->setStyleSheet("background-color: #36393F;");
}

} // namespace Ui