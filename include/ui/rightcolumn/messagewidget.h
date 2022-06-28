#pragma once

#include "ui/common/roundedimage.h"
#include "ui/rightcolumn/markdownlabel.h"
#include "api/ressourcemanager.h"
#include "api/objects/message.h"

#include <QWidget>
#include <QLabel>

namespace Ui {

// A widget to show messages
class MessageWidget : public Widget
{
    Q_OBJECT
public:
    MessageWidget(Api::RessourceManager *rm, const Api::Message *message, bool isFirst, bool separatorBefore, QWidget *parent);

    Api::RessourceManager *rm; // To request the API

private:
    void iconMessage(const Api::Message *message, const QString &text, const QString& iconName);

    void defaultMessage(const Api::Message *message, bool separatorBefore);
    void recipientMessage(const Api::Message *message);
    void callMessage(const Api::Message *message);
    void channelNameChangeMessage(const Api::Message *message);
    void channelIconChangeMessage(const Api::Message *message);
    void channelPinnedMessage(const Api::Message *message);
    void guildMemberJoinMessage(const Api::Message *message);
    void userPremiumGuildSubscriptionMessage(const Api::Message *message);
    void channelFollowAdd(const Api::Message *message);

    void enterEvent(QEvent *) override;
    void leaveEvent(QEvent *) override;
    void const setAvatar(const QString& avatarFileName);
    void const setReplyAvatar(const QString& avatarFileName);
    void addImage(const QString& filename, int width, int height);
    QString const processTime(const QTime& time);
    QString const processTimestamp(const QDateTime& dateTime);

    QString hoveredTimestamp;
    RoundedImage *avatar = nullptr;
    RoundedImage *replyAvatar = nullptr;
    Label *timestampLabel = nullptr;
    Label *textLabel = nullptr;
    Widget *mainMessage = nullptr;
    Widget *reply = nullptr;
    MarkdownLabel *content = nullptr;
    QVBoxLayout *dataLayout = nullptr;
    bool isFirst;

};

} // namespace Ui
