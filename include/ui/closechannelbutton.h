#pragma once

#include <QWidget>
#include <QMouseEvent>

namespace Ui {

class CloseChannelButton : public QWidget
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
