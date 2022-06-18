#include "ui/settings/switchbutton.h"

#include <QDebug>

namespace Ui {

SwitchButton::SwitchButton(bool statep)
{
    state = statep;

    iconBackground = new QLabel(this);
    iconBackground->setFixedSize(20, 20);
    iconBackground->setStyleSheet("background-color: #FFF;"
                                  "border-radius: 10px");

    checkIcon = new QLabel(this);
    checkIcon->setFixedSize(20, 20);
    checkIcon->move(17, 2);
    checkIcon->setStyleSheet("background-image: url(\"res/images/svg/switch-check-icon.svg\");"
                             "background-color: none;"
                             "background-position: center");

    crossIcon = new QLabel(this);
    crossIcon->setFixedSize(20, 20);
    crossIcon->move(3, 2);
    crossIcon->setStyleSheet("background-image: url(\"res/images/svg/switch-cross-icon.svg\");"
                             "background-color: none;"
                             "background-position: center");

    if (state) {
        iconBackground->move(17, 2);
        crossIcon->hide();
        this->setStyleSheet("background-color: #3BA55D;"
                            "border-radius: 12px");
    } else {
        iconBackground->move(3, 2);
        checkIcon->hide();
        this->setStyleSheet("background-color: #72767D;"
                            "border-radius: 12px");
    }

    this->setFixedSize(40, 24);
}

void SwitchButton::setState(bool statep)
{
    this->state = statep;
    if (state) {
        iconBackground->move(17, 2);
        crossIcon->hide();
        checkIcon->show();
        this->setStyleSheet("background-color: #3BA55D;"
                            "border-radius: 12px");
    } else {
        iconBackground->move(3, 2);
        checkIcon->hide();
        crossIcon->show();
        this->setStyleSheet("background-color: #72767D;"
                            "border-radius: 12px");
    }
}

void SwitchButton::mouseReleaseEvent(QMouseEvent *event)
{
    if (state) {
        state = false;
        iconBackground->move(3, 2);
        checkIcon->hide();
        crossIcon->show();
        this->setStyleSheet("background-color: #72767D;"
                            "border-radius: 12px");
    } else {
        state = true;
        iconBackground->move(17, 2);
        crossIcon->hide();
        checkIcon->show();
        this->setStyleSheet("background-color: #3BA55D;"
                            "border-radius: 12px");
    }
    
    emit clicked(state);
}

} // namespace Ui