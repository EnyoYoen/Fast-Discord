#pragma once

#include "ui/common/basicwidgets.h"

#include <QWidget>
#include <QLabel>
#include <QTextEdit>

namespace Ui {

class PopUp : public Widget
{
    Q_OBJECT
public:
    PopUp(Widget *content, int maximumWidth, int maximumHeight, QString icon, 
        QString title, bool titleCentered, bool closeable, QString htmlDescription, 
        QString cancelButtonStr, QString doneButtonStr, bool controlButtons, 
        bool horizontalButtons, QSize winSize, QWidget *parent);

signals:
    void cancelled();
    void done();

private:
    void mouseReleaseEvent(QMouseEvent *) override;

    Widget *actualPopup;
};

} // namespace Ui