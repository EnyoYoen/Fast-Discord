#include "ui/messagewidget.h"

#include "api/jsonutils.h"
#include "ui/attachmentfile.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QDateTime>

#include <fstream>
#include <cstring>
#include <iostream>

namespace Ui {

MessageWidget::MessageWidget(Api::RessourceManager *rmp, Api::Message *message, bool isFirstp, bool separatorBefore, QWidget *parent)
    : QWidget(parent)
{
    // Attributes initialization
    rm = rmp;
    isFirst = isFirstp;

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

        default:
            defaultMessage(message, separatorBefore);
    }

    // Style
    this->setMinimumHeight(26);
    this->setStyleSheet("padding: 0px;"
                        "margin: 0px;");
}

void MessageWidget::setAvatar(const std::string& avatarFileName)
{
    avatar->setImage(avatarFileName);
}

void MessageWidget::addImage(const std::string& filename, int width, int height)
{
    QLabel *imageLabel = new QLabel(this);

    if (width >= height && width > 400) {
        float ratio = width / 400;
        width /= ratio;
        height /= ratio;
    } else if (height > width && height > 400) {
        float ratio = height / 400;
        height /= ratio;
        width /= ratio;
    }
    imageLabel->setFixedSize(width, height);

    QPixmap pixmap(QString(filename.c_str()));
    imageLabel->setPixmap(pixmap.scaled(width, height, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    dataLayout->addWidget(imageLabel);
}

void MessageWidget::enterEvent(QEvent *)
{
    // Mouse hover : change the stylesheet and show the timestamp label
    setStyleSheet("background-color: #32353B;");
    if (content != nullptr)
        content->setStyleSheet("background-color: #32353B;"
                               "color: #DCDDDE;");
    if (!isFirst && timestampLabel != nullptr) timestampLabel->setText(" " + hoveredTimestamp);
}

void MessageWidget::leaveEvent(QEvent *)
{
    // Reset the stylesheet and hide the timestamp label
    setStyleSheet("background-color: none;");
    if (content != nullptr)
        content->setStyleSheet("background-color: #36393F;"
                               "color: #DCDDDE;");
    if (!isFirst && timestampLabel != nullptr) timestampLabel->setText("");
}

QString MessageWidget::processTime(QTime time)
{
    // Process the time to a nicer format

    // Get the hours and minutes
    int hour = time.hour();
    int minute = time.minute();
    // Process the hours and minutes
    std::string hourString = hour > 12 ? std::to_string(hour - 12) : std::to_string(hour);
    std::string minuteString = minute < 10 ? "0" + std::to_string(minute) : std::to_string(minute);
    // Create the final timestamp format
    QString messageTime = QString((hourString + ":" + minuteString + " ").c_str());
    hour / 12 == 1 ? messageTime += "PM" : messageTime += "AM";
    return messageTime;
}

QString MessageWidget::processTimestamp(QDateTime dateTime)
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
            date = QString(((messageDate.month() < 10 ? "0" + std::to_string(messageDate.month()) : std::to_string(messageDate.month())) + "/" +
                    (messageDate.day() < 10 ? "0" + std::to_string(messageDate.day()) : std::to_string(messageDate.day())) + "/" +
                    std::to_string(messageDate.year())).c_str());
        }
    } else {
        date = "Today at " + processTime(dateTime.time());
    }

    return date;
}

