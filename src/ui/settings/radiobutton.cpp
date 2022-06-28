#include "ui/settings/radiobutton.h"

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
        layout->setSpacing(10);

        Widget *color = new Widget(this);
        color->move(0, 0);
        color->setFixedSize(3, 62);
        color->setBorderRadius(4, 0, 4, 0);
        color->setBackgroundColor(parameters.color);

        circle = new Widget(this);
        circle->setFixedSize(20, 20);
        circle->setBorderRadius(10);
        circle->setBorderSize(2);

        activeCircle = new Widget(circle);
        activeCircle->setFixedSize(10, 10);
        activeCircle->move(5, 5);
        activeCircle->setBorderRadius(5);
        activeCircle->setBackgroundColor(Settings::InteractiveActive);

        
        Widget *text = new Widget(this);
        text->setFixedHeight(42);
        QVBoxLayout *textLayout = new QVBoxLayout(text);
        textLayout->setContentsMargins(0, 0, 0, 0);
        textLayout->setSpacing(4);

        QFont font;
        font.setPixelSize(16);
        font.setFamily("whitney");

        title = new Label(parameters.title, text);
        title->setFont(font);
        title->setFixedSize(QFontMetrics(font).width(parameters.title), 20);
        font.setPixelSize(14);
        description = new Label(parameters.description, text);
        description->setFixedSize(QFontMetrics(font).width(parameters.description), 18);
        description->setFont(font);

        textLayout->addWidget(title);
        textLayout->addWidget(description);


        layout->addWidget(color);
        layout->addWidget(circle);
        layout->addWidget(text, 0, Qt::AlignVCenter);
        layout->addStretch(1);

        this->setFixedHeight(62); 
        this->setBorderRadius(4);

        if (active) {
            circle->setBorderColor(Settings::InteractiveActive);
            title->setTextColor(Settings::InteractiveActive);
            description->setTextColor(Settings::InteractiveActive);
            this->setBackgroundColor(Settings::BackgroundModifierSelected);
        } else {
            activeCircle->hide();
            circle->setBorderColor(Settings::InteractiveNormal);
            title->setTextColor(Settings::InteractiveNormal);
            description->setTextColor(Settings::InteractiveNormal);
            this->setBackgroundColor(Settings::BackgroundSecondary);
        }
    }

    void setUnclicked()
    {
        active = false;
        activeCircle->hide();
        circle->setBorderColor(Settings::InteractiveNormal);
        title->setTextColor(Settings::InteractiveNormal);
        description->setTextColor(Settings::InteractiveNormal);
        this->setBackgroundColor(Settings::BackgroundSecondary);
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
        description->setTextColor(Settings::InteractiveActive);
        this->setBackgroundColor(Settings::BackgroundModifierSelected);
    }
    void enterEvent(QEvent *) override
    {
        if (!active) {
            circle->setBorderColor(Settings::InteractiveHover);
            title->setTextColor(Settings::InteractiveHover);
            description->setTextColor(Settings::InteractiveHover);
            this->setBackgroundColor(Settings::BackgroundModifierHover);
        }
    }
    void leaveEvent(QEvent *) override
    {
        if (!active) {
            circle->setBorderColor(Settings::InteractiveNormal);
            title->setTextColor(Settings::InteractiveNormal);
            description->setTextColor(Settings::InteractiveNormal);
            this->setBackgroundColor(Settings::BackgroundSecondary);
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
    QVBoxLayout *layout = new QVBoxLayout(this);
    this->setFixedHeight(radios.size() * 70 - 8);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(8);

    for (unsigned int i = 0 ; i < radios.size() ; i++) {
        RadioButton *button = new RadioButton(radios[i], i, selectedIndex == i, this);
        layout->addWidget(button);
        buttons.append(button);

        QObject::connect(button, &RadioButton::clicked, [this](int index){
            for (unsigned int i = 0 ; i < buttons.size() ; i++) {
                if (index != buttons[i]->index)
                    buttons[i]->setUnclicked();
            }
            emit clicked(index);
        });
    }
}

} // namespace Ui

#include "radiobutton.moc"