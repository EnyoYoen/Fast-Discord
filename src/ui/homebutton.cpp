#include "ui/homebutton.h"

#include <QHBoxLayout>
#include <QPixmap>
#include <QMouseEvent>

namespace Ui {

HomeButton::HomeButton(QWidget *parent)
    : QWidget(parent)
{
    // Attribute initialization (the home page is shown at the start)
    clic = true;

    QHBoxLayout *layout = new QHBoxLayout(this);
    image = new QLabel(this);
    pill = new GuildPill(this);

    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(8);
    layout->addWidget(pill);
    layout->addWidget(image);

    // Style
    this->setFixedSize(60, 48);
    pill->setHeight(40);
    image->setFixedSize(48, 48);
    image->setStyleSheet("background-color: #5865F2;"
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
        image->setStyleSheet("background-color: #36393f;"
                             "border: none;"
                             "border-radius: 24px;"
                             "background-image: url(res/images/svg/home-icon.svg);"
                             "background-repeat: no-repeat;"
                             "background-position: center;");
    }
    pill->setHeight(0);
}

void HomeButton::mousePressEvent(QMouseEvent *)
{
    // Widget clicked : change the stylesheet
    if (!clic) {
        image->setStyleSheet("background-color: #5865F2;"
                             "border: none;"
                             "border-radius: 16px;"
                             "background-image: url(res/images/svg/home-icon.svg);"
                             "background-repeat: no-repeat;"
                             "background-position: center;");
        clic = true;
    }
    pill->setHeight(40);
}

void HomeButton::enterEvent(QEvent *)
{
    // Mouse hover : change the stylesheet
    if (!clic) {
        image->setStyleSheet("background-color: #5865F2;"
                             "border: none;"
                             "border-radius: 16px;"
                             "background-image: url(res/images/svg/home-icon.svg);"
                             "background-repeat: no-repeat;"
                             "background-position: center;");
    }
    if (pill->height() != 40) pill->setHeight(20);
}

void HomeButton::leaveEvent(QEvent *)
{
    // Reset the stylesheet if not clicked
    if (!clic) {
        image->setStyleSheet("background-color: #36393f;"
                             "border: none;"
                             "border-radius: 24px;"
                             "background-image: url(res/images/svg/home-icon.svg);"
                             "background-repeat: no-repeat;"
                             "background-position: center;");
    }
    if (pill->height() != 40) pill->setHeight(0);
}

} // namespace Ui
