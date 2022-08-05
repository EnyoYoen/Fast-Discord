#pragma once

#include "ui/common/basicwidgets.h"

#include <QLabel>

namespace Ui {

struct RadioParameters
{
    QString title;
    QString description;
    Settings::ColorEnum color;
};

class RadioButton;

class RadioGroup : public Widget
{
    Q_OBJECT
public:
    RadioGroup(QVector<RadioParameters> radios, int selectedIndex, QWidget *parent);

signals:
    void clicked(int index);

private:
    QVector<RadioButton *> buttons;
};

} // namespace Ui