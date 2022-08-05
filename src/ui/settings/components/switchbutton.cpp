#include "ui/settings/components/switchbutton.h"

#include <QDebug>

namespace Ui {

SwitchButton::SwitchButton(bool statep)
{
    state = statep;

    iconBackground = new Widget(this);
    iconBackground->setFixedSize(Settings::scale(20), Settings::scale(20));
    iconBackground->setBackgroundColor(Settings::White);
    iconBackground->setBorderRadius(Settings::scale(10));

    checkIcon = new Widget(this);
    checkIcon->setFixedSize(Settings::scale(20), Settings::scale(20));
    checkIcon->move(Settings::scale(17), Settings::scale(2));
    checkIcon->setImage(":switch-check-icon.svg");

    crossIcon = new Widget(this);
    crossIcon->setFixedSize(Settings::scale(20), Settings::scale(20));
    crossIcon->move(Settings::scale(3), Settings::scale(2));
    crossIcon->setImage(":switch-cross-icon.svg");

    if (state) {
        iconBackground->move(Settings::scale(17), Settings::scale(2));
        crossIcon->hide();
        iconBackground->setBackgroundColor(Settings::White);
        iconBackground->setBorderRadius(Settings::scale(10));
        this->setBackgroundColor(Settings::SwitchActive);
    } else {
        iconBackground->move(Settings::scale(3), Settings::scale(2));
        checkIcon->hide();
        this->setBackgroundColor(Settings::SwitchInactive);
    }

    this->setBorderRadius(Settings::scale(12));
    this->setFixedSize(Settings::scale(40), Settings::scale(24));
    this->setCursor(Qt::CursorShape::PointingHandCursor);
}

void SwitchButton::setState(bool statep)
{
    this->state = statep;
    if (state) {
        iconBackground->move(Settings::scale(17), Settings::scale(2));
        crossIcon->hide();
        checkIcon->show();
        this->setBackgroundColor(Settings::SwitchActive);
    } else {
        iconBackground->move(Settings::scale(3), Settings::scale(2));
        checkIcon->hide();
        crossIcon->show();
        this->setBackgroundColor(Settings::SwitchInactive);
    }
}

void SwitchButton::mouseReleaseEvent(QMouseEvent *event)
{
    if (state) {
        state = false;
        iconBackground->move(Settings::scale(3), Settings::scale(2));
        checkIcon->hide();
        crossIcon->show();
        this->setBackgroundColor(Settings::SwitchInactive);
    } else {
        state = true;
        iconBackground->move(Settings::scale(17), Settings::scale(2));
        crossIcon->hide();
        checkIcon->show();
        this->setBackgroundColor(Settings::SwitchActive);
    }
    
    emit clicked(state);
}

} // namespace Ui