#include "ui/settings/settingsinput.h"

#include <QVBoxLayout>

namespace Ui { 

SettingsInput::SettingsInput(QString placeholder, QString text, bool readOnly, bool password, QWidget *parent)
    : Widget(parent)
{
    this->setFixedHeight(Settings::scale(40));
    this->setContentsMargins(Settings::scale(10), Settings::scale(10), Settings::scale(10), Settings::scale(10));
    this->setBorderRadius(Settings::scale(3));
    this->setBackgroundColor(Settings::BackgroundTertiary);

    QFont font;
    font.setPixelSize(Settings::scale(16));
    font.setFamily("whitney");
    input = new QLineEdit(text, this);
    input->setFont(font);
    input->setPlaceholderText(placeholder);
    input->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    if (readOnly) {
        input->setReadOnly(true);
        input->setStyleSheet("border: none;"
                             "background: none;"
                             "color: #A3A6AA");
    } else {
        this->setCursor(Qt::CursorShape::IBeamCursor);
        input->setStyleSheet("border: none;"
                             "background: none;"
                             "color: #DCDDDE");
    }
    if (password)
        input->setEchoMode(QLineEdit::EchoMode::Password);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(input);
}

void SettingsInput::mouseReleaseEvent(QMouseEvent *)
{
    input->setFocus();
}

} // namespace Ui