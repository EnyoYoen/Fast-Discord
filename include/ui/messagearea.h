#pragma once

#include "messagewidget.h"
#include "api/message.h"

#include <QShowEvent>
#include <QScrollArea>
#include <QVBoxLayout>

#include <vector>

namespace Ui {

// The area with all the messages in a channel
class MessageArea : public QScrollArea
{
    Q_OBJECT
public:
    MessageArea(Api::Requester *requester, const std::vector<Api::Message *>& messages, QWidget *parent);
    // Add a new message
    void addMessage(const Api::Message& newMessage, const Api::Message& lastMessage);

private:
    // The scroll to the very end
    void showEvent(QShowEvent *event) override;

    QVBoxLayout *messageLayout; // The layout of this widget

    Api::Requester *requester;  // To request the API
};

} // namespace Ui
