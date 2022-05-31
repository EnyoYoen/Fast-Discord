#include "ui/settings/divider.h"

namespace Ui {

Divider::Divider()
    : QWidget()
{
    this->setFixedSize(660, 81);
    this->setContentsMargins(0, 40, 0, 40);
    QWidget *line = new QWidget(this);
    line->move(0, 41);
    line->setFixedSize(660, 1);
    line->setStyleSheet("background-color: rgba(79, 84, 92, 0.48);");
}

} // namespace Ui