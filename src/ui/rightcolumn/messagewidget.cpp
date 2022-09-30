#include "ui/rightcolumn/messagewidget.h"

#include "ui/rightcolumn/attachmentfile.h"
#include "ui/rightcolumn/link.h"
#include "api/jsonutils.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QDateTime>
#include <QTextStream>
#include <QCryptographicHash>

namespace Ui {

MessageWidget::MessageWidget(Api::RessourceManager *rmp, const Api::Message *message, bool isFirstp, bool separatorBeforep, QWidget *parent)
    : Widget(parent)
{
    // Attributes initialization
    rm = rmp;
    isFirst = isFirstp;
    separatorBefore = separatorBeforep;

    switch (message->type) {
        case Api::Default:
            defaultMessage(message, separatorBefore);
            break;

        case Api::RecipientAdd:
        case Api::RecipientRemove:
            recipientMessage(message);
            break;

        case Api::CallT:
            callMessage(message);
            break;

        case Api::ChannelNameChange:
            channelNameChangeMessage(message);
            break;

        case Api::ChannelIconChange:
            channelIconChangeMessage(message);
            break;

        case Api::ChannelPinnedMessage:
            channelPinnedMessage(message);
            break;
        
        case Api::GuildMemberJoin:
            guildMemberJoinMessage(message);
            break;

        case Api::ChannelFollowAdd:
            channelFollowAdd(message);
            break;

        case Api::UserPremiumGuildSubscription:
        case Api::UserPremiumGuildSubscriptionTier1:
        case Api::UserPremiumGuildSubscriptionTier2:
        case Api::UserPremiumGuildSubscriptionTier3:
            userPremiumGuildSubscriptionMessage(message);
            break;

        case Api::Reply:
        default:
            defaultMessage(message, separatorBefore);
    }
}

void MessageWidget::updateSpacing()
{
    if (isFirst) {
        this->setContentsMargins(0, Settings::scale(Settings::messageGroupSpace), 0, 0);
        this->setMinimumHeight(height() - Settings::scale(Settings::lastMessageGroupSpace + Settings::messageGroupSpace));
    }
}

void MessageWidget::updateFont()
{
    if (iconContainer != nullptr) {
        iconContainer->setFixedWidth(Settings::scale(Settings::fontScaling * 4.5));
    }
    if (avatar != nullptr) {
        avatar->changeSize(Settings::scale(Settings::fontScaling * 2.5), Settings::scale(Settings::fontScaling * 2.5), Settings::scale(Settings::fontScaling * 1.25));
    }

    QFont font;
    font.setFamily("whitney");

    font.setPixelSize(Settings::scale(Settings::fontScaling));
    if (content != nullptr) {
        content->setFont(font);
    }
    if (name != nullptr) {
        name->setFont(font);
    }
    if (textLabel != nullptr) {
        textLabel->setFixedSize(QFontMetrics(font).horizontalAdvance(textLabel->text), Settings::scale(22));
        textLabel->setFont(font);
    }

    font.setPixelSize(Settings::scale(Settings::fontScaling - 2));
    if (replyContent != nullptr) {
        username->setFont(font);
        replyContent->setFont(font);
        //replyContent->setFixedSize(QFontMetrics(font).horizontalAdvance(replyContent->text), Settings::scale(18));
    }
    
    font.setPixelSize(Settings::scale(Settings::fontScaling - 4));
    if (date != nullptr) {
        date->setFixedSize(QFontMetrics(font).horizontalAdvance(date->text), Settings::scale(22));
        date->setFont(font);
    }
    if (timestampLabel != nullptr) {
        timestampLabel->setFixedSize(Settings::scale(Settings::fontScaling * 4.5), Settings::scale(16));
        timestampLabel->setFont(font);
    }
}

void const MessageWidget::setAvatar(const QString& avatarFileName)
{
    avatar->setRoundedImage(avatarFileName);
}

void const MessageWidget::setReplyAvatar(const QString& avatarFileName)
{
    replyAvatar->setRoundedImage(avatarFileName);
}

void MessageWidget::addImage(const QString& filename, int width, int height)
{
    QLabel *imageLabel = new QLabel(this);

    if (width >= height && width > Settings::scale(400)) {
        float ratio = width / Settings::scale(400);
        width /= ratio;
        height /= ratio;
    } else if (height > width && height > Settings::scale(400)) {
        float ratio = height / Settings::scale(400);
        height /= ratio;
        width /= ratio;
    }
    imageLabel->setFixedSize(width, height);

    QPixmap pixmap(filename);
    imageLabel->setPixmap(pixmap.scaled(width, height, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    dataLayout->addWidget(imageLabel);
}

void MessageWidget::enterEvent(QEvent *)
{
    if (textLabel != nullptr) {
        this->setBackgroundColor(Settings::BackgroundMessageHover);
    } else {
        if (mainMessage != nullptr)
            mainMessage->setBackgroundColor(Settings::BackgroundMessageHover);
        if (content != nullptr)
            content->setStyleSheet("color:" + Settings::colors[Settings::InteractiveHover].name());
        if (reply != nullptr)
            reply->setBackgroundColor(Settings::BackgroundMessageHover);
        if (!isFirst && timestampLabel != nullptr) timestampLabel->setText(" " + hoveredTimestamp);
    }
}

void MessageWidget::leaveEvent(QEvent *)
{
    if (textLabel != nullptr) {
        this->setBackgroundColor(Settings::None);
    } else {
        if (mainMessage != nullptr)
            mainMessage->setBackgroundColor(Settings::None);
        if (content != nullptr)
            content->setStyleSheet("color:" + Settings::colors[Settings::TextNormal].name());
        if (reply != nullptr)
            reply->setBackgroundColor(Settings::None);
        if (!isFirst && timestampLabel != nullptr) timestampLabel->setText("");
    }
}

QString const MessageWidget::processTime(const QTime& time)
{
    // Process the time to a nicer format

    // Get the hours and minutes
    int hour = time.hour();
    int minute = time.minute();
    // Process the hours and minutes
    QString hourString = hour > 12 ? QString::number(hour - 12) : QString::number(hour);
    QString minuteString = minute < 10 ? "0" + QString::number(minute) : QString::number(minute);
    // Create the final timestamp format
    QString messageTime = hourString + ":" + minuteString + " ";
    hour / 12 == 1 ? messageTime += "PM" : messageTime += "AM";
    return messageTime;
}

QString const MessageWidget::processTimestamp(const QDateTime& dateTime)
{
    // Get the date of the message and the current date to compare them
    QDate messageDate = dateTime.date();
    QDate currentDate = QDate::currentDate();
    QString date;
    if (messageDate != currentDate) {
        // The message was not sent today
        if (messageDate.year() == currentDate.year() &&
            messageDate.month() == currentDate.month() &&
            messageDate.day() == currentDate.day() - 1) {

            // The message was sent yesterday
            date = "Yesterday at " + processTime(dateTime.time());
        } else {
            // The message is older
            date = (messageDate.month() < 10 ? "0" + QString::number(messageDate.month()) : QString::number(messageDate.month())) + "/" +
                   (messageDate.day() < 10 ? "0" + QString::number(messageDate.day()) : QString::number(messageDate.day())) + "/" +
                   QString::number(messageDate.year());
        }
    } else {
        date = "Today at " + processTime(dateTime.time());
    }

    return date;
}

QString const MessageWidget::getHashFileName(const QString& url)
{
    return "embed_" + QString("%1").arg(QString(QCryptographicHash::hash(url.toUtf8(), QCryptographicHash::Sha1).toHex())) + (url.count() - url.lastIndexOf('.') > 5 ? "" : url.mid(url.lastIndexOf('.')));
}

void const MessageWidget::createReply(Api::Message *ref)
{
    reply = new Widget(this);
    reply->setFixedHeight(Settings::scale(28));
    QHBoxLayout *replyLayout = new QHBoxLayout(reply);

    Widget *replyIcon = new Widget(reply);
    replyIcon->setImage(":reply.png");
    replyIcon->setFixedSize(Settings::scale(68), Settings::scale(22));
    replyLayout->addWidget(replyIcon);

    if (!Settings::compactModeEnabled) {
        // Get the icon of the message
        if (ref->author.avatar.isNull()) {
            // Use an asset if the user doesn't have an icon
            replyLayout->addWidget(new RoundedImage(":user-icon-asset0.png", Settings::scale(16), Settings::scale(16), Settings::scale(8), reply));
        } else {
            // Request the avatar
            QString avatarFileName = ref->author.avatar + (ref->author.avatar.indexOf("a_") == 0 ? ".gif" : ".png");
            replyAvatar = new RoundedImage(Settings::scale(16), Settings::scale(16), Settings::scale(8), reply);
            replyLayout->addWidget(replyAvatar);
            rm->getImage([this](void *avatarFileName) {this->setReplyAvatar(*reinterpret_cast<QString *>(avatarFileName));}, "https://cdn.discordapp.com/avatars/" + ref->author.id + "/" + avatarFileName, avatarFileName);
        }
    }
    
    QFont font;
    font.setPixelSize(Settings::scale(Settings::fontScaling - 2));
    font.setFamily("whitney");

    username = new QLabel(ref->author.username, reply);
    username->setFont(font);
    username->setFixedHeight(Settings::scale(22));
    username->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
    username->setTextInteractionFlags(Qt::TextSelectableByMouse);
    username->setCursor(QCursor(Qt::PointingHandCursor));
    username->setStyleSheet("color:" + Settings::colors[Settings::HeaderPrimary].name() + "; background-color: none");
    if (Settings::roleColors != Settings::RoleColors::NotShown) {
        Api::Snowflake channelId = ref->channelId;
        Api::Snowflake authorId = ref->author.id;
        rm->getGuilds([this, channelId, authorId, replyLayout](void *guildsPtr){
            QVector<Api::Guild *> guilds = *reinterpret_cast<QVector<Api::Guild *> *>(guildsPtr);
            Api::Snowflake guildId = 0;
            for (int i = 0 ; i < guilds.size() ; i++) {
                QVector<Api::Channel *> channels = guilds[i]->channels;
                for (int j = 0 ; j < channels.size() ; j++) {
                    if (channelId == channels[j]->id) guildId = guilds[i]->id;
                }
            }
            if (guildId != 0) {
                rm->getGuildMember([this, guilds, guildId, replyLayout](void *guildMemberPtr){
                    QVector<Api::Snowflake> rolesIds = reinterpret_cast<Api::GuildMember *>(guildMemberPtr)->roles;
                    for (int i = 0 ; i < guilds.size() ; i++) {
                        if (guilds[i]->id == guildId) {
                            QVector<Api::Role *> guildRoles = guilds[i]->roles;
                            Api::Role *highestRole = nullptr;
                            for (int j = 0 ; j < guildRoles.size() ; j++) {
                                for (int k = 0 ; k < rolesIds.size() ; k++) {
                                    if (guildRoles[j]->id == QString::number(rolesIds[k].value) && (highestRole == nullptr || highestRole->position < guildRoles[j]->position))
                                        highestRole = guildRoles[j];
                                }
                            }
                            if (highestRole) {
                                qint32 colorInt = highestRole->color;
                                QColor color((colorInt & 0x00FF0000) >> 16, (colorInt & 0x0000FF00) >> 8, (colorInt & 0x000000FF));
                                if (Settings::roleColors == Settings::RoleColors::NextToName) {
                                    Widget *colorNameContainer = new Widget(this);
                                    colorNameContainer->setBackgroundColor(Settings::BackgroundSecondaryAlt);
                                    colorNameContainer->setBorderRadius(Settings::scale(5));
                                    colorNameContainer->setFixedSize(Settings::scale(18), Settings::scale(18));
                                    Widget *colorName = new Widget(colorNameContainer);
                                    colorName->setBackgroundColor(QColor::fromHsv(color.hue(), color.saturation() * (Settings::customColorSaturation ? Settings::saturation : 1.0f), color.value()));
                                    colorName->setBorderColor(Settings::Black);
                                    colorName->setBorderSize(Settings::scale(2));
                                    colorName->setBorderRadius(Settings::scale(6));
                                    colorName->setFixedSize(Settings::scale(12), Settings::scale(12));
                                    colorName->move(Settings::scale(3), Settings::scale(3));
                                    replyLayout->addWidget(colorNameContainer);
                                    replyLayout->addSpacing(Settings::scale(4));
                                } else {
                                    username->setStyleSheet("color:" + QColor::fromHsv(color.hue(), color.saturation() * (Settings::customColorSaturation ? Settings::saturation : 1.0f), color.value()).name() + "; background-color: none");
                                }
                            }
                            break;
                        }
                    }
                }, guildId, authorId);
            }
        });
    }

    replyContent = new MarkdownLabel(ref->content.replace('\n', "  "), Settings::InteractiveNormal, rm, nullptr);
    replyContent->setFont(font);
    replyContent->setFixedSize(QFontMetrics(font).horizontalAdvance(ref->content), Settings::scale(18));
    replyContent->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    replyContent->setCursor(QCursor(Qt::PointingHandCursor));

    replyLayout->addWidget(username);
    replyLayout->addWidget(replyContent);
    replyLayout->setSpacing(Settings::scale(4));
    replyLayout->addStretch(1);
}

Widget *MessageWidget::createEmbedField(Api::EmbedField *embedField)
{
    Widget *embedFieldWidget = new Widget(this);
    QVBoxLayout *embedFieldLayout = new QVBoxLayout(embedFieldWidget);
    embedFieldLayout->setSpacing(Settings::scale(2));

    QFont font;
    font.setPixelSize(Settings::scale(Settings::fontScaling - 3));
    font.setFamily("whitney");

    Label *fieldName = new Label(embedField->name, embedFieldWidget);
    fieldName->setFixedSize(Settings::scale(QFontMetrics(font).horizontalAdvance(embedField->name)), Settings::scale(20 * (embedField->name.count('\n') + 1)));

    MarkdownLabel *fieldValue = new MarkdownLabel(embedField->value, Settings::InteractiveHover, rm, embedFieldWidget);

    embedFieldLayout->addWidget(fieldName);
    embedFieldLayout->addWidget(fieldValue);

    return embedFieldWidget;
}

Widget *MessageWidget::createEmbed(Api::Embed *embed)
{
    Widget *embedWidget = new Widget(this);
    embedWidget->setBackgroundColor(Settings::BackgroundSecondary);
    if (embed->color != 0) {
        embedWidget->setBorderColor(QColor((embed->color & 0x00FF0000) >> 16, (embed->color & 0x0000FF00) >> 8, (embed->color & 0x000000FF)));
        embedWidget->setBorderSize(0, 0, 0, Settings::scale(4));
    }
    embedWidget->setBorderRadius(Settings::scale(4));
    QVBoxLayout *embedLayout = new QVBoxLayout(embedWidget);
    embedLayout->setContentsMargins(Settings::scale(20), Settings::scale(8), Settings::scale(16), Settings::scale(16));
    embedLayout->setSpacing(0);

    Widget *embedNoFooter = new Widget(embedWidget);
    QHBoxLayout *embedNoFooterLayout = new QHBoxLayout(embedNoFooter);
    embedNoFooterLayout->setContentsMargins(0, 0, 0, 0);
    embedNoFooterLayout->setSpacing(Settings::scale(16));

    Widget *embedContent = new Widget(embedNoFooter);
    QVBoxLayout *contentLayout = new QVBoxLayout(embedContent);
    contentLayout->setContentsMargins(0, Settings::scale(8), 0, Settings::scale(8));
    contentLayout->setSpacing(Settings::scale(8));

    QFont font;
    font.setPixelSize(Settings::scale(Settings::fontScaling - 3));
    font.setFamily("whitney");

    if (embed->author != nullptr && !embed->author->name.isNull()) {
        Widget *author = new Widget(embedContent);
        QHBoxLayout *authorLayout = new QHBoxLayout(author);
        authorLayout->setSpacing(0);
        authorLayout->setContentsMargins(0, 0, 0, 0);

        if (!embed->author->iconUrl.isNull()) {
            RoundedImage *authorAvatar = new RoundedImage(Settings::scale(24), Settings::scale(24), Settings::scale(12), author);
            authorLayout->addWidget(authorAvatar);
            authorLayout->addSpacing(8);
            rm->getImage([authorAvatar](void *avatarFileName){authorAvatar->setRoundedImage(*reinterpret_cast<QString *>(avatarFileName));}, embed->author->iconUrl, getHashFileName(embed->author->iconUrl));
        }

        if (embed->author->url.isNull()) {
            Label *authorName = new Label(embed->author->name, author);
            authorName->setTextColor(Settings::HeaderPrimary);
            authorName->setFixedSize(Settings::scale(QFontMetrics(font).horizontalAdvance(embed->author->name)), Settings::scale(20));
            authorLayout->addWidget(authorName);
        } else {
            Link *authorLink = new Link(embed->author->name, embed->author->url, Settings::scale(Settings::fontScaling - 3), Settings::HeaderPrimary, author);
            authorLink->setFixedSize(Settings::scale(QFontMetrics(font).horizontalAdvance(embed->author->name)), Settings::scale(20));
            authorLayout->addWidget(authorLink);
        }
        authorLayout->addStretch(1);

        contentLayout->addWidget(author);
    }

    if (!embed->title.isNull()) {
        font.setPixelSize(Settings::scale(Settings::fontScaling));
        if (embed->url.isNull()) {
            MarkdownLabel *title = new MarkdownLabel(embed->title, Settings::HeaderPrimary, rm, embedContent);
            contentLayout->addWidget(title);
        } else {
            Link *titleLink = new Link(embed->title, embed->url, Settings::scale(Settings::fontScaling - 3), Settings::Link, embedContent);
            titleLink->setFixedSize(Settings::scale(QFontMetrics(font).horizontalAdvance(embed->title)), Settings::scale(20));
            contentLayout->addWidget(titleLink);
        }
    }

    if (!embed->description.isNull()) {
        font.setPixelSize(Settings::scale(Settings::fontScaling - 3));
        MarkdownLabel *description = new MarkdownLabel(embed->description, Settings::TextNormal, rm, embedContent);
        contentLayout->addWidget(description);
    }

    Widget *fieldsWidget = new Widget(embedContent);
    QVBoxLayout *fieldsLayout = new QVBoxLayout(fieldsWidget);
    fieldsLayout->setContentsMargins(0, Settings::scale(5), 0, 0);
    fieldsLayout->setSpacing(Settings::scale(5));

    Widget *inlinedFields = nullptr;
    QHBoxLayout *inlinedFieldsLayout = nullptr;
    QVector<Api::EmbedField *> fields = embed->fields;
    for (int i = 0 ; i < fields.size() ; i++) {
        if (fields[i]->einline == (qint8)Optional::True) {
            if (inlinedFields == nullptr) {
                inlinedFields = new Widget(embedContent);
                inlinedFieldsLayout = new QHBoxLayout(inlinedFields);
                inlinedFieldsLayout->setContentsMargins(0, 0, 0, 0);
                inlinedFieldsLayout->setSpacing(0);
            }
            inlinedFieldsLayout->addWidget(createEmbedField(fields[i]));
        } else {
            if (inlinedFields != nullptr) {
                fieldsLayout->addWidget(inlinedFields);
                inlinedFields = nullptr;
                inlinedFieldsLayout = nullptr;
            }
            fieldsLayout->addWidget(createEmbedField(fields[i]));
        }
    }
    contentLayout->addWidget(fieldsWidget);

    if (embed->image != nullptr && !embed->image->url.isNull()) {
        Widget *image = new Widget(embedContent);
        rm->getImage([image](void *imageName){image->setImage(*reinterpret_cast<QString *>(imageName));}, embed->image->url, getHashFileName(embed->image->url));
        contentLayout->addWidget(image);
    }

    if (embed->video != nullptr && !embed->video->url.isNull()) {
        Widget *video = new Widget(embedContent);
        rm->getImage([video](void *videoName){video->setMovie(new QMovie(*reinterpret_cast<QString *>(videoName)), true);}, embed->video->url, getHashFileName(embed->video->url));
        contentLayout->addWidget(video);
    }

    embedNoFooterLayout->addWidget(embedContent);

    if (embed->thumbnail != nullptr && !embed->thumbnail->url.isNull()) {
        Widget *thumbnailContainer = new Widget(embedNoFooter);
        QHBoxLayout *thumbnailContainerLayout = new QHBoxLayout(thumbnailContainer);
        thumbnailContainerLayout->setContentsMargins(0, Settings::scale(8), 0, 0);
        Widget *thumbnail = new Widget(thumbnailContainer);
        thumbnailContainerLayout->addWidget(thumbnail);
        rm->getImage([thumbnail](void *thumbnailName){thumbnail->setImage(*reinterpret_cast<QString *>(thumbnailName));}, embed->thumbnail->url, getHashFileName(embed->thumbnail->url));
        embedNoFooterLayout->addWidget(thumbnail);
    }
    
    embedLayout->addWidget(embedNoFooter);

    if (embed->footer != nullptr && !embed->footer->text.isNull()) {
        Widget *footer = new Widget(embedWidget);
        QHBoxLayout *footerLayout = new QHBoxLayout(footer);
        footerLayout->setSpacing(0);
        footerLayout->setContentsMargins(0, 0, 0, 0);

        if (!embed->footer->iconUrl.isNull()) {
            RoundedImage *footerIcon = new RoundedImage(Settings::scale(20), Settings::scale(20), Settings::scale(10), footer);
            footerLayout->addWidget(footerIcon);
            footerLayout->addSpacing(8);
            rm->getImage([footerIcon](void *iconFileName){footerIcon->setRoundedImage(*reinterpret_cast<QString *>(iconFileName));}, embed->footer->iconUrl, getHashFileName(embed->footer->iconUrl));
        }

        Label *footerText = new Label(embed->footer->text, footer);
        footerText->setTextColor(Settings::TextNormal);
        footerText->setFixedSize(Settings::scale(QFontMetrics(font).horizontalAdvance(embed->footer->text)), Settings::scale(20));
        footerLayout->addWidget(footerText);

        if (!embed->timestamp.isNull()) {
            QString processedTimestamp = processTimestamp(QDateTime::fromString(embed->timestamp, Qt::ISODate).toLocalTime());
            Label *timestamp = new Label(" • " + processedTimestamp, footer);
            timestamp->setTextColor(Settings::TextNormal);
            timestamp->setFixedSize(Settings::scale(QFontMetrics(font).horizontalAdvance(" • " + processedTimestamp)), Settings::scale(20));
            footerLayout->addWidget(timestamp);
        }
        footerLayout->addStretch(1);

        embedLayout->addWidget(footer);
    } else if (!embed->timestamp.isNull()) {
        QString processedTimestamp = processTimestamp(QDateTime::fromString(embed->timestamp, Qt::ISODate).toLocalTime());
        Label *timestamp = new Label(processedTimestamp, embedWidget);
        timestamp->setFixedSize(Settings::scale(QFontMetrics(font).horizontalAdvance(processedTimestamp)), Settings::scale(20));
        embedLayout->addWidget(timestamp);
    }

    return embedWidget;
}

Widget *MessageWidget::createReaction(const Api::Reaction& reaction)
{
    Widget *reactionContainer = new Widget();
    reactionContainer->setFixedHeight(Settings::scale(20));
    reactionContainer->setContentsMargins(Settings::scale(2), Settings::scale(6), Settings::scale(2), Settings::scale(6));
    reactionContainer->setBorderRadius(Settings::scale(8));
    reactionContainer->setBackgroundColor(Settings::BackgroundTertiary);
    QHBoxLayout *reactionsLayout = new QHBoxLayout(reactionContainer);
    reactionsLayout->setContentsMargins(0, 0, 0, 0);
    reactionsLayout->setSpacing(Settings::scale(6));

    Label *emoji = new Label(reactionContainer);
    emoji->setFixedSize(Settings::scale(16), Settings::scale(16));
    if (reaction.emoji.id.value) {
        rm->getImage([emoji](void *fileName){
            emoji->setImage(*reinterpret_cast<QString *>(fileName));
        }, "https://cdn.discordapp.com/emojis/" + reaction.emoji.id, QString::number(reaction.emoji.id.value));
    } else {
        emoji->setText(reaction.emoji.name);
    }

    QFont font;
    font.setPixelSize(Settings::scale(Settings::fontScaling - 2));
    font.setFamily("whitney");
    Label *count = new Label(QString::number(reaction.count), reactionContainer);
    count->setFixedSize(QFontMetrics(font).horizontalAdvance(QString::number(reaction.count)), Settings::scale(Settings::fontScaling - 2));
    count->setTextColor(Settings::InteractiveNormal);

    reactionsLayout->addWidget(emoji, 0, Qt::AlignVCenter);
    reactionsLayout->addWidget(count, 0, Qt::AlignVCenter);

    return reactionContainer;
}

Widget *MessageWidget::createReactions(QVector<Api::Reaction *> reactions)
{
    Widget *reactionsContainer = new Widget();
    QHBoxLayout *reactionsLayout = new QHBoxLayout(reactionsContainer);
    reactionsLayout->setContentsMargins(0, 0, 0, 0);
    reactionsLayout->setSpacing(Settings::scale(4));
    for (int i = 0 ; i < reactions.size() ; i++)
        reactionsLayout->addWidget(createReaction(*reactions[i]));
    reactionsLayout->addStretch();
    return reactionsContainer;
}

void MessageWidget::defaultMessage(const Api::Message *message, bool separatorBefore)
{
    // Create the main widgets
    QVBoxLayout *layout = new QVBoxLayout(this);
    mainMessage = new Widget(this);
    QHBoxLayout *mainLayout = new QHBoxLayout(mainMessage);
    Widget *data = new Widget(mainMessage);
    dataLayout = new QVBoxLayout(data);
    Widget *compactMessageContent = new Widget(data);
    QHBoxLayout *compactLayout = new QHBoxLayout(compactMessageContent);
    iconContainer = new Widget(mainMessage);
    QVBoxLayout *iconLayout = new QVBoxLayout(iconContainer);
    
    QFont font;
    font.setPixelSize(Settings::scale(Settings::fontScaling - 2));
    font.setFamily("whitney");

    // Get the date and time of the message
    QDateTime dateTime = QDateTime::fromString(message->timestamp, Qt::ISODate).toLocalTime();

    if (message->referencedMessage != nullptr && message->referencedMessage->id != 0) {
        createReply(message->referencedMessage);
        heightp += 22;
        isFirst = true;
        layout->addWidget(reply);
    }
    if (isFirst && !Settings::compactModeEnabled) {
        // The message is not grouped to another message

        // Variable creation
        const Api::User& author = message->author;
        QString avatarId = author.avatar;

        heightp += 40;

        // Get the icon of the message
        if (avatarId.isNull()) {
            // Use an asset if the user doesn't have an icon
            avatar = new RoundedImage(":user-icon-asset0.png", Settings::scale(40), Settings::scale(40), Settings::scale(20), iconContainer);
            iconLayout->addWidget(avatar);
            iconLayout->setAlignment(avatar, Qt::AlignTop | Qt::AlignHCenter);
        } else {
            // Request the avatar
            QString avatarFileName = avatarId + (avatarId.indexOf("a_") == 0 ? ".gif" : ".png");
            avatar = new RoundedImage(Settings::scale(40), Settings::scale(40), Settings::scale(20), iconContainer);
            iconLayout->addWidget(avatar);
            iconLayout->setAlignment(avatar, Qt::AlignTop | Qt::AlignHCenter);
            rm->getImage([this](void *avatarFileName) {this->setAvatar(*reinterpret_cast<QString *>(avatarFileName));}, "https://cdn.discordapp.com/avatars/" + author.id + "/" + avatarFileName, avatarFileName);
        }

        // Widget to show some infos of the message
        Widget *messageInfos = new Widget(data);
        QHBoxLayout *infosLayout = new QHBoxLayout(messageInfos);
        name = new QLabel(author.username, messageInfos);
        font.setPixelSize(Settings::scale(Settings::fontScaling));
        name->setFont(font);
        name->setTextInteractionFlags(Qt::TextSelectableByMouse);
        name->setCursor(QCursor(Qt::PointingHandCursor));
        name->setCursor(QCursor(Qt::IBeamCursor));
        name->setStyleSheet("color:" + Settings::colors[Settings::HeaderPrimary].name() + "; background-color: none");
        if (Settings::roleColors != Settings::RoleColors::NotShown) {
            Api::Snowflake channelId = message->channelId;
            Api::Snowflake authorId = message->author.id;
            rm->getGuilds([this, channelId, authorId, infosLayout](void *guildsPtr){
                QVector<Api::Guild *> guilds = *reinterpret_cast<QVector<Api::Guild *> *>(guildsPtr);
                Api::Snowflake guildId = 0;
                for (int i = 0 ; i < guilds.size() ; i++) {
                    QVector<Api::Channel *> channels = guilds[i]->channels;
                    for (int j = 0 ; j < channels.size() ; j++) {
                        if (channelId == channels[j]->id) guildId = guilds[i]->id;
                    }
                }
                if (guildId != 0) {
                    rm->getGuildMember([this, guilds, guildId, infosLayout](void *guildMemberPtr){
                        QVector<Api::Snowflake> rolesIds = reinterpret_cast<Api::GuildMember *>(guildMemberPtr)->roles;
                        for (int i = 0 ; i < guilds.size() ; i++) {
                            if (guilds[i]->id == guildId) {
                                QVector<Api::Role *> guildRoles = guilds[i]->roles;
                                Api::Role *highestRole = nullptr;
                                for (int j = 0 ; j < guildRoles.size() ; j++) {
                                    for (int k = 0 ; k < rolesIds.size() ; k++) {
                                        if (guildRoles[j]->id == QString::number(rolesIds[k].value) && (highestRole == nullptr || highestRole->position < guildRoles[j]->position))
                                            highestRole = guildRoles[j];
                                    }
                                }
                                if (highestRole) {
                                    qint32 colorInt = highestRole->color;
                                    QColor color((colorInt & 0x00FF0000) >> 16, (colorInt & 0x0000FF00) >> 8, (colorInt & 0x000000FF));
                                    if (Settings::roleColors == Settings::RoleColors::NextToName) {
                                        Widget *colorNameContainer = new Widget(this);
                                        colorNameContainer->setBackgroundColor(Settings::BackgroundSecondaryAlt);
                                        colorNameContainer->setBorderRadius(Settings::scale(5));
                                        colorNameContainer->setFixedSize(Settings::scale(20), Settings::scale(20));
                                        Widget *colorName = new Widget(colorNameContainer);
                                        colorName->setBackgroundColor(QColor::fromHsv(color.hue(), color.saturation() * (Settings::customColorSaturation ? Settings::saturation : 1.0f), color.value()));
                                        colorName->setBorderColor(Settings::Black);
                                        colorName->setBorderSize(Settings::scale(2));
                                        colorName->setBorderRadius(Settings::scale(7));
                                        colorName->setFixedSize(Settings::scale(14), Settings::scale(14));
                                        colorName->move(Settings::scale(3), Settings::scale(3));
                                        infosLayout->addWidget(colorNameContainer);
                                        infosLayout->addSpacing(Settings::scale(4));
                                    } else {
                                        name->setStyleSheet("color:" + QColor::fromHsv(color.hue(), color.saturation() * (Settings::customColorSaturation ? Settings::saturation : 1.0f), color.value()).name() + "; background-color: none");
                                    }
                                }
                                break;
                            }
                        }
                    }, guildId, authorId);
                }
            });
        }
        date = new Label(processTimestamp(dateTime), messageInfos);
        font.setPixelSize(Settings::scale(Settings::fontScaling - 4));
        date->setTextColor(Settings::TextMuted);
        date->setFixedSize(QFontMetrics(font).horizontalAdvance(processTimestamp(dateTime)), Settings::scale(22));
        date->setFont(font);
        date->setTextColor(Settings::TextMuted);

        // Add widgets and style the infos layout
        infosLayout->addWidget(name);
        infosLayout->addWidget(date);
        infosLayout->insertStretch(-1);
        infosLayout->setContentsMargins(0, 0, 0, 0);

        // Add the message infos and style the data layout
        dataLayout->addWidget(messageInfos);
        dataLayout->setSpacing(0);

        // Style the message widget
        this->setContentsMargins(0, separatorBefore ? 0 : Settings::scale(Settings::messageGroupSpace), 0, 0);
    } else {
        heightp += 22;

        // Add the label that shows the timestamp when the message is hovered
        hoveredTimestamp = processTime(dateTime.time());
        timestampLabel = new Label(this);
        font.setPixelSize(Settings::scale(Settings::fontScaling - 4));
        timestampLabel->setFont(font);
        timestampLabel->setFixedHeight(Settings::scale(22));
        timestampLabel->setTextColor(Settings::TextMuted);
        timestampLabel->setFlags(Qt::AlignCenter);
        iconLayout->addWidget(timestampLabel);
        iconLayout->setContentsMargins(0, 0, 0, 0);
        mainLayout->setContentsMargins(0, 0, 0, 0);

        if (Settings::compactModeEnabled) {
            heightp += 4;

            if (isFirst) {
                timestampLabel->setText(hoveredTimestamp);
                timestampLabel->setFixedSize(Settings::scale(Settings::fontScaling * 4.5), Settings::scale(16));
                timestampLabel->setFlags(Qt::AlignCenter);
            }

            // Widget to show some infos of the message
            Widget *messageInfos = new Widget(data);
            QHBoxLayout *infosLayout = new QHBoxLayout(messageInfos);
            name = new QLabel(message->author.username, messageInfos);
            font.setBold(true);
            font.setPixelSize(Settings::scale(Settings::fontScaling));
            name->setFont(font);
            name->setTextInteractionFlags(Qt::TextSelectableByMouse);
            name->setCursor(QCursor(Qt::PointingHandCursor));
            name->setStyleSheet("color:" + Settings::colors[Settings::HeaderPrimary].name() + "; background-color: none");
            name->setCursor(QCursor(Qt::IBeamCursor));
            font.setBold(false);

            // Add widgets and style the infos layout
            infosLayout->addWidget(name);
            infosLayout->insertStretch(-1);
            infosLayout->setContentsMargins(0, 0, 0, 0);
            
            compactLayout->addWidget(messageInfos, 0, Qt::AlignmentFlag::AlignTop);
            compactLayout->setSpacing(8);
            compactLayout->setContentsMargins(0, 0, 0, 0);
        }
    }

    // Style the icon container
    iconContainer->setFixedWidth(Settings::scale(Settings::fontScaling * 4.5));
    iconContainer->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Minimum);

    // Create and style the content label
    content = nullptr;
    if (!message->content.isNull() && !message->content.isEmpty()) {
        content = new MarkdownLabel(message->content, Settings::InteractiveHover, rm, this);
        if (Settings::compactModeEnabled) {
            compactLayout->addWidget(content, 1);
        } else {
            dataLayout->addWidget(content, 0, Qt::AlignmentFlag::AlignVCenter);
        }
    }

    if (Settings::compactModeEnabled) {
        dataLayout->addWidget(compactMessageContent);
    }

    // Style the data layout
    dataLayout->setContentsMargins(0, 0, 0, 0);

    if (!message->attachments.empty()) {
        QVector<Api::Attachment *> attachments = message->attachments;
        for (int i = 0 ; i < attachments.size() ; i++) {
            Api::Attachment *attachment = attachments[i];
            if (attachment->contentType != nullptr
            && attachment->contentType.indexOf("image") != -1
            && attachment->contentType.indexOf("svg") == -1) {
                QString filename = attachment->id +
                        attachment->filename.mid(0, attachment->filename.lastIndexOf('.'));
                if (attachment->width >= attachment->height && attachment->width > 400)
                    heightp += attachment->height / (attachment->width / 400);
                else if (attachment->height > attachment->width && attachment->height > 400)
                    heightp += attachment->height / (attachment->height / 400);
                else 
                    heightp += attachment->height;
                rm->getImage([this, attachment](void *filename) {
                    this->addImage(*reinterpret_cast<QString *>(filename), attachment->width, attachment->height);
                }, attachment->proxyUrl, filename);
            } else {
                dataLayout->addWidget(new AttachmentFile(rm->requester, attachment, data));
                heightp += 52;
            }
        }
    }

    if (!message->embeds.isEmpty()) {
        QVector<Api::Embed *> embeds = message->embeds;
        for (int i = 0 ; i < embeds.size() ; i++) {
            dataLayout->addSpacing(8);
            dataLayout->addWidget(createEmbed(embeds[i]));
            dataLayout->addSpacing(8);
        }
    }

    if (!message->reactions.isEmpty())
        dataLayout->addWidget(createReactions(message->reactions));

    mainMessage->setMinimumHeight(Settings::scale(heightp));

    // Add widgets to the main layout and style it
    mainLayout->addWidget(iconContainer);
    mainLayout->addWidget(data);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    layout->addWidget(mainMessage);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);
}

void MessageWidget::iconMessage(const Api::Message *message, const QString &text, const QString& iconName)
{
    QFont font;
    font.setPixelSize(Settings::scale(Settings::fontScaling));
    font.setFamily("whitney");

    QHBoxLayout *layout = new QHBoxLayout(this);
    QLabel *icon = new QLabel(this);
    textLabel = new Label(text, this);
    textLabel->setFixedSize(QFontMetrics(font).horizontalAdvance(text), Settings::scale(22));
    textLabel->setFlags(Qt::AlignVCenter);
    textLabel->setFont(font);

    font.setPixelSize(Settings::scale(Settings::fontScaling - 4));
    Label *timestampLabel = new Label(processTimestamp(QDateTime::fromString(message->timestamp, Qt::ISODate).toLocalTime()), this);
    timestampLabel->setFixedSize(QFontMetrics(font).horizontalAdvance(processTimestamp(QDateTime::fromString(message->timestamp, Qt::ISODate).toLocalTime())), Settings::scale(16));
    timestampLabel->setFont(font);

    icon->setFixedWidth(Settings::scale(44));
    icon->setPixmap(QPixmap(":" + iconName));

    textLabel->setTextColor(Settings::ChannelIcon);
    textLabel->setCursor(QCursor(Qt::IBeamCursor));
    timestampLabel->setTextColor(Settings::TextMuted);

    layout->addSpacing(Settings::scale(28));
    layout->addWidget(icon);
    layout->addWidget(textLabel, 0, Qt::AlignmentFlag::AlignBottom);
    layout->addSpacing(Settings::scale(4));
    layout->addWidget(timestampLabel);
    layout->addStretch(1);

    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);
}

