#include "../../include/ui/messagewidget.h"

#include "../../include/api/request.h"
#include "../../include/api/jsonutils.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QDateTime>

#include <fstream>
#include <cstring>
#include <iostream>

namespace Ui {

MessageWidget::MessageWidget(const Api::Message& message, bool isFirstp, bool separatorBefore, QWidget *parent) : QWidget(parent)
{
    isFirst = isFirstp;
    setMinimumHeight(26);

    QHBoxLayout *mainLayout = new QHBoxLayout();
    QVBoxLayout *dataLayout = new QVBoxLayout();
    QHBoxLayout *iconLayout = new QHBoxLayout();
    QWidget *data = new QWidget();
    QWidget *iconContainer = new QWidget();

    QDateTime dateTime = QDateTime::fromString(QString((*message.timestamp).c_str()), Qt::ISODate).toLocalTime();
    int hour = dateTime.time().hour();
    int minute = dateTime.time().minute();
    std::string hourString = hour > 12 ? std::to_string(hour - 12) : std::to_string(hour);
    std::string minuteString = minute < 10 ? "0" + std::to_string(minute) : std::to_string(minute);
    QString messageTime = QString((hourString + ":" + minuteString + " ").c_str());
    hour / 12 == 1 ? messageTime += "PM" : messageTime += "AM";

    if (isFirst) {
        std::string avatarFileName;
        Api::User& author = *message.author;
        std::string *avatarId = author.avatar;
        if (avatarId == nullptr) {
            avatarFileName = "res/images/png/user-icon-asset.png";
        } else {
            avatarFileName = *author.id + (author.avatar->rfind("a_") == 0 ? ".gif" : ".webp");
            if (!std::ifstream(("cache/" + avatarFileName).c_str()).good()) {
                Api::Request::requestFile("https://cdn.discordapp.com/avatars/" + *author.id + "/" + avatarFileName, "cache/" + avatarFileName);
            }
            avatarFileName = "cache/" + avatarFileName;
        }
        iconLayout->addWidget(new RoundedImage(avatarFileName, 40, 40, 20));

        QWidget *messageInfos = new QWidget();
        QHBoxLayout *infosLayout = new QHBoxLayout();
        QLabel *name = new QLabel((*author.username).c_str());

        QLabel *date;
        QDate messageDate = dateTime.date();
        QDate currentDate = QDate::currentDate();
        if (messageDate != currentDate) {
            if (messageDate.year() == currentDate.year() && messageDate.month() == currentDate.month() && messageDate.day() == currentDate.day() - 1) {
                date = new QLabel("Yesterday at " + messageTime);
            } else {
                date = new QLabel(((messageDate.day() < 10 ? "0" + std::to_string(messageDate.day()) : std::to_string(messageDate.day())) + "/" + (messageDate.month() < 10 ? "0" + std::to_string(messageDate.month()) : std::to_string(messageDate.month())) + "/" + std::to_string(messageDate.year())).c_str());
            }
        } else {
            date = new QLabel("Today at " + messageTime);
        }

        name->setTextInteractionFlags(Qt::TextSelectableByMouse);
        name->setCursor(QCursor(Qt::IBeamCursor));
        name->setStyleSheet("color: #FFF");

        date->setTextInteractionFlags(Qt::TextSelectableByMouse);
        date->setStyleSheet("color: #72767D;");

        infosLayout->addWidget(name);
        infosLayout->addWidget(date);
        infosLayout->insertStretch(-1);
        infosLayout->setContentsMargins(0, 0, 0, 0);

        messageInfos->setLayout(infosLayout);
        dataLayout->addWidget(messageInfos);
        dataLayout->setSpacing(0);
        setContentsMargins(0, separatorBefore ? 0 : 20, 0, 0);
    } else {
        hoveredTimestamp = messageTime;
        timestampLabel = new QLabel();
        timestampLabel->setFixedHeight(22);
        iconLayout->addWidget(timestampLabel);
        iconLayout->setContentsMargins(0, 0, 0, 0);
        mainLayout->setContentsMargins(0, 0, 0, 0);
    }

    iconContainer->setFixedWidth(72);
    iconContainer->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Minimum);
    iconContainer->setStyleSheet("color: #72767D;");
    iconContainer->setLayout(iconLayout);

    QLabel *content = new QLabel((*message.content).c_str());
    content->setTextInteractionFlags(Qt::TextSelectableByMouse);
    content->setCursor(QCursor(Qt::IBeamCursor));
    content->setWordWrap(true);
    content->setStyleSheet("color: #DCDDDE");

    dataLayout->setContentsMargins(0, 0, 0, 0);
    dataLayout->addWidget(content);

    data->setLayout(dataLayout);

    mainLayout->addWidget(iconContainer);
    mainLayout->addWidget(data);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);


    setStyleSheet("padding: 0px;"
                  "margin: 0px;");
    setLayout(mainLayout);
}

void MessageWidget::enterEvent(QEvent *)
{
    setStyleSheet("background-color: #32353B;");
    if (!isFirst) timestampLabel->setText(" " + hoveredTimestamp);
}

void MessageWidget::leaveEvent(QEvent *)
{
    setStyleSheet("background-color: none;");
    if (!isFirst) timestampLabel->setText("");
}

} // namespace Ui
