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

    void updateSpacing();
    void updateFont();
    void updateTheme();

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
    QString const getHashFileName(const QString& url);
    void const createReply(Api::Message *ref);
    Widget *createEmbed(Api::Embed *embed);
    Widget *createEmbedField(Api::EmbedField *embedField);

    QString hoveredTimestamp;
    RoundedImage *avatar = nullptr;
    RoundedImage *replyAvatar = nullptr;
    QLabel *name = nullptr;
    QLabel *username = nullptr;
    Label *timestampLabel = nullptr;
    Label *textLabel = nullptr;
    Label *date = nullptr;
    Label *replyContent = nullptr;
    Widget *mainMessage = nullptr;
    Widget *reply = nullptr;
    Widget *iconContainer = nullptr;
    MarkdownLabel *content = nullptr;
    QVBoxLayout *dataLayout = nullptr;
    int heightp = 0;
    bool isFirst;
    bool separatorBefore;
};

} // namespace Ui