void MessageWidget::recipientMessage(const Api::Message *message)
{
    QString arrowName;
    QString text;

    if (message->type == Api::RecipientAdd) {
        arrowName = "green-right-arrow.svg";
        text = message->mentions[0]->username + " added " +
                message->author.username + " to the group.";
    } else {
        arrowName = "red-left-arrow.svg";
        text = message->mentions[0]->username + " left the group.";
    }

    iconMessage(message, text, arrowName);
}

void MessageWidget::callMessage(const Api::Message *message)
{
    rm->getClient([message, this](void *clientPtr){

    QString phoneName;
    QString text;
    Api::Snowflake clientId = reinterpret_cast<Api::Client *>(clientPtr)->id;
    QDateTime dateTime = QDateTime::fromString(message->timestamp, Qt::ISODate).toLocalTime();

    QVector<Api::Snowflake> participants = message->call->participants;
    bool participated = false;
    for (int i = 0 ; i < participants.size() ; i++) {
        if (participants[i] == clientId) participated = true;
    }

    if (!participated) {
        text = "You missed a call from " + message->author.username;
    } else {
        text = message->author.username + " started a call";
    }
    QString endedTimestampStr = message->call->endedTimestamp;
    if (!endedTimestampStr.isNull()) {
        phoneName = "gray-phone.svg";
        qint64 duration = QDateTime::fromString(endedTimestampStr, Qt::ISODate)
                .toLocalTime().toSecsSinceEpoch() -
                dateTime.toLocalTime().toSecsSinceEpoch();
        if (duration < 60) {
            text += " that lasted a few seconds";
        } else if (duration < 3600) {
            text += " that lasted ";
            if (duration < 120)
                text += "a minute";
            else
                text += QString::number(duration / 60) + " minutes";
        } else {
            text += " that lasted ";
            if (duration < 7200)
                text += "a hour";
            else
                text += QString::number(duration / 3600) + " hours";
        }
    } else {
        phoneName = "green-phone.svg";
    }
    text += ".";

    if (clientId == message->author.id)
        phoneName = "green-phone.svg";

    iconMessage(message, text, phoneName);

    });
}

