#pragma once

#include "messagearea.h"
#include "api/channel.h"
#include "api/message.h"
#include "api/client.h"
#include "api/jsonutils.h"
#include "api/request.h"

#include <QWidget>
#include <QHBoxLayout>
#include <QLabel>

#include <string>
#include <vector>
#include <map>

using json = nlohmann::json;

namespace Ui {

// The right column of the UI (with messages)
class RightColumn : public QWidget
{
    Q_OBJECT
public:
    RightColumn(Api::Requester *requesterp, Api::Client *clientp, QWidget *parent);
    void userTyping(const json& data); // Used when we recieve TYPING_START event
                                       // from the gateway to show the typing label

signals:
    void userTypingRecieved(const Api::User *user);
    void messagesRecieved(std::vector<Api::Message *> *messages);

public slots:
    void clean();
    void openChannel(Api::Channel& channel);
    void addMessage(Api::Message message);

private slots:
    void setMessages(std::vector<Api::Message *> *messages);
    void setUserTyping(const Api::User *user);
    void sendTyping();
    void sendMessage(const std::string& content);

private:
    // Main widgets
    QHBoxLayout *layout;
    QVBoxLayout *messagesLayout;
    MessageArea *messageArea;
    QLabel      *typingLabel;

    Api::Requester *requester; // To request the API

    // Storing messages that we already gathered
    std::map<std::string, std::vector<Api::Message *> *> channelsMessages;

    Api::Client *client;              // Store the actual user
    std::string currentOpenedChannel; // Current channel ID
    long typingTimestamp;             // The timestamp of the start of the typing
};

} // namespace Ui
