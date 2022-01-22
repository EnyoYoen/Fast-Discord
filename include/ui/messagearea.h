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
    MessageArea(Api::RessourceManager *rm, const std::vector<Api::Message *>& messages, QWidget *parent);
    // Add a new message
    void addMessage(const Api::Message& newMessage, const Api::Message& lastMessage);

signals:
    void scrollbarHigh();

public slots:
    void addMessages(const std::vector<Api::Message *>& messages);

private slots:
    void scrollBarMoved(int value);
    void changeSliderValue(int min, int max);

private:
    // The scroll to the very end
    void showEvent(QShowEvent *event) override;

    QVBoxLayout *messageLayout; // The layout of this widget

    Api::RessourceManager *rm;  // To request the API

    int tempScrollBarValue;
    int tempScrollBarRange;
    bool emitScrollBarHigh;
};

} // namespace Ui
