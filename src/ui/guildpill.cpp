#include "ui/guildpill.h"

#include <QPainter>
#include <QPaintEvent>

namespace Ui {

GuildPill::GuildPill(QWidget *parent)
    : QWidget(parent)
{
    this->setFixedSize(4, 0);
    this->setStyleSheet("background-color: #FFF;");
}

void GuildPill::setHeight(unsigned int height)
{
    this->setFixedHeight(height);
}

void GuildPill::paintEvent(QPaintEvent *)
{
    int h = height();
    if (h != 0) {
        QPainter p(this);
        p.setRenderHint(QPainter::Antialiasing);
        p.setRenderHint(QPainter::SmoothPixmapTransform);

        p.drawArc(0, 0, 4, 4, 0, 90*16);
        p.drawArc(0, h - 4, 4, 4, 0, -90*16);

        p.drawArc(1, 0, 4, 4, 0, 90*16);
        p.drawArc(1, h - 4, 4, 4, 0, -90*16);
    }
}

} // namespace Ui
