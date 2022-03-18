#pragma once

#include "ui/roundedimage.h"
#include "ui/markdownlabel.h"
#include "api/ressourcemanager.h"
#include "api/objects/message.h"

#include <QWidget>
#include <QLabel>

namespace Ui {

// A widget to show messages
class MessageWidget : public QWidget
{
    Q_OBJECT
public:
    MessageWidget(Api::RessourceManager *rm, Api::Message *message, bool isFirst, bool separatorBefore, QWidget *parent);

    Api::RessourceManager *rm; // To request the API

private:
    void iconMessage(Api::Message *message, const QString &text, const QString& iconName);

    void defaultMessage(Api::Message *message, bool separatorBefore);
    void recipientMessage(Api::Message *message);
    void callMessage(Api::Message *message);
    void channelNameChangeMessage(Api::Message *message);
    void channelIconChangeMessage(Api::Message *message);
    void channelPinnedMessage(Api::Message *message);
    void guildMemberJoinMessage(Api::Message *message);
    void userPremiumGuildSubscriptionMessage(Api::Message *message);
    void channelFollowAdd(Api::Message *message);

    void enterEvent(QEvent *) override;
    void leaveEvent(QEvent *) override;
    void setAvatar(const QString& avatarFileName);
    void setReplyAvatar(const QString& avatarFileName);
    void addImage(const QString& filename, int width, int height);
    QString processTime(QTime time);
    QString processTimestamp(QDateTime dateTime);

    QString hoveredTimestamp;         // The timestamp we show
    RoundedImage *avatar = nullptr;   // The avatar if there is one
    RoundedImage *replyAvatar = nullptr;
    QLabel *timestampLabel = nullptr; // The label of the timestamp
    MarkdownLabel *content = nullptr;
    QVBoxLayout *dataLayout = nullptr;
    bool isFirst;             // Is the message is not grouped

};

} // namespace Ui
