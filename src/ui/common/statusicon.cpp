#include "ui/common/statusicon.h"
#include "settings/settings.h"

#include <QPainterPath>
#include <QPainter>

namespace Ui {

StatusIcon::StatusIcon(QWidget *parent)
    : Widget(parent)
{
    this->setFixedSize(Settings::scale(10), Settings::scale(10));
    this->setBackgroundColor(Settings::StatusOffline);
    this->setBorderRadius(Settings::scale(5));
}

void StatusIcon::setStatus(const QString& status)
{
    if (status == "online") this->setBackgroundColor(Settings::StatusOnline);
    else if (status == "idle") this->setBackgroundColor(Settings::StatusIdle);
    else if (status == "dnd") this->setBackgroundColor(Settings::StatusDND);
    else if (status == "offline") this->setBackgroundColor(Settings::StatusOffline);
}

} // namespace Ui
