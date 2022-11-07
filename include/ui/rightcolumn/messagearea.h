#pragma once

#include "ui/rightcolumn/messagewidget.h"
#include "api/objects/message.h"

#include <QQueue>
#include <QShowEvent>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QThread>
#include <QMutex>

struct QueuedMessage {
    const Api::Message *message;
    const Api::Message *lastMessage;
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
    void addMessage(const Api::Message *newMessage, const Api::Message *lastMessage);
    void clear();

    void addReaction(const Api::Snowflake& userId, const Api::Snowflake& channelId, const Api::Snowflake& messageId, Api::GuildMember *member, Api::Emoji *emoji);
    void removeReaction(const Api::Snowflake& userId, const Api::Snowflake& channelId, const Api::Snowflake& messageId, Api::Emoji *emoji);
    
    void updateMessagesFont();
    void updateMessagesSpacing();
    void updateTheme();

signals:
    void messagesEnd();
    void scrollbarHigh();
    void separatorCreate(const QDate&, bool);
    void messageCreate(const Api::Message *, bool, bool, bool);

public slots:
    void addMessages(const QVector<Api::Message *>& messages);

private slots:
    void const scrollBottom();
    void const scrollBarMoved(int value);
    void const changeSliderValue(int min, int max);
    void const displaySeparator(const QDate& date, bool top);
    void const displayMessage(const Api::Message *message, bool top, bool first, bool separator);

private:
    void showEvent(QShowEvent *event) override; // To scroll to the very end
    void loop();

    QVector<MessageWidget *> messages;
    Widget *messageBox;
    QVBoxLayout *messageLayout; // The layout of this widget

    Api::RessourceManager *rm;  // To request the API

    QThread *messageCreateThread;
    QQueue<QueuedMessage> messageQueue;
    QMutex lock;
    QWaitCondition messageWaiter;
    bool stopped;

    unsigned long timestamp;
    int tempScrollBarValue;
    int tempScrollBarRange;
    bool emitScrollBarHigh;
};

} // namespace Ui
