#include "ui/leftcolumn/guildpill.h"

#include <QPainter>
#include <QPaintEvent>
#include <QPixmap>

namespace Ui {

GuildPill::GuildPill(QWidget *parent)
    : Widget(parent)
{
    this->setFixedSize(4, 0);
    this->setBorderRadius(0, 4, 0, 4);
    this->setBackgroundColor(Settings::HeaderPrimary);
}

} // namespace Ui
