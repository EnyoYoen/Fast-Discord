#include "ui/settings/selectimagebutton.h"

namespace Ui {

SelectImageButton::SelectImageButton(QWidget *parent)
    : QLabel(parent)
{
    this->setFixedSize(128, 128);
    this->setStyleSheet("background-color: #5865F2;"
                        "border-radius: 64px");
    QLabel *icon = new QLabel(this);
    icon->setPixmap(QPixmap("res/images/svg/add-image-icon.svg"));
    icon->setFixedSize(24, 24);
    icon->move(52, 52);
}

void SelectImageButton::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
        emit clicked();
}

} // namespace Ui