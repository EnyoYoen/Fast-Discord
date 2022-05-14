#include "ui/middlecolumn/closecallbutton.h"

namespace Ui {

CloseCallButton::CloseCallButton(Api::RessourceManager *rmp, QWidget *parent)
    : QLabel(parent)
{
    rm = rmp;

    this->setFixedSize(32, 32);
    this->setCursor(Qt::PointingHandCursor);
    this->setStyleSheet("background-image: url(\"res/images/svg/close-call-icon.svg\");"
                        "background-repeat: no-repeat;"
                        "background-position: center;"
                        "border-radius: 4px;"
                        "border: none");
}

void CloseCallButton::mouseReleaseEvent(QMouseEvent *)
{
    emit clicked();
    rm->stopCall();
}

void CloseCallButton::enterEvent(QEvent *)
{
    this->setStyleSheet("background-image: url(\"res/images/svg/close-call-icon.svg\");"
                        "background-repeat: no-repeat;"
                        "background-position: center;"
                        "border-radius: 4px;"
                        "background-color: #393D43;"
                        "border: none");
}

void CloseCallButton::leaveEvent(QEvent *)
{
    this->setStyleSheet("background-image: url(\"res/images/svg/close-call-icon.svg\");"
                        "background-repeat: no-repeat;"
                        "background-position: center;"
                        "border-radius: 4px;"
                        "background-color: none;"
                        "border: none");
}


} // namespace Ui