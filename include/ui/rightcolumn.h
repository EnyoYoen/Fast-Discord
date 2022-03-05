#pragma once

#include "messagearea.h"
#include "api/channel.h"
#include "api/message.h"
#include "api/client.h"
#include "api/jsonutils.h"
#include "api/ressourcemanager.h"

#include <QWidget>
#include <QHBoxLayout>
#include <QLabel>

#include <string>
#include <vector>
#include <map>

namespace Ui {

// The right column of the UI (with messages)
class RightColumn : public QWidget
{
    Q_OBJECT
public:
    RightColumn(Api::RessourceManager *rm, Api::Client *clientp, QWidget *parent);
    void userTyping(const json& data); // Used when we recieve TYPING_START event
                                       // from the gateway to show the typing label

signals:
    void userTypingReceived(const Api::User *user);
    void messagesReceived(std::vector<Api::Message *> *messages);
    void moreMessagesReceived(const std::vector<Api::Message *>& messages);
    void messageAdded(const std::string& channelId);

public slots:
    void clean();
    void openGuildChannel(const std::string& guildId, const std::string& id);
    void openPrivateChannel(const std::string& id);
    void addMessage(const Api::Message& message);

private slots:
    void setMessages(std::vector<Api::Message *> *messages);
    void setUserTyping(const Api::User *user);
    void sendTyping();
    void sendMessage(const std::string& content);
    void loadMoreMessages();

private:
    void openChannel(const std::string& channelId, int type);

    // Main widgets
    QHBoxLayout *layout;
    QVBoxLayout *messagesLayout;
    MessageArea *messageArea;
    QLabel      *typingLabel;
    bool         placeholder;

    Api::RessourceManager *rm; // To request the API

    Api::Client *client;              // Store the actual user
    std::string currentOpenedChannel; // Current channel ID
    long typingTimestamp;             // The timestamp of the start of the typing
};

} // namespace Ui
