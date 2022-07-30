#pragma once

#include "ui/common/basicwidgets.h"

#include <QMouseEvent>

namespace Ui {

class Value;

class Slider : public Widget
{
    Q_OBJECT
public:
    Slider(QVector<int> values, QString suffix, int defaultValue, int selectedValue, QWidget *parent);

signals:
    void valueChanged(int value);

private:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *event);

    void setActive(int index);

    QVector<Value *> valueWidgets;
    QHBoxLayout *lineLayout;

    QVector<int> values;
    int index;
    bool pressed = false;
};

} // namespace Ui