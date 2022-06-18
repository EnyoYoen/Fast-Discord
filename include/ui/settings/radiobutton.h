#pragma once

#include <QLabel>

namespace Ui {

struct RadioParameters
{
    QString title;
    QString description;
    QString color;

    static constexpr const char *Green  = "#3BA55D";
    static constexpr const char *Orange = "#CB8515";
    static constexpr const char *Red    = "#ED4245";
};

class RadioButton;

class RadioGroup : public QLabel
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