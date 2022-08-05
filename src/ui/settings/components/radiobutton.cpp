#include "ui/settings/components/radiobutton.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMouseEvent>

namespace Ui {

class RadioButton : public Widget
{
    Q_OBJECT
public:
    RadioButton(RadioParameters parameters, int indexp, bool activep, QWidget *parent)
        : Widget(parent)
    {
        index = indexp;
        active = activep;

        QHBoxLayout *layout = new QHBoxLayout(this);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(Settings::scale(10));

        Widget *color = new Widget(this);
        color->move(0, 0);
        color->setFixedSize(Settings::scale(3), Settings::scale(62));
        color->setBorderRadius(Settings::scale(4), 0, Settings::scale(4), 0);
        color->setBackgroundColor(parameters.color);

        circle = new Widget(this);
        circle->setFixedSize(Settings::scale(20), Settings::scale(20));
        circle->setBorderRadius(Settings::scale(10));
        circle->setBorderSize(Settings::scale(2));

        activeCircle = new Widget(circle);
        activeCircle->setFixedSize(Settings::scale(10), Settings::scale(10));
        activeCircle->move(Settings::scale(5), Settings::scale(5));
        activeCircle->setBorderRadius(Settings::scale(5));
        activeCircle->setBackgroundColor(Settings::InteractiveActive);

        
        Widget *text = new Widget(this);
        text->setFixedHeight(Settings::scale(42));
        QVBoxLayout *textLayout = new QVBoxLayout(text);
        textLayout->setContentsMargins(0, 0, 0, 0);
        textLayout->setSpacing(Settings::scale(4));

        QFont font;
        font.setPixelSize(Settings::scale(16));
        font.setFamily("whitney");

        title = new Label(parameters.title, text);
        title->setFont(font);
        title->setFixedSize(QFontMetrics(font).horizontalAdvance(parameters.title), Settings::scale(20));
        textLayout->addWidget(title);

        if (!parameters.description.isEmpty()) {
            font.setPixelSize(Settings::scale(14));
            description = new Label(parameters.description, text);
            description->setFixedSize(QFontMetrics(font).horizontalAdvance(parameters.description), Settings::scale(18));
            description->setFont(font);
            textLayout->addWidget(description);
        } else {
            description = nullptr;
        }


        layout->addWidget(color);
        layout->addWidget(circle);
        layout->addWidget(text, 0, Qt::AlignVCenter);
        layout->addStretch(1);

        this->setFixedHeight(parameters.description.isEmpty() ? Settings::scale(46) : Settings::scale(62)); 
        this->setBorderRadius(Settings::scale(4));

        if (active) {
            circle->setBorderColor(Settings::InteractiveActive);
            title->setTextColor(Settings::InteractiveActive);
            this->setBackgroundColor(Settings::BackgroundModifierSelected);
            if (description)
                description->setTextColor(Settings::InteractiveActive);
        } else {
            activeCircle->hide();
            circle->setBorderColor(Settings::InteractiveNormal);
            title->setTextColor(Settings::InteractiveNormal);
            this->setBackgroundColor(Settings::BackgroundSecondary);
            if (description)
                description->setTextColor(Settings::InteractiveNormal);
        }
    }

    void setUnclicked()
    {
        active = false;
        activeCircle->hide();
        circle->setBorderColor(Settings::InteractiveNormal);
        title->setTextColor(Settings::InteractiveNormal);
        this->setBackgroundColor(Settings::BackgroundSecondary);
        if (description)
            description->setTextColor(Settings::InteractiveNormal);
    }

    int index;

signals:
    void clicked(int index);

private:
    void mouseReleaseEvent(QMouseEvent *event) override
    {
        if (event->button() == Qt::LeftButton)
            emit clicked(index);
    }
    void mousePressEvent(QMouseEvent *) override
    {
        active = true;
        activeCircle->show();
        circle->setBorderColor(Settings::InteractiveActive);
        title->setTextColor(Settings::InteractiveActive);
        this->setBackgroundColor(Settings::BackgroundModifierSelected);
        if (description)
            description->setTextColor(Settings::InteractiveActive);
    }
    void enterEvent(QEvent *) override
    {
        if (!active) {
            circle->setBorderColor(Settings::InteractiveHover);
            title->setTextColor(Settings::InteractiveHover);
            this->setBackgroundColor(Settings::BackgroundModifierHover);
            if (description)
                description->setTextColor(Settings::InteractiveHover);
        }
    }
    void leaveEvent(QEvent *) override
    {
        if (!active) {
            circle->setBorderColor(Settings::InteractiveNormal);
            title->setTextColor(Settings::InteractiveNormal);
            this->setBackgroundColor(Settings::BackgroundSecondary);
            if (description)
                description->setTextColor(Settings::InteractiveNormal);
        }
    }

    Widget *activeCircle;
    Widget *circle;
    Label *title;
    Label *description;

    bool active;
};


RadioGroup::RadioGroup(QVector<RadioParameters> radios, int selectedIndex, QWidget *parent)
    : Widget(parent)
{
    int c = 0;
    for (int i = 0 ; i < radios.size() ; i++) {
        if (!radios[i].description.isEmpty())
            c++;
    }

    QVBoxLayout *layout = new QVBoxLayout(this);
    this->setFixedHeight(Settings::scale(radios.size() * 54 + c * 16 - 8));
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(Settings::scale(8));

    for (int i = 0 ; i < radios.size() ; i++) {
        RadioButton *button = new RadioButton(radios[i], i, selectedIndex == i, this);
        layout->addWidget(button);
        buttons.append(button);

        QObject::connect(button, &RadioButton::clicked, [this](int index){
            for (int i = 0 ; i < buttons.size() ; i++) {
                if (index != buttons[i]->index)
                    buttons[i]->setUnclicked();
            }
            emit clicked(index);
        });
    }
}

} // namespace Ui

#include "radiobutton.moc"