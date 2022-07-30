#include "ui/middlecolumn/callwidget.h"

#include "ui/middlecolumn/closecallbutton.h"

namespace Ui {

CallWidget::CallWidget(Api::RessourceManager *rmp, QWidget *parent)
    : Widget(parent)
{
    rm = rmp;

    this->hide();
    this->setFixedSize(Settings::scale(240), Settings::scale(44));
    this->setBackgroundColor(Settings::BackgroundSecondaryAlt);
    this->setBorderColor(Settings::BackgroundModifierAccent);
    this->setBorderSize(0, 0, Settings::scale(1), 0);

    Label *state = new Label("Voice connected", this);
    name = new Label(this);
    CloseCallButton *button = new CloseCallButton(rm, this);

    QFont font;
    font.setPixelSize(Settings::scale(12));
    font.setFamily("whitney");

    name->setFont(font);
    name->setFixedWidth(Settings::scale(181));
    name->move(Settings::scale(8), Settings::scale(26));
    name->setBackgroundColor(Settings::BackgroundSecondaryAlt);
    name->setTextColor(Settings::HeaderSecondary);

    font.setPixelSize(Settings::scale(14));
    font.setBold(true);
    state->setFont(font);
    state->move(Settings::scale(8), Settings::scale(8));
    state->setBackgroundColor(Settings::BackgroundSecondaryAlt);
    state->setTextColor(Settings::TextPositive);

    button->move(Settings::scale(200), Settings::scale(8));

    QObject::connect(button, &CloseCallButton::clicked, [this](){this->hide();});
}

void CallWidget::call(QString channel, QString guild)
{
    name->setText(channel + " / " + guild);
    this->show();
}

} // namespace Ui