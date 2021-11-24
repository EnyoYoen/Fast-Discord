#include "ui/messagetextinput.h"

#include "api/message.h"

#include <algorithm>
#include <ctime>

namespace Ui {

MessageTextInput::MessageTextInput(QWidget *parent) : QPlainTextEdit(parent)
{
    // Initialize attribute
    lastTypingTimestamp = 0;

    // Style
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setPlaceholderText("Message");
    this->setFixedHeight(22);
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    this->setStyleSheet("color: #DCDDDE;"
                        "border: 0px;");
}

void MessageTextInput::keyPressEvent(QKeyEvent *event)
{
    // If the return is pressed without shift
    if (!event->modifiers().testFlag(Qt::ShiftModifier) && event->key() == Qt::Key_Return) {
        // Get the text and strip the spaces and the new lines
        std::string content = toPlainText().toUtf8().constData();
        content.erase(std::remove(content.begin(), content.end(), '\n'), content.end());
        content.erase(std::remove(content.begin(), content.end(), ' '), content.end());

        // Emit signals if the message is not empty
        if (content != "") {
            emit clear();
            emit returnPressed(content);
        }
    }

    // Compare the last timestamp and the actual time
    time_t actualTimestamp = std::time(nullptr);
    if (actualTimestamp - lastTypingTimestamp > 8) {
        // Send typing because we typed more than 8 seconds ago
        lastTypingTimestamp = actualTimestamp;
        emit typing();
    }

    // Spread the event
    QPlainTextEdit::keyPressEvent(event);
}

} // namespace Ui
