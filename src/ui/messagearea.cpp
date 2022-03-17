#include "ui/messagearea.h"

#include "ui/messagewidget.h"
#include "ui/messageseparator.h"

#include <QScrollBar>
#include <QDateTime>

namespace Ui {

MessageArea::MessageArea(Api::RessourceManager *rmp, QWidget * /*parent*/)
    : QScrollArea(/*parent*/) // TODO stylesheet bug
{
    // Set the ressource manager
    rm = rmp;
    timestamp = -1;
    emitScrollBarHigh = true;
    stopped = false;

    messageCreateThread = QThread::create([this](){loop();});
    messageCreateThread->start();

    // Create widgets
    messageBox = new QWidget(this);
    messageLayout = new QVBoxLayout(messageBox);

    // Style the layout
    messageLayout->setSpacing(0);
    messageLayout->setContentsMargins(0, 0, 0, 0);

    // Style the widget
    this->setWidget(messageBox);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setContentsMargins(0, 0, 0, 0);
    this->setWidgetResizable(true);
    this->setStyleSheet("QScrollArea {border: none; padding: 0px;}"
                        "QScrollBar::handle:vertical {border: none; border-radius: 8px; background-color: #202225;}"
                        "QScrollBar:vertical {border: none; background-color: #2F3136; border-radius: 8px; width: 16px;}"
                        "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {border:none; background: none; height: 0;}");

    QObject::connect(this->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(scrollBarMoved(int)));
    QObject::connect(this->verticalScrollBar(), SIGNAL(rangeChanged(int, int)), this, SLOT(changeSliderValue(int, int)));
    QObject::connect(this, SIGNAL(messageCreate(Api::Message *, bool, bool, bool)), this, SLOT(displayMessage(Api::Message *, bool, bool, bool)));
    QObject::connect(this, SIGNAL(separatorCreate(const QDate&, bool)), this, SLOT(displaySeparator(const QDate&, bool)));
    QObject::connect(this, SIGNAL(messagesEnd()), this, SLOT(scrollBottom()));
}

void MessageArea::setMessages(const QVector<Api::Message *>& messages)
{
    this->show();

    if (messages.size() > 0) {
        lock.lock();
        // Loop through the messages starting by the end (the most recents)
        messageQueue.push(QueuedMessage{messages.back(), nullptr, false, false});
        for (int i = messages.size() - 2; i >= 1 ; i--) {
            messageQueue.push(QueuedMessage{messages[i], messages[i + 1], false, false});
        }
        messageQueue.push(QueuedMessage{messages[0], messages[1], true, false});
        lock.unlock();
        messageWaiter.notify_one();

        QWidget *spacer = new QWidget(messageBox);
        spacer->setFixedHeight(22);
        messageLayout->addWidget(spacer);
        messageLayout->insertStretch(0);
    }
}

void MessageArea::addMessage(Api::Message *newMessage, Api::Message *lastMessage)
{
    lock.lock();
    messageQueue.push(QueuedMessage{newMessage, lastMessage, false, false});
    lock.unlock();
    messageWaiter.notify_one();
    if (verticalScrollBar()->value() < 0.95 * verticalScrollBar()->minimum()) scrollBottom();
}

void MessageArea::scrollBottom()
{
    // Scroll to the bottom
    QScrollBar *vsb = verticalScrollBar();
    vsb->setValue(vsb->maximum());
}

void MessageArea::clear()
{
    // Remove all items from the layout
    if (messageLayout->count() > 0) {
        QLayoutItem *item;
        while ((item = messageLayout->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }
    }

    lock.lock();
    for (unsigned int i = 0; i < messageQueue.size() ; i++) {
        messageQueue.pop();
    }
    lock.unlock();

    this->hide();
}

void MessageArea::addMessages(const QVector<Api::Message *>& messages)
{
    int size = messages.size();
    if (size != 0) {
        lock.lock();
        for (int i = 0; i < size - 1; i++) {
            messageQueue.push(QueuedMessage{messages[i], messages[i + 1], false, true});
        }
        messageQueue.push(QueuedMessage{messages.back(), nullptr, false, true});
        lock.unlock();
        messageWaiter.notify_one();
    }
}

void MessageArea::changeSliderValue(int min, int max)
{
    this->verticalScrollBar()->setValue(max - min - tempScrollBarRange + tempScrollBarValue);
    timestamp = QDateTime::currentSecsSinceEpoch();
    emitScrollBarHigh = true;
}

void MessageArea::showEvent(QShowEvent *)
{
    // Set the scroll bar to the maximum to show the most recent messages
    QScrollBar *vsb = verticalScrollBar();
    vsb->setValue(vsb->maximum());
}

void MessageArea::scrollBarMoved(int value)
{
    tempScrollBarValue = value;
    tempScrollBarRange = this->verticalScrollBar()->maximum() - this->verticalScrollBar()->minimum();
    if (value < tempScrollBarRange * 0.1 && emitScrollBarHigh && timestamp - QDateTime::currentSecsSinceEpoch() > 1) {
        emitScrollBarHigh = false;
        emit scrollbarHigh();
    }
}

void MessageArea::loop()
{
    while (!stopped) {
        if (messageQueue.size() > 0) {
            do {
                lock.lock();
                QueuedMessage queuedMessage = messageQueue.front();
                messageQueue.pop();
                lock.unlock();
                Api::Message *message = queuedMessage.message;

                if (queuedMessage.lastMessage == nullptr) {
                    emit messageCreate(message, queuedMessage.top, true, false);
                } else {
                    Api::Message *lastMessage = queuedMessage.lastMessage;

                    // Get date and time of the messages
                    QDateTime firstDateTime = QDateTime::fromString(lastMessage->timestamp, Qt::ISODate);
                    QDateTime secondDateTime = QDateTime::fromString(message->timestamp, Qt::ISODate);

                    // Determine if we need a separator (messages not sent the same day)
                    bool separator;
                    if (firstDateTime.toLocalTime().date() != secondDateTime.toLocalTime().date()) {
                        // The messages are not sent on the same day
                        emit separatorCreate(secondDateTime.date(), queuedMessage.top);
                        separator = true;
                    } else {
                        // The messages are sent on the same day
                        separator = false;
                    }

                    // Determine If the two messages are grouped

                    // Get seconds since epoch of the messages
                    qint64 firstTime = firstDateTime.toSecsSinceEpoch();
                    qint64 secondTime = secondDateTime.toSecsSinceEpoch();
                    bool first;
                    // If the messages are separated by more than 7.30 minutes
                    // or the authors are not the same
                    if (secondTime - firstTime > 450 || lastMessage->author.id != message->author.id) {
                        // The message is not grouped to another message
                        first = true;
                    } else {
                        // The message is grouped to another message
                        first = false;
                    }

                    // Create the message and add it to the layout
                    emit messageCreate(message, queuedMessage.top, first, separator);
                }

                if (queuedMessage.end)
                    emit messagesEnd();
            } while (!messageQueue.empty());
        } else {
            std::unique_lock<std::mutex> uniqueLock(lock);
            messageWaiter.wait(uniqueLock);
        }
    }
}

void MessageArea::displayMessage(Api::Message *message, bool top, bool first, bool separator)
{
    if (top)
        messageLayout->insertWidget(separator ? 1 : 0, new MessageWidget(rm, message, first, separator, this));
    else
        messageLayout->insertWidget(messageLayout->count() - 1, new MessageWidget(rm, message, first, separator, this));
}

void MessageArea::displaySeparator(const QDate& date, bool top)
{
    if (top)
        messageLayout->insertWidget(0, new MessageSeparator(date, this));
    else
        messageLayout->insertWidget(messageLayout->count() - 1, new MessageSeparator(date, this));
}

MessageArea::~MessageArea()
{
    stopped = true;
    messageWaiter.notify_one();
}

} // namespace Ui
