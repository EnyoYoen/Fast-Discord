#pragma once

#include "ui/common/basicwidgets.h"

#include <QLabel>
#include <QMouseEvent>

namespace Ui {

class CloseButton : public Widget
{
    Q_OBJECT
public:
    CloseButton(QWidget *parent);

signals:
    void clicked();

private:
    void mouseReleaseEvent(QMouseEvent *) override;
    void mousePressEvent(QMouseEvent *) override;
    void enterEvent(QEvent *) override;
    void leaveEvent(QEvent *) override;

    Label  *text;
    Widget *icon;
    Widget *iconContainer;
    bool pressed;
};

} // namespace Ui