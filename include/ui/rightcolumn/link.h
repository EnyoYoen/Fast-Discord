#pragma once

#include "settings/settings.h"
#include "ui/common/basicwidgets.h"

#include <QMouseEvent>

namespace Ui {

class Link : public Label
{
public:
    Link(QString text, const QString& url, int fontSize, Settings::ColorEnum color, QWidget *parent);

private:
    void mouseReleaseEvent(QMouseEvent *event) override;
    void enterEvent(QEvent *) override;
    void leaveEvent(QEvent *) override;

    QString url;
    QFont font;
};

} // namespace Ui
