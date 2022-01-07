#include "ui/closechannelbutton.h"

#include <QHBoxLayout>

namespace Ui {

CloseChannelButton::CloseChannelButton(QWidget *parent)
    : QWidget(parent)
{
    QWidget *container = new QWidget(this);

    container->setFixedSize(16, 38);
    container->setStyleSheet("background-image: url(res/images/svg/close-icon.svg);"
                             "background-repeat: no-repeat;"
                             "background-position: center;");
}

void CloseChannelButton::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        emit clicked();
    }
}

} // namespace Ui
