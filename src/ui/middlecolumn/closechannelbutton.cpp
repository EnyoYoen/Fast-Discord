#include "ui/middlecolumn/closechannelbutton.h"

#include "settings/settings.h"

#include <QHBoxLayout>

namespace Ui {

CloseChannelButton::CloseChannelButton(QWidget *parent)
    : Widget(parent)
{
    this->setFixedSize(16, 38);
    this->setPixmap(QPixmap("res/images/svg/close-icon.svg"));
}

void CloseChannelButton::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        emit clicked();
    }
}

} // namespace Ui