void MessageWidget::defaultMessage(Api::Message *message, bool separatorBefore)
{
    // Create the main widgets
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    QWidget *data = new QWidget(this);
    dataLayout = new QVBoxLayout(data);
    QWidget *iconContainer = new QWidget(this);
    QVBoxLayout *iconLayout = new QVBoxLayout(iconContainer);

    // Get the date and time of the message
    QDateTime dateTime = QDateTime::fromString(QString((*message->timestamp).c_str()), Qt::ISODate).toLocalTime();

    if (isFirst) {
        // The message is not grouped to another message

        // Variable creation
        Api::User& author = *message->author;
        std::string *avatarId = author.avatar;

        // Get the icon of the message
        if (avatarId == nullptr || *avatarId == "") {
            // Use an asset if the user doesn't have an icon
            iconLayout->addWidget(new RoundedImage("res/images/png/user-icon-asset0.png", 40, 40, 20, iconContainer));
        } else {
            // Request the icon
            std::string avatarFileName = *author.avatar + (author.avatar->rfind("a_") == 0 ? ".gif" : ".png");
            avatar = new RoundedImage(40, 40, 20, iconContainer);
            iconLayout->addWidget(avatar);
            rm->getImage([this](void *avatarFileName) {this->setAvatar(*reinterpret_cast<std::string *>(avatarFileName));}, "https://cdn.discordapp.com/avatars/" + *author.id + "/" + avatarFileName, avatarFileName);
        }

        // Widget to show some infos of the message
        QWidget *messageInfos = new QWidget(data);
        QHBoxLayout *infosLayout = new QHBoxLayout(messageInfos);
        QLabel *name = new QLabel((*author.username).c_str(), messageInfos);
        QLabel *date = new QLabel(processTimestamp(dateTime), messageInfos);

        // Style the name label
        name->setTextInteractionFlags(Qt::TextSelectableByMouse);
        name->setCursor(QCursor(Qt::IBeamCursor));
        name->setStyleSheet("color: #FFF");

        // Style the date
        date->setTextInteractionFlags(Qt::TextSelectableByMouse);
        date->setStyleSheet("color: #72767D;");

        // Add widgets and style the infos layout
        infosLayout->addWidget(name);
        infosLayout->addWidget(date);
        infosLayout->insertStretch(-1);
        infosLayout->setContentsMargins(0, 0, 0, 0);

        // Add the message infos and style the data layout
        dataLayout->addWidget(messageInfos);
        dataLayout->setSpacing(0);

        // Style the message widget
        this->setContentsMargins(0, separatorBefore ? 0 : 20, 0, 0);
    } else {
        // Add the label that shows the timestamp when the message is hovered
        hoveredTimestamp = processTime(dateTime.time());
        timestampLabel = new QLabel(this);
        timestampLabel->setFixedHeight(22);
        iconLayout->addWidget(timestampLabel);
        iconLayout->setContentsMargins(0, 0, 0, 0);
        mainLayout->setContentsMargins(0, 0, 0, 0);
    }

    // Style the icon container
    iconContainer->setFixedWidth(72);
    iconContainer->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Minimum);
    iconContainer->setStyleSheet("color: #72767D;");
    iconContainer->setLayout(iconLayout);

    // Create and style the content label
    content = nullptr;
    if (message->content != nullptr && *message->content != "") {
        content = new MarkdownLabel(*message->content, rm, this);
        dataLayout->addWidget(content);
    }

    // Style the data layout
    dataLayout->setContentsMargins(0, 0, 0, 0);

    if (message->attachments != nullptr) {
        std::vector<Api::Attachment *> attachments = *message->attachments;
        for (unsigned int i = 0 ; i < attachments.size() ; i++) {
            Api::Attachment *attachment = attachments[i];
            if (attachments[i]->contentType != nullptr
              && attachments[i]->contentType->find("image") != std::string::npos
              && attachments[i]->contentType->find("svg") == std::string::npos) {
                std::string filename = *attachments[i]->id +
                        attachments[i]->filename->substr(attachments[i]->filename->find_last_of('.'));
                rm->getImage([this, attachment](void *filename) {
                    this->addImage(*reinterpret_cast<std::string *>(filename), attachment->width, attachment->height);
                }, *attachments[i]->proxyUrl, filename);
            } else {
                dataLayout->addWidget(new AttachmentFile(rm->requester, attachment, data));
            }
        }
    }

    // Add widgets to the main layout and style it
    mainLayout->addWidget(iconContainer);
    mainLayout->addWidget(data);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);
}

