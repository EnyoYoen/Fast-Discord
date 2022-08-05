#include "ui/settings/components/selectimagebutton.h"

namespace Ui {

SelectImageButton::SelectImageButton(QWidget *parent)
    : Label(parent)
{
    this->setFixedSize(Settings::scale(128), Settings::scale(128));
    this->setBorderRadius(Settings::scale(64));
    this->setBackgroundColor(Settings::BrandExperiment);
    Widget *icon = new Widget(this);
    icon->setImage("res/images/svg/add-image-icon.svg");
    icon->setFixedSize(Settings::scale(24), Settings::scale(24));
    icon->move(Settings::scale(52), Settings::scale(52));
}

void SelectImageButton::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
        emit clicked();
}

} // namespace Ui