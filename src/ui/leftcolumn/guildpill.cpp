#include "ui/leftcolumn/guildpill.h"

#include <QPainter>
#include <QPaintEvent>
#include <QPixmap>

namespace Ui {

GuildPill::GuildPill(QWidget *parent)
    : QLabel(parent)
{
    this->setFixedSize(4, 0);
    this->setStyleSheet("background-color: #FFF;");
}

void GuildPill::setHeight(unsigned int height)
{
    QPixmap image;
    switch (height) {
        case 8:
            image.load("res/images/png/short-pill.png");
            break;

        case 20:
            image.load("res/images/png/hover-pill.png");
            break;

        case 40:
            image.load("res/images/png/long-pill.png");
            break;
    }

    this->setFixedHeight(height);
    this->setPixmap(image);
}

} // namespace Ui
