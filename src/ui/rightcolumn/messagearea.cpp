#include "ui/rightcolumn/messagearea.h"

#include "ui/rightcolumn/messagewidget.h"
#include "ui/rightcolumn/messageseparator.h"

#include <QScrollBar>
#include <QDateTime>

namespace Ui {

MessageArea::MessageArea(Api::RessourceManager *rmp, QWidget * /*parent*/)
    : QScrollArea(/*parent*/)
{
    // Set the ressource manager
    rm = rmp;
    timestamp = -1;
    emitScrollBarHigh = true;
    stopped = false;

    messageCreateThread = QThread::create([this](){loop();});
    messageCreateThread->start();

    // Create widgets
    messageBox = new Widget(this);
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
                        "QScrollBar::handle:vertical {border: none; border-radius: 2px; background-color: " + Settings::colors[Settings::BackgroundTertiary].name() + ";}"
                        "QScrollBar:vertical {border: none; background-color: " + Settings::colors[Settings::BackgroundSecondary].name() + "; border-radius: 8px; width: 4px;}"
                        "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {border:none; background: none; height: 0;}"
                        "QScrollBar:left-arrow:vertical, QScrollBar::right-arrow:vertical {background: none;}"
                        "QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {background: none;}");

    QObject::connect(this->verticalScrollBar(), &QScrollBar::valueChanged, this, &MessageArea::scrollBarMoved);
    QObject::connect(this->verticalScrollBar(), &QScrollBar::rangeChanged, this, &MessageArea::changeSliderValue);
    QObject::connect(this, &MessageArea::messageCreate, this, &MessageArea::displayMessage);
    QObject::connect(this, &MessageArea::separatorCreate, this, &MessageArea::displaySeparator);
    QObject::connect(this, &MessageArea::messagesEnd, this, &MessageArea::scrollBottom);
}

void MessageArea::setMessages(const QVector<Api::Message *>& messages)
{
    this->show();

    if (messages.size() > 0) {
        lock.tryLock();
        // Loop through the messages starting by the end (the most recents)
        messageQueue.enqueue(QueuedMessage{messages.back(), nullptr, false, false});
        for (int i = messages.size() - 2; i >= 1 ; i--) {
            messageQueue.enqueue(QueuedMessage{messages[i], messages[i + 1], false, false});
        }
        if (messages.size() > 1)
            messageQueue.enqueue(QueuedMessage{messages[0], messages[1], true, false});
        lock.unlock();
        messageWaiter.wakeOne();

        messageLayout->addSpacing(22);
        messageLayout->insertStretch(0);
    }
}

void MessageArea::addMessage(const Api::Message *newMessage, const Api::Message *lastMessage)
{
    lock.tryLock();
    messageQueue.enqueue(QueuedMessage{newMessage, lastMessage, false, false});
    lock.unlock();
    messageWaiter.wakeOne();
    if (verticalScrollBar()->value() < 0.95 * verticalScrollBar()->minimum()) scrollBottom();
}

void const MessageArea::scrollBottom()
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
    
    messages.clear();

    lock.tryLock();
    messageQueue.clear();
    lock.unlock();

    this->hide();
}

void MessageArea::addMessages(const QVector<Api::Message *>& messages)
{
    int size = messages.size();
    if (size != 0) {
        lock.tryLock();
        for (int i = 0; i < size - 1; i++) {
            messageQueue.enqueue(QueuedMessage{messages[i], messages[i + 1], false, true});
        }
        messageQueue.enqueue(QueuedMessage{messages.back(), nullptr, false, true});
        lock.unlock();
        messageWaiter.wakeOne();
    }
}

void MessageArea::updateMessagesFont()
{
    for (int i = 0 ; i < messages.size() ; i++) {
        messages[i]->updateFont();
    }
}

void MessageArea::updateMessagesSpacing()
{
    for (int i = 0 ; i < messages.size() ; i++) {
        messages[i]->updateSpacing();
        messages[i]->update();
    }
    this->update();
    messageLayout->update();
}

void const MessageArea::changeSliderValue(int min, int max)
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

void const MessageArea::scrollBarMoved(int value)
{
    messageBox->setBackgroundColor(Settings::BackgroundPrimary);
    tempScrollBarValue = value;
    tempScrollBarRange = this->verticalScrollBar()->maximum() - this->verticalScrollBar()->minimum();
    if (value < tempScrollBarRange * 0.1 && emitScrollBarHigh && timestamp - QDateTime::currentSecsSinceEpoch() < -1) {
        emitScrollBarHigh = false;
        emit scrollbarHigh();
    }
}

void MessageArea::loop()
{
    while (!stopped) {
        if (messageQueue.size() > 0) {
            do {
                lock.tryLock();
                QueuedMessage queuedMessage = messageQueue.dequeue();
                lock.unlock();
                const Api::Message *message = queuedMessage.message;

                if (queuedMessage.lastMessage == nullptr) {
                    emit messageCreate(message, queuedMessage.top, true, false);
                } else {
                    const Api::Message *lastMessage = queuedMessage.lastMessage;

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
                    if (secondTime - firstTime > 450 || lastMessage->author.id != message->author.id || (lastMessage->type != Api::Default && lastMessage->type != Api::Reply)) {
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
            lock.lock();
            messageWaiter.wait(&lock);
            lock.unlock();
        }
    }
}

void const MessageArea::displayMessage(const Api::Message *message, bool top, bool first, bool separator)
{
    MessageWidget *messageWidget = new MessageWidget(rm, message, first, separator, this);
    messages.append(messageWidget);
    if (top)
        messageLayout->insertWidget(separator ? 1 : 0, messageWidget);
    else
        messageLayout->insertWidget(messageLayout->count() - 1, messageWidget);
}

void const MessageArea::displaySeparator(const QDate& date, bool top)
{
    if (top)
        messageLayout->insertWidget(0, new MessageSeparator(date, this));
    else
        messageLayout->insertWidget(messageLayout->count() - 1, new MessageSeparator(date, this));
}

void MessageArea::updateTheme()
{
    for (int i = 0 ; i < messages.size() ; i++) {
        messages[i]->updateTheme();
    }
    
    this->setStyleSheet("QScrollArea {border: none; padding: 0px;}"
                        "QScrollBar::handle:vertical {border: none; border-radius: " + QString::number(Settings::scale(2)) + "px; background-color: " + Settings::colors[Settings::BackgroundTertiary].name() + ";}"
                        "QScrollBar:vertical {border: none; background-color: " + Settings::colors[Settings::BackgroundSecondary].name() + "; border-radius: " + QString::number(Settings::scale(8)) + "px; width: " + QString::number(Settings::scale(4)) + "px;}"
                        "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {border:none; background: none; height: 0;}"
                        "QScrollBar:left-arrow:vertical, QScrollBar::right-arrow:vertical {background: none;}"
                        "QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {background: none;}");
}

MessageArea::~MessageArea()
{
    stopped = true;
    messageWaiter.wakeOne();
    messageCreateThread->wait();
}

} // namespace Ui
