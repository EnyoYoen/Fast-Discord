#pragma once

#include "ui/common/basicwidgets.h"
#include "ui/rightcolumn/messagearea.h"
#include "ui/rightcolumn/channelheader.h"
#include "ui/rightcolumn/memberlist.h"
#include "api/jsonutils.h"
#include "api/ressourcemanager.h"
#include "api/objects/channel.h"
#include "api/objects/message.h"
#include "api/objects/client.h"

#include <QWidget>
#include <QHBoxLayout>
#include <QLabel>

namespace Ui {

// The right column of the UI (with messages)
class RightColumn : public Widget
{
    Q_OBJECT
public:
    RightColumn(Api::RessourceManager *rm, QWidget *parent);
    void updateTheme();
    void userTyping(const json& data); // Used when we recieve TYPING_START event
                                       // from the gateway to show the typing label

signals:
    void userTypingReceived(const Api::User *user);
    void messagesReceived(const QVector<Api::Message *>& messages);
    void moreMessagesReceived(const QVector<Api::Message *>& messages);
    void messageAdded(const Api::Snowflake& channelId);

public slots:
    void clean();
    void openGuildChannel(const QString& channelName, const Api::Snowflake& guildId, const Api::Snowflake& id);
    void openPrivateChannel(const QString& channelName, const Api::Snowflake& id);
    void const addMessage(const Api::Message& message);
    void setMembers(Api::GuildMemberGateway members);

private slots:
    void const setMessages(const QVector<Api::Message *>& messages);
    void const setUserTyping(const Api::User *user);
    void const sendTyping();
    void const sendMessage(const QString& content);
    void const loadMoreMessages();
    void const setUploadFilePath(const QString& filepath);

private:
    void openChannel(const Api::Snowflake& channelId, const QString& channelName, int type, const QVector<Api::Snowflake>& recipientIds);

    // Main widgets
    ChannelHeader *header;
    QHBoxLayout   *layout;
    QVBoxLayout   *messagesLayout;
    MessageArea   *messageArea;
    MemberList    *memberList;
    Label         *typingLabel;
    QLabel        *fileLabel;
    Widget        *inputBox = nullptr;
    bool           placeholder;

    Api::RessourceManager *rm; // To request the API
    QString filePath;

    Api::Snowflake currentOpenedChannel; // Current channel ID
    long typingTimestamp;                // The timestamp of the start of the typing
};

} // namespace Ui
