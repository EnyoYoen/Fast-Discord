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

    // Create the main widgets
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    QWidget *data = new QWidget(this);
    data->setStyleSheet("background-color: white;");
    dataLayout = new QVBoxLayout(data);
    QWidget *iconContainer = new QWidget(this);
    QVBoxLayout *iconLayout = new QVBoxLayout(iconContainer);

    // Process the timestamp to a nicer format

    // Get the date and time of the message
    QDateTime dateTime = QDateTime::fromString(QString((*message->timestamp).c_str()), Qt::ISODate).toLocalTime();
    // Get the hours and minutes
    int hour = dateTime.time().hour();
    int minute = dateTime.time().minute();
    // Process the hours and minutes
    std::string hourString = hour > 12 ? std::to_string(hour - 12) : std::to_string(hour);
    std::string minuteString = minute < 10 ? "0" + std::to_string(minute) : std::to_string(minute);
    // Create the final timestamp format
    QString messageTime = QString((hourString + ":" + minuteString + " ").c_str());
    hour / 12 == 1 ? messageTime += "PM" : messageTime += "AM";

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
        QLabel *date;

        // Get the date of the message and the current date to compare them
        QDate messageDate = dateTime.date();
        QDate currentDate = QDate::currentDate();
        if (messageDate != currentDate) {
            // The message was not send today
            if (messageDate.year() == currentDate.year() && messageDate.month() == currentDate.month() && messageDate.day() == currentDate.day() - 1) {
                // The message was send yesterday
                date = new QLabel("Yesterday at " + messageTime, messageInfos);
            } else {
                // The message is older
                date = new QLabel(((messageDate.day() < 10 ? "0" + std::to_string(messageDate.day()) : std::to_string(messageDate.day())) + "/" + (messageDate.month() < 10 ? "0" + std::to_string(messageDate.month()) : std::to_string(messageDate.month())) + "/" + std::to_string(messageDate.year())).c_str(), messageInfos);
            }
        } else {
            date = new QLabel("Today at " + messageTime);
        }

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
        hoveredTimestamp = messageTime;
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
    content = new MarkdownLabel(*message->content, rm, this);
    content->setStyleSheet("background-color: none;");

    // Style the data layout
    dataLayout->setContentsMargins(0, 0, 0, 0);
    dataLayout->addWidget(content);

    if (message->attachments != nullptr) {
        std::vector<Api::Attachment *> attachments = *message->attachments;
        for (unsigned int i = 0 ; i < attachments.size() ; i++) {
            Api::Attachment *attachment = attachments[i];
            if (attachments[i]->contentType != nullptr
              && attachments[i]->contentType->find("image") != std::string::npos
              && attachments[i]->contentType->find("svg") == std::string::npos) {
                rm->getImage([this, attachment](void *filename) {
                    this->addImage(*reinterpret_cast<std::string *>(filename), attachment->width, attachment->height);
                }, *attachments[i]->proxyUrl, *attachments[i]->filename);
            } else {
                dataLayout->addWidget(new AttachmentFile(rm, attachment, data));
            }
        }
    }

    // Add widgets to the main layout and style it
    mainLayout->addWidget(iconContainer);
    mainLayout->addWidget(data);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);

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
    imageLabel->setPixmap(pixmap.scaled(width, height, Qt::KeepAspectRatio));
    dataLayout->addWidget(imageLabel);
}

void MessageWidget::enterEvent(QEvent *)
{
    // Mouse hover : change the stylesheet and show the timestamp label
    setStyleSheet("background-color: #32353B;");
    content->setStyleSheet("background-color: #32353B;"
                           "color: #DCDDDE;");
    if (!isFirst) timestampLabel->setText(" " + hoveredTimestamp);
}

void MessageWidget::leaveEvent(QEvent *)
{
    // Reset the stylesheet and hide the timestamp label
    setStyleSheet("background-color: none;");
    content->setStyleSheet("background-color: #36393F;"
                           "color: #DCDDDE;");
    if (!isFirst) timestampLabel->setText("");
}

} // namespace Ui
