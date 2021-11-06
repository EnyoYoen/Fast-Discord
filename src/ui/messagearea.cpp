#include "../../include/ui/messagearea.h"

#include "../../include/ui/messagewidget.h"
#include "../../include/ui/messageseparator.h"

#include <QScrollBar>
#include <QGroupBox>
#include <QDateTime>

namespace Ui {

MessageArea::MessageArea(const std::vector<Api::Message *>& messages) : QScrollArea()
{
    QGroupBox *messageBox = new QGroupBox();
    messageLayout = new QVBoxLayout();

    if (messages.size() > 0) {
        MessageWidget *firstMessage = new MessageWidget(*messages.back(), true, false);
        firstMessage->setContentsMargins(0, 0, 0, 0);
        messageLayout->addWidget(firstMessage);

        for (int i = messages.size() - 2; i >= 0 ; i--) {
            QDateTime firstDateTime = QDateTime::fromString(QString((*messages[i + 1]->timestamp).c_str()), Qt::ISODate);
            QDateTime secondDateTime = QDateTime::fromString(QString((*messages[i]->timestamp).c_str()), Qt::ISODate);

            bool separator;
            if (firstDateTime.date() != secondDateTime.date()) {
                messageLayout->addWidget(new MessageSeparator(secondDateTime.date()));
                separator = true;
            } else {
                separator = false;
            }

            qint64 firstTime = firstDateTime.toSecsSinceEpoch();
            qint64 secondTime = secondDateTime.toSecsSinceEpoch();
            bool first;

            if (secondTime - firstTime > 450 || *messages[i + 1]->author->id != *messages[i]->author->id) {
                first = true;
            } else {
                first = false;
            }

            messageLayout->addWidget(new MessageWidget(*messages[i], first, separator));
        }
        messageLayout->insertStretch(0);
    }

    messageLayout->setSpacing(0);
    messageLayout->setContentsMargins(0, 0, 0, 0);
    setWidgetResizable(true);
    setStyleSheet("QScrollArea {border: none; padding: 0px;}"
                  "QScrollBar::handle:vertical {border: none; border-radius: 8px; background-color: #202225;}"
                  "QScrollBar:vertical {border: none; background-color: #2F3136; border-radius: 8px; width: 16px;}"
                  "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {border:none; background: none; height: 0;}");

    messageBox->setLayout(messageLayout);
    setWidget(messageBox);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setContentsMargins(0, 0, 0, 0);
}

void MessageArea::addMessage(const Api::Message& newMessage, const Api::Message& lastMessage)
{
    QDateTime firstDateTime = QDateTime::fromString(QString((*lastMessage.timestamp).c_str()), Qt::ISODate);
    QDateTime secondDateTime = QDateTime::fromString(QString((*newMessage.timestamp).c_str()), Qt::ISODate);

    bool separator;
    if (firstDateTime.date() != secondDateTime.date()) {
        messageLayout->addWidget(new MessageSeparator(secondDateTime.date()));
        separator = true;
    } else {
        separator = false;
    }

    qint64 firstTime = firstDateTime.toSecsSinceEpoch();
    qint64 secondTime = secondDateTime.toSecsSinceEpoch();
    bool first;

    if (secondTime - firstTime > 450 || *lastMessage.author->id != *newMessage.author->id) {
        first = true;
    } else {
        first = false;
    }

    messageLayout->addWidget(new MessageWidget(newMessage, first, separator));
}

void MessageArea::showEvent(QShowEvent *)
{
    QScrollBar *vsb = verticalScrollBar();
    vsb->setValue(vsb->maximum());
}

} // namespace Ui
