#include "ui/messagearea.h"

#include "ui/messagewidget.h"
#include "ui/messageseparator.h"

#include <QScrollBar>
#include <QDateTime>

namespace Ui {

MessageArea::MessageArea(Api::RessourceManager *rmp, const std::vector<Api::Message *>& messages, QWidget */*parent*/)
    : QScrollArea(/*parent*/) // TODO stylesheet bug
{
    // Set the ressource manager
    rm = rmp;
    emitScrollBarHigh = true;

    // Create widgets
    QWidget *messageBox = new QWidget(this);
    messageLayout = new QVBoxLayout(messageBox);
    QWidget *spacer = new QWidget(messageBox);
    spacer->setFixedHeight(22);

    // Don't do some things if there are no messages
    if (messages.size() > 0) {
        // Create the first message widget and style it
        MessageWidget *firstMessage = new MessageWidget(rm, *messages.back(), true, false, this);
        firstMessage->setContentsMargins(0, 0, 0, 0);

        // Add it to the layout
        messageLayout->addWidget(firstMessage);

        // Loop through the messages starting by the end (the most recents)
        for (int i = messages.size() - 2; i >= 0 ; i--) {
            // Get date and time of the messages
            QDateTime firstDateTime = QDateTime::fromString(QString((*messages[i + 1]->timestamp).c_str()), Qt::ISODate);
            QDateTime secondDateTime = QDateTime::fromString(QString((*messages[i]->timestamp).c_str()), Qt::ISODate);

            // Determine if we need a separator (messages not sent the same day)
            bool separator;
            if (firstDateTime.date() != secondDateTime.date()) {
                // The messages are not sent on the same day
                messageLayout->addWidget(new MessageSeparator(secondDateTime.date(), this));
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
            if (secondTime - firstTime > 450 || *messages[i + 1]->author->id != *messages[i]->author->id) {
                // The message is not grouped to another message
                first = true;
            } else {
                // The message is grouped to another message
                first = false;
            }

            // Create the message and add it to the layout
            messageLayout->addWidget(new MessageWidget(rm, *messages[i], first, separator, this));
        }
        messageLayout->addWidget(spacer);
        messageLayout->insertStretch(0);
    }

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
}

void MessageArea::addMessage(const Api::Message& newMessage, const Api::Message& lastMessage)
{
    // Get date and time of the messages
    QDateTime firstDateTime = QDateTime::fromString(QString((*lastMessage.timestamp).c_str()), Qt::ISODate);
    QDateTime secondDateTime = QDateTime::fromString(QString((*newMessage.timestamp).c_str()), Qt::ISODate);

    // Determine if we need a separator (messages not sent the same day)
    bool separator;
    if (firstDateTime.date() != secondDateTime.date()) {
        // The messages are not sent on the same day
        messageLayout->insertWidget(messageLayout->count() - 1, new MessageSeparator(secondDateTime.date(), this));
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
    if (secondTime - firstTime > 420 || *lastMessage.author->id != *newMessage.author->id) {
        first = true;
    } else {
        first = false;
    }

    // Create the message and add it to the layout
    messageLayout->insertWidget(messageLayout->count() - 1, new MessageWidget(rm, newMessage, first, separator, this));
    QScrollBar *vsb = verticalScrollBar();
    vsb->setValue(vsb->maximum());
}

void MessageArea::addMessages(const std::vector<Api::Message *>& messages)
{
    unsigned int size = messages.size() - 1;
    for (unsigned int i = size ; i > 1; i--) {
        // Get date and time of the messages
        QDateTime firstDateTime = QDateTime::fromString(QString((*messages[size - i + 1]->timestamp).c_str()), Qt::ISODate);
        QDateTime secondDateTime = QDateTime::fromString(QString((*messages[size - i]->timestamp).c_str()), Qt::ISODate);

        // Determine if we need a separator (messages not sent the same day)
        bool separator;
        if (firstDateTime.date() != secondDateTime.date()) {
            // The messages are not sent on the same day
            messageLayout->insertWidget(0, new MessageSeparator(secondDateTime.date(), this));
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
        if (secondTime - firstTime > 420 || *messages[size - i + 1]->author->id != *messages[size - i]->author->id) {
            first = true;
        } else {
            first = false;
        }

        // Create the message and add it to the layout
        messageLayout->insertWidget(separator ? 1 : 0, new MessageWidget(rm, *messages[size - i], first, separator, this));
    }

    this->verticalScrollBar()->setValue((this->verticalScrollBar()->maximum() - this->verticalScrollBar()->minimum()) - tempScrollBarRange + tempScrollBarValue);
    //emitScrollBarHigh = true;
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
    if (value < tempScrollBarRange * 0.1 && emitScrollBarHigh) {
        emitScrollBarHigh = false;
        emit scrollbarHigh();
    }
}

} // namespace Ui
