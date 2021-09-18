#include "../include/ui/messagetextinput.h"

#include "../include/api/message.h"

#include <algorithm>
#include <ctime>

namespace Ui {

MessageTextInput::MessageTextInput(QWidget *parent) : QPlainTextEdit(parent)
{
    lastTypingTimestamp = 0;

    setStyleSheet("color: #DCDDDE;"
                  "border: 0px;");
    setPlaceholderText("Message");
    setFixedHeight(22);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
}

void MessageTextInput::keyPressEvent(QKeyEvent *event)
{
    if (!event->modifiers().testFlag(Qt::ShiftModifier) && event->key() == Qt::Key_Return) {
        std::string content = toPlainText().toUtf8().constData();
        content.erase(std::remove(content.begin(), content.end(), '\n'), content.end());
        content.erase(std::remove(content.begin(), content.end(), ' '), content.end());
        if (content != "") {
            emit clear();
            emit returnPressed(content);
        }
    }
    time_t actualTimestamp = std::time(nullptr);
    if (actualTimestamp - lastTypingTimestamp > 8) {
        lastTypingTimestamp = actualTimestamp;
        emit typing();
    }

    QPlainTextEdit::keyPressEvent(event);
}

} // namespace Ui
