#pragma once

#include <QLabel>

namespace Ui {

class SwitchButton : public QLabel
{
    Q_OBJECT
public:
    SwitchButton(bool state);

    void setState(bool state);

signals:
    void clicked(bool state);

private:
    void mouseReleaseEvent(QMouseEvent *) override;

    QLabel *checkIcon;
    QLabel *crossIcon;
    QLabel *iconBackground;

    bool state;
};

} // namespace Ui