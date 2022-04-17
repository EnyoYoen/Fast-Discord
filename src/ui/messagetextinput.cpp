#include "ui/messagetextinput.h"

#include "api/objects/message.h"

#include <QDateTime>

namespace Ui {

MessageTextInput::MessageTextInput(QWidget *parent) : QTextEdit(parent)
{
    // Initialize attribute
    lastTypingTimestamp = 0;

    QFont font;
    font.setPixelSize(16);
    font.setFamily("whitney");
    this->setFont(font);

    // Style
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setPlaceholderText("Message");
    this->setFixedHeight(30);
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    this->setStyleSheet("color: #DCDDDE;"
                        "border: 0px;");
}

void MessageTextInput::keyPressEvent(QKeyEvent *event)
{
    // If the return is pressed without shift
    if (!event->modifiers().testFlag(Qt::ShiftModifier) && event->key() == Qt::Key_Return) {
        // Get the text and strip the spaces and the new lines
        QString content = toPlainText().trimmed();

        // Emit signals if the message is not empty
        if (!content.isEmpty()) {
            emit clear();
            emit returnPressed(content);
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
