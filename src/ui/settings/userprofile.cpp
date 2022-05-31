#include "ui/settings/userprofile.h"

#include "ui/settings/divider.h"

namespace Ui {

UserProfile::UserProfile(Api::RessourceManager *rmp, QWidget *parent)
    : QScrollArea(parent)
{
    rm = rmp;

    QWidget *container = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(container);
    container->setMaximumWidth(752);
    container->setContentsMargins(40, 60, 40, 80);
    container->setStyleSheet("background-color: #36393F");

    QFont font;
    font.setPixelSize(20);
    font.setFamily("whitney");

    QLabel *title = new QLabel("User Profile", container);
    title->setFont(font);
    title->setStyleSheet("color: #FFF;");

    layout->addWidget(title);
    layout->addWidget(new Divider());

    this->setWidget(container);
    this->setStyleSheet("QScrollBar::handle {border: none; border-radius: 2px; background-color: #202225;}"
                        "QScrollBar {border: none; background-color: #36393F; border-radius: 8px; width: 3px;}"
                        "QScrollBar::add-line, QScrollBar::sub-line {border:none; background: none; height: 0;}");
}

} // namespace Ui