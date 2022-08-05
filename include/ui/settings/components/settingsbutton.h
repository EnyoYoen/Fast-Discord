#pragma once

#include "ui/common/basicwidgets.h"

#include <QLabel>
#include <QMouseEvent>

namespace Ui {

class SettingsButton : public Widget
{
    Q_OBJECT
public:
    enum class Type : quint8 {
        NoBackground,
        Edit,
        Normal,
        NormalOutlined,
        Important,
        Critical
    };

    SettingsButton(Type type, QString text, QWidget *parent);

signals:
    void clicked();

private:
    void mouseReleaseEvent(QMouseEvent *) override;
    void mousePressEvent(QMouseEvent *) override;
    void enterEvent(QEvent *) override;
    void leaveEvent(QEvent *) override;

    Label *content;
    Type type;
    bool pressed;
};

} // namespace Ui