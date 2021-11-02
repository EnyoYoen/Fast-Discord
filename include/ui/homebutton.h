#pragma once

#include <QLabel>

namespace Ui {

class HomeButton : public QLabel
{
    Q_OBJECT

public:
    HomeButton(QWidget *parent = nullptr);
    void unclicked();

signals:
    void clicked();

private:
    void mouseReleaseEvent(QMouseEvent *) override;
    void mousePressEvent(QMouseEvent *) override;
    void enterEvent(QEvent *) override;
    void leaveEvent(QEvent *) override;

    bool clic;
};

} // namespace Ui
