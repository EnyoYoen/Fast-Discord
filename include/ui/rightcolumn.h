#pragma once

#include "ui/messagearea.h"
#include "api/jsonutils.h"
#include "api/ressourcemanager.h"
#include "api/objects/channel.h"
#include "api/objects/message.h"
#include "api/objects/client.h"

#include <QWidget>
#include <QHBoxLayout>
#include <QLabel>

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
    void messagesReceived(QVector<Api::Message *> messages);
    void moreMessagesReceived(const QVector<Api::Message *>& messages);
    void messageAdded(const Api::Snowflake& channelId);

public slots:
    void clean();
    void openGuildChannel(const Api::Snowflake& guildId, const Api::Snowflake& id);
    void openPrivateChannel(const Api::Snowflake& id);
    void addMessage(const Api::Message& message);

private slots:
    void setMessages(QVector<Api::Message *> messages);
    void setUserTyping(const Api::User *user);
    void sendTyping();
    void sendMessage(const QString& content);
    void loadMoreMessages();

private:
    void openChannel(const Api::Snowflake& channelId, int type);

    // Main widgets
    QHBoxLayout *layout;
    QVBoxLayout *messagesLayout;
    MessageArea *messageArea;
    QLabel      *typingLabel;
    bool         placeholder;

    Api::RessourceManager *rm; // To request the API

    Api::Client *client;                 // Store the actual user
    Api::Snowflake currentOpenedChannel; // Current channel ID
    long typingTimestamp;                // The timestamp of the start of the typing
};

} // namespace Ui
