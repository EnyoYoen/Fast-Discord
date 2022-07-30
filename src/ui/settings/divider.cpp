#include "ui/settings/divider.h"

#include "settings/settings.h"

namespace Ui {

Divider::Divider()
    : QWidget()
{
    this->setFixedSize(Settings::scale(660), Settings::scale(81));
    this->setContentsMargins(0, Settings::scale(40), 0, Settings::scale(40));
    QWidget *line = new QWidget(this);
    line->move(0, Settings::scale(41));
    line->setFixedSize(Settings::scale(660), Settings::scale(1));
    line->setStyleSheet("background-color: rgba(79, 84, 92, 0.48);");
}

} // namespace Ui