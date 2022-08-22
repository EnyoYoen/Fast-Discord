#include "ui/rightcolumn/link.h"

#include <QDesktopServices>

namespace Ui {

Link::Link(QString text, const QString& urlp, int fontSize, Settings::ColorEnum color, QWidget *parent)
    : Label(text, parent)
{
    url = urlp;

    font.setFamily("whitney");
    font.setPixelSize(fontSize);

    this->setFont(font);
    this->setTextColor(color);
    this->setCursor(Qt::CursorShape::PointingHandCursor);
}

void Link::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
        QDesktopServices::openUrl(url);
}

void Link::enterEvent(QEvent *)
{
    font.setUnderline(true);
    this->setFont(font);
}

void Link::leaveEvent(QEvent *)
{
    font.setUnderline(false);
    this->setFont(font);
}

} // namespace Ui