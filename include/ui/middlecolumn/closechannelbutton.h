#pragma once

#include "ui/common/basicwidgets.h"

#include <QWidget>
#include <QMouseEvent>

namespace Ui {

class CloseChannelButton : public Widget
{
    Q_OBJECT
public:
    CloseChannelButton(QWidget *parent);

signals:
    void clicked();

private:
    void mouseReleaseEvent(QMouseEvent *event) override;
};

} // namespace Ui
