#pragma once

#include "ui/messagewidget.h"
#include "api/objects/message.h"

#include <QShowEvent>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QThread>

#include <queue>
#include <mutex>
#include <condition_variable>

struct QueuedMessage {
    Api::Message *message;
    Api::Message *lastMessage;
    bool end;
    bool top;
};

namespace Ui {

// The area with all the messages in a channel
class MessageArea : public QScrollArea
{
    Q_OBJECT
public:
    MessageArea(Api::RessourceManager *rm, QWidget *parent);
    ~MessageArea();

    // Add messages
    void setMessages(const QVector<Api::Message *>& messages);
    void addMessage(Api::Message *newMessage, Api::Message *lastMessage);
    void clear();

signals:
    void messagesEnd();
    void scrollbarHigh();
    void separatorCreate(const QDate&, bool);
    void messageCreate(Api::Message *, bool, bool, bool);

public slots:
    void addMessages(const QVector<Api::Message *>& messages);

private slots:
    void scrollBottom();
    void scrollBarMoved(int value);
    void changeSliderValue(int min, int max);
    void displaySeparator(const QDate& date, bool top);
    void displayMessage(Api::Message *message, bool top, bool first, bool separator);

private:
    void showEvent(QShowEvent *event) override; // To scroll to the very end
    void loop();

    QWidget *messageBox;
    QVBoxLayout *messageLayout; // The layout of this widget

    Api::RessourceManager *rm;  // To request the API

    QThread *messageCreateThread;
    std::queue<QueuedMessage> messageQueue;
    std::mutex lock;
    std::condition_variable messageWaiter;
    bool stopped;

    unsigned long timestamp;
    int tempScrollBarValue;
    int tempScrollBarRange;
    bool emitScrollBarHigh;
};

} // namespace Ui
