#include "ui/settings/slider.h"

#include <QFont>
#include <QStackedLayout>

namespace Ui {

class Value : public Widget
{
    Q_OBJECT
public:
    Value(QString suffix, int value, int indexp, bool defaultValue, bool selected, QWidget *parent)
        : Widget(parent)
    {
        index = indexp;

        this->setFixedSize(Settings::scale(24), Settings::scale(38));

        QVBoxLayout *layout = new QVBoxLayout(this);
        layout->setSpacing(Settings::scale(4));
        layout->setContentsMargins(0, 0, 0, 0);

        QFont font;
        font.setPixelSize(Settings::scale(10));
        font.setFamily("whitney");

        Label *textValue = new Label(QString::number(value) + suffix, this);
        textValue->setFont(font);
        textValue->setTextColor(defaultValue ? Settings::TextPositive : Settings::TextMuted);
        textValue->setFlags(Qt::AlignCenter);
        textValue->setFixedSize(Settings::scale(24), Settings::scale(10));

        line = new Widget(this);
        line->setBorderColor(Settings::SliderBorder);
        this->setActive(selected);

        layout->addWidget(textValue);
        layout->addWidget(line, 0, Qt::AlignHCenter);
    }

    void setActive(bool active)
    {
        if (active) {
            line->setCursor(Qt::CursorShape::SizeHorCursor);
            line->setBackgroundColor(Settings::White);
            line->setFixedSize(Settings::scale(10), Settings::scale(24));
            line->setBorderRadius(Settings::scale(3));
            line->setBorderSize(Settings::scale(1));
        } else {
            line->setCursor(Qt::CursorShape::ArrowCursor);
            line->setBackgroundColor(Settings::SliderBackground);
            line->setFixedSize(Settings::scale(2), Settings::scale(24));
            line->setBorderRadius(0);
            line->setBorderSize(0);
        }
    }

signals:
    void pressed(int index);

private:
    void mousePressEvent(QMouseEvent *)
    {
        emit pressed(index);
    }

    Widget *line;
    int index;
};

Slider::Slider(QVector<int> valuesp, QString suffix, int defaultValue, int selectedValue, QWidget *parent)
    : Widget(parent)
{
    values = valuesp;
    for (int i = 0 ; i < values.size() ; i++) {
        if (selectedValue == values[i]) {
            index = i;
            break;
        }
    }

    this->setFixedHeight(Settings::scale(40));

    QStackedLayout *layout = new QStackedLayout(this);
    layout->setStackingMode(QStackedLayout::StackingMode::StackAll);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);

    Widget *lineContainer = new Widget(this);

    QVBoxLayout *lineContainerLayout = new QVBoxLayout(lineContainer);
    lineContainerLayout->setContentsMargins(Settings::scale(6), 0, Settings::scale(6), 0);

    Widget *horizontalLine = new Widget(lineContainer);
    horizontalLine->setBackgroundColor(Settings::SliderBackground);
    horizontalLine->setFixedHeight(Settings::scale(8));
    horizontalLine->setBorderRadius(Settings::scale(4));

    lineLayout = new QHBoxLayout(horizontalLine);
    lineLayout->setContentsMargins(0, 0, 0, 0);

    Widget *filledLine = new Widget(horizontalLine);
    filledLine->setBackgroundColor(Settings::BrandExperiment);
    filledLine->setFixedHeight(Settings::scale(8));
    filledLine->setBorderRadius(Settings::scale(4));

    lineLayout->addWidget(filledLine, selectedValue - values[0]);
    lineLayout->addStretch(values[values.size()-1] - selectedValue - 1);

    lineContainerLayout->addSpacing(Settings::scale(22));
    lineContainerLayout->addWidget(horizontalLine);
    lineContainerLayout->addSpacing(Settings::scale(8));

    QWidget *valuesContainer = new QWidget(this);
    valuesContainer->setAttribute(Qt::WA_TranslucentBackground);
    QHBoxLayout *valuesLayout = new QHBoxLayout(valuesContainer);
    valuesLayout->setSpacing(0);
    valuesLayout->setContentsMargins(0, 0, 0, 0);

