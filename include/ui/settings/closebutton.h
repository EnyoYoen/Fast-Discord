#pragma once

#include <QLabel>
#include <QMouseEvent>

namespace Ui {

class CloseButton : public QWidget
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

    QLabel  *text;
    QLabel  *icon;
    QWidget *iconContainer;
    bool pressed;
};

} // namespace Ui