void MessageWidget::channelNameChangeMessage(const Api::Message *message)
{
    iconMessage(message, message->author.username + " changed the channel name: " + message->content, "pen.svg");
}

void MessageWidget::channelIconChangeMessage(const Api::Message *message)
{
    iconMessage(message, message->author.username + " changed the channel icon.", "pen.svg");
}

void MessageWidget::channelPinnedMessage(const Api::Message *message)
{
    iconMessage(message, message->author.username + " pinned a message to this channel. See all pinned messages.", "pin.svg");
}

void MessageWidget::userPremiumGuildSubscriptionMessage(const Api::Message *message)
{
    QString text = message->author.username + " just boosted the server.";
    if (message->type == Api::UserPremiumGuildSubscriptionTier1)
        text += "(Tier 1)";
    else if (message->type == Api::UserPremiumGuildSubscriptionTier2)
        text += "(Tier 2)";
    else if (message->type == Api::UserPremiumGuildSubscriptionTier3)
        text += "(Tier 3)";

    iconMessage(message, text, "boost.svg");
}

void MessageWidget::guildMemberJoinMessage(const Api::Message *message)
{
    int randInt = rand() % 39;
    QFile messageFile(":welcome-messages.txt");
    messageFile.open(QIODevice::ReadOnly);
    for (int i = 0 ; i < randInt ; i++) {
        messageFile.readLine();
    }
    QString text = messageFile.readLine();
    int index = text.indexOf("{}");
    while (index != -1){
        text.replace(index, 2, message->author.username);
        index = text.indexOf("{}");
    }

    iconMessage(message, text, "green-right-arrow.svg");
}

void MessageWidget::channelFollowAdd(const Api::Message *message)
{
    iconMessage(message, message->content + " has added " + message->author.username
     + " to this channel. Its most important updates will show up here.", "green-right-arrow.svg");
}

void MessageWidget::updateTheme()
{
    if (content)
        content->setStyleSheet("background-color:" + Settings::colors[Settings::BackgroundPrimary].name()
            + ";color:" + Settings::colors[Settings::InteractiveHover].name());
}

} // namespace Ui