void MessageWidget::recipientMessage(Api::Message *message)
{
    std::string arrowName;
    std::string text;

    if (message->type == Api::RecipientAdd) {
        arrowName = "green-right-arrow.svg";
        text = *(*message->mentions)[0]->username + " added " +
                *message->author->username + " to the group.";
    } else {
        arrowName = "red-left-arrow.svg";
        text = *(*message->mentions)[0]->username + " left the group.";
    }

    QHBoxLayout *layout = new QHBoxLayout(this);
    QWidget *spacer = new QWidget(this);
    QLabel *arrow = new QLabel(this);
    QLabel *textLabel = new QLabel(text.c_str(), this);
    QLabel *timestampLabel = new QLabel(processTimestamp(QDateTime::fromString(QString((*message->timestamp).c_str()), Qt::ISODate).toLocalTime()), this);

    spacer->setFixedWidth(28);
    arrow->setFixedWidth(44);
    arrow->setPixmap(QPixmap(("res/images/svg/" + arrowName).c_str()));

    textLabel->setStyleSheet("color: #8E9297;");
    timestampLabel->setStyleSheet("color: #72767D;");
    textLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    textLabel->setCursor(QCursor(Qt::IBeamCursor));
    timestampLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);

    layout->addWidget(spacer);
    layout->addWidget(arrow);
    layout->addWidget(textLabel);
    layout->addWidget(timestampLabel);
    layout->addStretch();

    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);

    this->setMinimumHeight(26);
}

void MessageWidget::callMessage(Api::Message *message)
{
    rm->getClient([message, this](void *clientPtr){

    std::string phoneName;
    std::string text;
    std::string clientId = *reinterpret_cast<Api::Client *>(clientPtr)->id;
    QDateTime dateTime = QDateTime::fromString(QString((*message->timestamp).c_str()), Qt::ISODate).toLocalTime();

    std::vector<std::string> participants = *message->call->participants;
    bool participated = false;
    for (unsigned int i = 0 ; i < participants.size() ; i++) {
        if (participants[i] == clientId) participated = true;
    }

    if (!participated) {
        text = "You missed a call from " + *message->author->username;
    } else {
        text = *message->author->username + " started a call";
    }
    std::string *endedTimestampStr = message->call->endedTimestamp;
    if (endedTimestampStr != nullptr && *endedTimestampStr != "") {
        phoneName = "gray-phone.svg";
        qint64 duration = QDateTime::fromString(endedTimestampStr->c_str(), Qt::ISODate)
                .toLocalTime().toSecsSinceEpoch() -
                dateTime.toLocalTime().toSecsSinceEpoch();
        if (duration < 60) {
            text += " that lasted a few seconds";
        } else if (duration < 3600) {
            text += " that lasted ";
            if (duration < 120)
                text += "a minute";
            else
                text += std::to_string(duration / 60) + " minutes";
        } else {
            text += " that lasted ";
            if (duration < 7200)
                text += "a hour";
            else
                text += std::to_string(duration / 3600) + " hours";
        }
    } else {
        phoneName = "green-phone.svg";
    }
    text += ".";

    if (clientId == *message->author->id)
        phoneName = "green-phone.svg";

    QHBoxLayout *layout = new QHBoxLayout(this);
    QWidget *spacer = new QWidget(this);
    QLabel *phone = new QLabel(this);
    QLabel *textLabel = new QLabel(text.c_str(), this);
    QLabel *timestampLabel = new QLabel(processTimestamp(dateTime), this);

    spacer->setFixedWidth(28);
    phone->setFixedWidth(44);
    phone->setPixmap(QPixmap(("res/images/svg/" + phoneName).c_str()));

    textLabel->setStyleSheet("color: #8E9297;");
    timestampLabel->setStyleSheet("color: #72767D;");
    textLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    textLabel->setCursor(QCursor(Qt::IBeamCursor));
    timestampLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);

    layout->addWidget(spacer);
    layout->addWidget(phone);
    layout->addWidget(textLabel);
    layout->addWidget(timestampLabel);
    layout->addStretch();

    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);

    this->setMinimumHeight(26);

    });
}

} // namespace Ui
