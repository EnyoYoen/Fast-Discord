#pragma once

#include "messagewidget.h"
#include "api/message.h"

#include <QShowEvent>
#include <QScrollArea>
#include <QVBoxLayout>

#include <vector>

namespace Ui {

class MessageArea : public QScrollArea
{
    Q_OBJECT
public:
    MessageArea(const std::vector<Api::Message *>& messages);
    void addMessage(const Api::Message& newMessage, const Api::Message& lastMessage);

private:
    void showEvent(QShowEvent *event) override;

    QVBoxLayout *messageLayout;
};

} // namespace Ui
