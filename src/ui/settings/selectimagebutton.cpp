#include "ui/settings/selectimagebutton.h"

namespace Ui {

SelectImageButton::SelectImageButton(QWidget *parent)
    : Label(parent)
{
    this->setFixedSize(128, 128);
    this->setBorderRadius(64);
    this->setBackgroundColor(Settings::BrandExperiment);
    Widget *icon = new Widget(this);
    icon->setImage("res/images/svg/add-image-icon.svg");
    icon->setFixedSize(24, 24);
    icon->move(52, 52);
}

void SelectImageButton::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
        emit clicked();
}

} // namespace Ui