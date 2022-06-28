#pragma once

#include "ui/common/basicwidgets.h"

#include <QLabel>

namespace Ui {

class SwitchButton : public Widget
{
    Q_OBJECT
public:
    SwitchButton(bool state);

    void setState(bool state);

signals:
    void clicked(bool state);

private:
    void mouseReleaseEvent(QMouseEvent *) override;

    Widget *checkIcon;
    Widget *crossIcon;
    Widget *iconBackground;

    bool state;
};

} // namespace Ui