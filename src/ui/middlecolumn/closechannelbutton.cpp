#include "ui/middlecolumn/closechannelbutton.h"

#include "settings/settings.h"

#include <QHBoxLayout>

namespace Ui {

CloseChannelButton::CloseChannelButton(QWidget *parent)
    : Widget(parent)
{
    this->setFixedSize(Settings::scale(16), Settings::scale(38));
    this->setPixmap(QPixmap("res/images/svg/close-icon.svg"));
}

void CloseChannelButton::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        emit clicked();
    }
}

} // namespace Ui
