#pragma once

#include <QTextEdit>
#include <QObject>
#include <QKeyEvent>

namespace Ui {

// The input to send messages
class MessageTextInput : public QTextEdit
{
    Q_OBJECT
public:
    MessageTextInput(QWidget *parent = 0);

signals:
    void returnPressed(std::string);
    void typing();

private:
    void keyPressEvent(QKeyEvent *event) override;

    time_t lastTypingTimestamp; // The timestamp of the last typing
};

} // namespace Ui
