#include "ui/messagetextinput.h"

#include "api/message.h"

#include <algorithm>
#include <ctime>

#include <QDebug>

namespace Ui {

MessageTextInput::MessageTextInput(QWidget *parent) : QTextEdit(parent)
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

        unsigned long start = content.find_first_not_of(" \n\r\t\f\v");
        content = start == std::string::npos ? "" : content.substr(start);
        unsigned long end = content.find_last_not_of(" \n\r\t\f\v");
        content = end == std::string::npos ? "" : content.substr(0, end + 1);
        QString qcontent(content.c_str());

        // Emit signals if the message is not empty
        if (!qcontent.isEmpty()) {
            emit clear();
            emit returnPressed(qcontent);
        }
    } else {
        // Compare the last timestamp and the actual time
        time_t actualTimestamp = std::time(nullptr);
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
