#pragma once

#include <QWidget>
#include <QPaintEvent>

namespace Ui {

class StatusIcon : public QWidget
{
public:
    StatusIcon(QWidget *parent);
    void setStatus(const QString& status);

private:
    void paintEvent(QPaintEvent *);
};

} // namespace Ui
