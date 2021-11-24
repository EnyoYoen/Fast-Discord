#include "ui/homebutton.h"

#include <QPixmap>
#include <QMouseEvent>

namespace Ui {

HomeButton::HomeButton(QWidget *parent) : QLabel(parent)
{
    // Attribute initialization (the home page is shown at the start)
    clic = true;

    // Style
    this->setFixedSize(48, 48);
    this->setContentsMargins(0, 12, 0, 0);
    this->setStyleSheet("background-color: #5865F2;"
                  "border: none;"
                  "border-radius: 16px;"
                  "background-image: url(res/images/svg/home-icon.svg);"
                  "background-repeat: no-repeat;"
                  "background-position: center;");
}

void HomeButton::mouseReleaseEvent(QMouseEvent *event)
{
    // Emit signals when clicked to open the home page
    if (event->button() == Qt::LeftButton) {
        emit clicked();
    }
}

void HomeButton::unclicked()
{
    // Reset the stylesheet of this widget if currently clicked
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
    // Widget clicked : change the stylesheet
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
    // Mouse hover : change the stylesheet
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
    // Reset the stylesheet if not clicked
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
