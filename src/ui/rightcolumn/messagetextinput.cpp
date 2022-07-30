#include "ui/rightcolumn/messagetextinput.h"

#include "settings/settings.h"
#include "api/objects/message.h"

#include <QDateTime>

namespace Ui {

MessageTextInput::MessageTextInput(const QString& channelName, QWidget *parent)
    : QTextEdit()
{
    // Initialize attribute
    lastTypingTimestamp = 0;

    QFont font;
    font.setPixelSize(Settings::scale(16));
    font.setFamily("whitney");
    this->setFont(font);

    // Style
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setPlaceholderText("Message " + channelName);
    this->setFixedHeight(Settings::scale(30));
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    this->setStyleSheet("color:" + Settings::colors[Settings::ChanneltextareaPlaceholder].name() + ";"
                        "border: 0px;"
                        "background: " + Settings::colors[Settings::ChanneltextareaBackground].name());

    QObject::connect(this, &QTextEdit::textChanged, [this](){
        if (this->toPlainText().isEmpty())
            this->setStyleSheet("color:" + Settings::colors[Settings::ChanneltextareaPlaceholder].name() + ";"
                                "border: 0px;"
                                "background: " + Settings::colors[Settings::ChanneltextareaBackground].name());
        else 
            this->setStyleSheet("color:" + Settings::colors[Settings::TextNormal].name() + ";"
                                "border: 0px;"
                                "background: " + Settings::colors[Settings::ChanneltextareaBackground].name());
    });
}

void MessageTextInput::keyPressEvent(QKeyEvent *event)
{
    // If the return is pressed without shift
    if (!event->modifiers().testFlag(Qt::ShiftModifier) && event->key() == Qt::Key_Return) {
        // Get the text and strip the spaces and the new lines
        QString content = toPlainText().trimmed();

        emit returnPressed(content);
        if (!content.isEmpty()) {
            emit clear();
        }
    } else {
        // Compare the last timestamp and the actual time
        qint64 actualTimestamp = QDateTime::currentSecsSinceEpoch();
        if (actualTimestamp - lastTypingTimestamp > 8) {
            // Send typing because we typed more than 8 seconds ago
            lastTypingTimestamp = actualTimestamp;
            emit typing();
        }

        // Spread the event
        QTextEdit::keyPressEvent(event);
    }
}

} // namespace Ui
