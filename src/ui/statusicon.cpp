#include "ui/statusicon.h"

#include <QPainterPath>
#include <QPainter>

namespace Ui {

StatusIcon::StatusIcon(QWidget *parent)
    : QWidget(parent)
{
    this->setFixedSize(10, 10);
    this->setStyleSheet("background-color: rgb(90, 90, 90);");
}

void StatusIcon::setStatus(const QString& status)
{
    if (status == "online") this->setStyleSheet("background-color: rgb(0, 224, 71);");
    else if (status == "idle") this->setStyleSheet("background-color: rgb(255, 169, 21);");
    else if (status == "dnd") this->setStyleSheet("background-color: rgb(255, 48, 51);");
    else if (status == "offline") this->setStyleSheet("background-color: rgb(90, 90, 90);");
}

void StatusIcon::paintEvent(QPaintEvent *)
{
    // Make the icon rounded with some antialiasing
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    QPainterPath path = QPainterPath();
    path.addRoundedRect(0, 0, 10, 10, 5, 5);
    p.setClipPath(path);
}

} // namespace Ui
