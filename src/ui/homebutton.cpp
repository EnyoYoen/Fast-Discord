#include "ui/homebutton.h"

#include <QPixmap>
#include <QMouseEvent>

namespace Ui {

HomeButton::HomeButton(QWidget *parent) : QLabel(parent)
{
    setContentsMargins(0, 12, 0, 0);
    clic = true;
    setStyleSheet("background-color: #5865F2;"
                  "border: none;"
                  "border-radius: 16px;"
                  "background-image: url(res/images/svg/home-icon.svg);"
                  "background-repeat: no-repeat;"
                  "background-position: center;");
}

void HomeButton::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        emit clicked();
    }
}

void HomeButton::unclicked()
{
    if (clic) {
        clic = false;
        setStyleSheet("background-color: #36393f;"
                      "border: none;"
                      "border-radius: 24px;"
                      "background-image: url(res/images/svg/home-icon.svg);"
                      "background-repeat: no-repeat;"
                      "background-position: center;");
    }
}

void HomeButton::mousePressEvent(QMouseEvent *)
{
    if (!clic) {
        setStyleSheet("background-color: #5865F2;"
                      "border: none;"
                      "border-radius: 16px;"
                      "background-image: url(res/images/svg/home-icon.svg);"
                      "background-repeat: no-repeat;"
                      "background-position: center;");
        clic = true;
    }
}

void HomeButton::enterEvent(QEvent *)
{
    if (!clic) {
        setStyleSheet("background-color: #5865F2;"
                      "border: none;"
                      "border-radius: 16px;"
                      "background-image: url(res/images/svg/home-icon.svg);"
                      "background-repeat: no-repeat;"
                      "background-position: center;");
    }
}

void HomeButton::leaveEvent(QEvent *)
{
    if (!clic) {
        setStyleSheet("background-color: #36393f;"
                      "border: none;"
                      "border-radius: 24px;"
                      "background-image: url(res/images/svg/home-icon.svg);"
                      "background-repeat: no-repeat;"
                      "background-position: center;");
    }
}

} // namespace Ui
