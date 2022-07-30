#include "ui/middlecolumn/closecallbutton.h"

namespace Ui {

CloseCallButton::CloseCallButton(Api::RessourceManager *rmp, QWidget *parent)
    : Widget(parent)
{
    rm = rmp;

    this->setFixedSize(Settings::scale(32), Settings::scale(32));
    this->setCursor(Qt::PointingHandCursor);
    this->setImage("res/images/svg/close-call-icon.svg");
    this->setBorderRadius(Settings::scale(4));
}

void CloseCallButton::mouseReleaseEvent(QMouseEvent *)
{
    emit clicked();
    rm->stopCall();
}

void CloseCallButton::enterEvent(QEvent *)
{
    this->setBackgroundColor(Settings::BackgroundModifierSelected);
}

void CloseCallButton::leaveEvent(QEvent *)
{
    this->setBackgroundColor(Settings::None);
}


} // namespace Ui