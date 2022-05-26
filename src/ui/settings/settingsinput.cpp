#include "ui/settings/settingsinput.h"

#include <QVBoxLayout>

namespace Ui { 

SettingsInput::SettingsInput(QString placeholder, QString text, bool readOnly, bool password, QWidget *parent)
    : QWidget(parent)
{
    this->setFixedHeight(40);
    this->setContentsMargins(10, 10, 10, 10);
    this->setAttribute(Qt::WA_StyledBackground);
    this->setStyleSheet("border-radius: 3px;"
                        "background-color: #202225;");

    QFont font;
    font.setPixelSize(16);
    font.setFamily("whitney");
    input = new QLineEdit(text, this);
    input->setFont(font);
    input->setPlaceholderText(placeholder);
    input->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    if (readOnly) {
        input->setReadOnly(true);
        input->setStyleSheet("border: none;"
                             "color: #A3A6AA");
    } else {
        this->setCursor(Qt::CursorShape::IBeamCursor);
        input->setStyleSheet("border: none;"
                             "color: #DCDDDE");
    }
    if (password)
        input->setEchoMode(QLineEdit::EchoMode::Password);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(input);
}

void SettingsInput::mouseReleaseEvent(QMouseEvent *)
{
    input->setFocus();
}

} // namespace Ui