    int i;
    for (i = 0 ; i < values.size() - 1 ; i++) {
        Value *value = new Value(suffix, values[i], i, values[i] == defaultValue, values[i] == selectedValue, this);
        QObject::connect(value, &Value::pressed, [this](int indexp){
            if (indexp == index) {
                pressed = true;
            } else {
                for (int i = 0 ; i < valueWidgets.size() ; i++)
                    valueWidgets[i]->setActive(false);
                valueWidgets[indexp]->setActive(true);
                index = indexp;

                lineLayout->setStretch(0, values[indexp] - values[0]);
                lineLayout->setStretch(1, values[values.size()-1] - values[indexp] - 1);

                emit valueChanged(values[index]);
            }
        });
        valueWidgets.append(value);
        valuesLayout->addWidget(value);
        valuesLayout->addStretch(values[i + 1] - values[i]);
    }
    Value *value = new Value(suffix, values[i], i, values[i] == defaultValue, values[i] == selectedValue, this);
    QObject::connect(value, &Value::pressed, [this](int indexp){
        if (indexp == index) {
            pressed = true;
        } else {
            for (int i = 0 ; i < valueWidgets.size() ; i++)
                valueWidgets[i]->setActive(false);
            valueWidgets[indexp]->setActive(true);
            index = indexp;

            lineLayout->setStretch(0, values[indexp] - values[0]);
            lineLayout->setStretch(1, values[values.size()-1] - values[indexp] - 1);

            emit valueChanged(values[index]);
        }
    });
    valueWidgets.append(value);
    valuesLayout->addWidget(value);

    layout->addWidget(valuesContainer);
    layout->addWidget(lineContainer);
}

void Slider::mousePressEvent(QMouseEvent *event)
{
    float clicValue = values[0] + event->x() * (values[values.size()-1] - values[0]) / (float)this->width();
    float minimumOffset = abs(values[0] - clicValue);
    int minimumOffsetIndex = 0;
    for (int i = 1 ; i < values.size() ; i++) {
        float offset = abs(values[i] - clicValue);
        if (offset < minimumOffset) {
            minimumOffset = offset;
            minimumOffsetIndex = i;
        }
    }

    if (index != minimumOffsetIndex) {
        for (int i = 0 ; i < valueWidgets.size() ; i++)
            valueWidgets[i]->setActive(false);
        valueWidgets[minimumOffsetIndex]->setActive(true);
        index = minimumOffsetIndex;

        lineLayout->setStretch(0, values[minimumOffsetIndex] - values[0]);
        lineLayout->setStretch(1, values[values.size()-1] - values[minimumOffsetIndex] - 1);

        emit valueChanged(values[index]);
    }
}

void Slider::mouseReleaseEvent(QMouseEvent *)
{
    pressed = false;
}

void Slider::mouseMoveEvent(QMouseEvent *event)
{
    if (pressed) {
        float clicValue = values[0] + event->x() * (values[values.size()-1] - values[0]) / (float)this->width();
        float minimumOffset = abs(values[0] - clicValue);
        int minimumOffsetIndex = 0;
        for (int i = 1 ; i < values.size() ; i++) {
            float offset = abs(values[i] - clicValue);
            if (offset < minimumOffset) {
                minimumOffset = offset;
                minimumOffsetIndex = i;
            }
        }

        if (index != minimumOffsetIndex) {
            for (int i = 0 ; i < valueWidgets.size() ; i++)
                valueWidgets[i]->setActive(false);
            valueWidgets[minimumOffsetIndex]->setActive(true);
            index = minimumOffsetIndex;

            lineLayout->setStretch(0, values[minimumOffsetIndex] - values[0]);
            lineLayout->setStretch(1, values[values.size()-1] - values[minimumOffsetIndex] - 1);

            emit valueChanged(values[index]);
        }
    }
}

} // namespace Ui

#include "slider.moc"