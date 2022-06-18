#include "ui/settings/radiobutton.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMouseEvent>

namespace Ui {

class RadioButton : public QLabel
{
    Q_OBJECT
public:
    RadioButton(RadioParameters parameters, int indexp, bool activep, QWidget *parent)
        : QLabel(parent)
    {
        index = indexp;
        active = activep;

        QHBoxLayout *layout = new QHBoxLayout(this);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(10);

        QLabel *color = new QLabel(this);
        color->move(0, 0);
        color->setFixedSize(3, 62);
        color->setStyleSheet("background-color:" + parameters.color);

        circle = new QLabel(this);
        circle->setFixedSize(20, 20);

        activeCircle = new QLabel(circle);
        activeCircle->setFixedSize(10, 10);
        activeCircle->move(5, 5);
        activeCircle->setStyleSheet("background-color: #FFF;"
                                    "border-radius: 5px");

        
        QWidget *text = new QWidget(this);
        text->setStyleSheet("background-color: none");
        text->setFixedHeight(42);
        QVBoxLayout *textLayout = new QVBoxLayout(text);
        textLayout->setContentsMargins(0, 0, 0, 0);
        textLayout->setSpacing(4);

        QFont font;
        font.setPixelSize(16);
        font.setFamily("whitney");

        title = new QLabel(parameters.title, text);
        title->setFont(font);
        font.setPixelSize(14);
        description = new QLabel(parameters.description, text);
        description->setFont(font);

        textLayout->addWidget(title);
        textLayout->addWidget(description);


        layout->addWidget(color);
        layout->addWidget(circle);
        layout->addWidget(text, 0, Qt::AlignVCenter);
        layout->addStretch(1);

        this->setFixedHeight(62);                  

        if (active) {
            circle->setStyleSheet("border: 2px solid #FFF;"
                                  "border-radius: 10px");
            title->setStyleSheet("color: #FFF");
            description->setStyleSheet("color: #FFF");
            this->setStyleSheet("background-color: rgba(79, 84, 92, 0.6);"
                                "border-radius: 3px");
        } else {
            activeCircle->hide();
            circle->setStyleSheet("border: 2px solid #B9BBBE;"
                                "border-radius: 10px");
            title->setStyleSheet("color: #B9BBBE");
            description->setStyleSheet("color: #B9BBBE");
            this->setStyleSheet("background-color: #2F3136;"
                                "border-radius: 3px");
        }
    }

    void setUnclicked()
    {
        active = false;
        activeCircle->hide();
        title->setStyleSheet("color: #B9BBBE");
        description->setStyleSheet("color: #B9BBBE");
        circle->setStyleSheet("border: 2px solid #B9BBBE;"
                              "border-radius: 10px");
        this->setStyleSheet("background-color: #2F3136;"
                            "border-radius: 3px");
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
        this->setStyleSheet("background-color: rgba(79, 84, 92, 0.6);"
                            "border-radius: 3px");
        title->setStyleSheet("color: #FFF");
        description->setStyleSheet("color: #FFF");
        circle->setStyleSheet("border: 2px solid #FFF;"
                              "border-radius: 10px");
    }
    void enterEvent(QEvent *) override
    {
        if (!active) {
            this->setStyleSheet("background-color: rgba(79, 84, 92, 0.4);"
                                "border-radius: 3px");
            title->setStyleSheet("color: #DCDDDE");
            description->setStyleSheet("color: #DCDDDE");
            circle->setStyleSheet("border: 2px solid #DCDDDE;"
                                "border-radius: 10px");
        }
    }
    void leaveEvent(QEvent *) override
    {
        if (!active) {
            this->setStyleSheet("background-color: #2F3136;"
                                "border-radius: 3px");
            title->setStyleSheet("color: #B9BBBE");
            description->setStyleSheet("color: #B9BBBE");
            circle->setStyleSheet("border: 2px solid #B9BBBE;"
                                "border-radius: 10px");
        }
    }

    QLabel *activeCircle;
    QLabel *circle;
    QLabel *title;
    QLabel *description;

    bool active;
};


RadioGroup::RadioGroup(QVector<RadioParameters> radios, int selectedIndex, QWidget *parent)
    : QLabel(parent)
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