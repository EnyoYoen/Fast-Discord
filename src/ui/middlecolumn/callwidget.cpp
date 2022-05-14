#include "ui/middlecolumn/callwidget.h"

#include "ui/middlecolumn/closecallbutton.h"

namespace Ui {

CallWidget::CallWidget(Api::RessourceManager *rmp, QWidget *parent)
    : QLabel(parent)
{
    rm = rmp;

    this->hide();
    this->setFixedSize(240, 44);
    this->setStyleSheet("background-color: #292B2F;"
                        "border-bottom: 1px solid #3A3F45;");

    QLabel *state = new QLabel("Voice connected", this);
    name = new QLabel(this);
    CloseCallButton *button = new CloseCallButton(rm, this);

    QFont font;
    font.setPixelSize(12);
    font.setFamily("whitney");

    name->setFont(font);
    name->move(8, 26);
    name->setStyleSheet("background-color: #292B2F;"
                        "color: #B9BBBE;"
                        "border: none");

    font.setPixelSize(14);
    font.setBold(true);
    state->setFont(font);
    state->move(8, 8);
    state->setStyleSheet("background-color: #292B2F;"
                         "color: #46C46E;"
                         "border: none");

    button->move(200, 8);

    QObject::connect(button, &CloseCallButton::clicked, [this](){this->hide();});
}

void CallWidget::call(QString channel, QString guild)
{
    name->setText(channel + " / " + guild);
    this->show();
}

} // namespace Ui