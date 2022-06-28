#include "ui/settings/switchbutton.h"

#include <QDebug>

namespace Ui {

SwitchButton::SwitchButton(bool statep)
{
    state = statep;

    iconBackground = new Widget(this);
    iconBackground->setFixedSize(20, 20);
    iconBackground->setBackgroundColor(Settings::White);
    iconBackground->setBorderRadius(10);

    checkIcon = new Widget(this);
    checkIcon->setFixedSize(20, 20);
    checkIcon->move(17, 2);
    checkIcon->setImage("res/images/svg/switch-check-icon.svg");

    crossIcon = new Widget(this);
    crossIcon->setFixedSize(20, 20);
    crossIcon->move(3, 2);
    crossIcon->setImage("res/images/svg/switch-cross-icon.svg");

    if (state) {
        iconBackground->move(17, 2);
        crossIcon->hide();
        iconBackground->setBackgroundColor(Settings::White);
        iconBackground->setBorderRadius(10);
        this->setBackgroundColor(Settings::SwitchActive);
    } else {
        iconBackground->move(3, 2);
        checkIcon->hide();
        this->setBackgroundColor(Settings::SwitchInactive);
    }

    this->setBorderRadius(12);
    this->setFixedSize(40, 24);
    this->setCursor(Qt::CursorShape::PointingHandCursor);
}

void SwitchButton::setState(bool statep)
{
    this->state = statep;
    if (state) {
        iconBackground->move(17, 2);
        crossIcon->hide();
        checkIcon->show();
        this->setBackgroundColor(Settings::SwitchActive);
    } else {
        iconBackground->move(3, 2);
        checkIcon->hide();
        crossIcon->show();
        this->setBackgroundColor(Settings::SwitchInactive);
    }
}

void SwitchButton::mouseReleaseEvent(QMouseEvent *event)
{
    if (state) {
        state = false;
        iconBackground->move(3, 2);
        checkIcon->hide();
        crossIcon->show();
        this->setBackgroundColor(Settings::SwitchInactive);
    } else {
        state = true;
        iconBackground->move(17, 2);
        crossIcon->hide();
        checkIcon->show();
        this->setBackgroundColor(Settings::SwitchActive);
    }
    
    emit clicked(state);
}

} // namespace Ui