#include "ui/privatechannel.h"

#include "ui/closechannelbutton.h"

#include <QPixmap>
#include <QBitmap>
#include <QPainter>
#include <QPainterPath>
#include <QString>
#include <QGridLayout>

namespace Ui {

PrivateChannelWidget::PrivateChannelWidget(Api::RessourceManager *rmp, const Api::PrivateChannel& privateChannel, QWidget *parent)
    : QWidget(parent)
{
    // Attributes initialization
    rm = rmp;
    id = privateChannel.id;
    statusIcon = nullptr;
    statusBackground = nullptr;

    // I have to create an other widget otherwise
    // the background color is not applied everywhere, I don't know why
    QWidget *container = new QWidget(this);

    // Create and style the layout
    QGridLayout *layout = new QGridLayout(container);
    layout->setContentsMargins(8, 0, 8, 0);
    layout->setHorizontalSpacing(4);
    layout->setVerticalSpacing(4);

    // Variables initialization
    QString channelIconFileName;
    int channelType = privateChannel.type;

    if (channelType == Api::DM) {
        rm->getUser([&](void *user){
            subtext = new QLabel("", container);
            QFont font;
            font.setPixelSize(12);
            font.setFamily("whitney");
            subtext->setFont(font);

            Api::User *dmUser = static_cast<Api::User *>(user);
            QString avatar = dmUser->avatar;
            if (avatar.isNull()) {
                // Use an asset if the other user doesn't have an icon
                channelIconFileName = "res/images/png/user-icon-asset0.png";

                // Create the icon
                icon = new RoundedImage(channelIconFileName, 32, 32, 16, container);
            } else {
                // Create the icon
                icon = new RoundedImage(32, 32, 16, container);

                // Request the icon image
                channelIconFileName = dmUser->id + (avatar.indexOf("a_") == 0 ? ".gif" : ".png");
                rm->getImage([this](void *iconFileName) {this->setIcon(*static_cast<QString *>(iconFileName));}, "https://cdn.discordapp.com/avatars/" + dmUser->id + "/" + avatar, channelIconFileName);
            }

            // Set the background of the status icon
            statusBackground = new QLabel(this);
            statusBackground->setFixedSize(16, 16);
            statusBackground->move(28, 24);
            statusBackground->setStyleSheet("border-radius: 8px;"
                                            "background-color: #2F3136;");

            // Set the status icon
            statusIcon = new StatusIcon(this);
            statusIcon->move(31, 27);

            // Set the DM name to the other user name
            name = new QLabel(dmUser->username, container);
            font.setPixelSize(16);
            name->setFont(font);

            // Style the widgets
            if (name) name->setFixedSize(145, 14);
            subtext->setFixedSize(145, 14);

            CloseChannelButton *closeButton = new CloseChannelButton(container);
            QObject::connect(closeButton, &CloseChannelButton::clicked, this, &PrivateChannelWidget::closeChannel);

            // Add widgets to the layout
            layout->addWidget(icon, 0, 0, 2, 1);
            layout->addWidget(name, 0, 1);
            layout->addWidget(subtext, 1, 1);
            layout->addWidget(closeButton, 0, 2, 2, 1);

            // Create the main layout and add the container
            QHBoxLayout *mainLayout = new QHBoxLayout(this);
            mainLayout->addWidget(container);
            mainLayout->setContentsMargins(0, 0, 0, 0);

            // Style this widget
            this->setFixedSize(224, 44);
            this->setStyleSheet("border-radius: 4px;"
                                "color: #8E9297;");
        }, privateChannel.recipientIds[0]);
    } else if (channelType == Api::GroupDM) {
        // Get the subtext of the group DM with the number of people in it
        size_t nMember = privateChannel.recipientIds.size();
        QString str_member = " member";
        // Plural
        if (nMember + 1 > 1) {
            str_member += "s";
        }
        subtext = new QLabel(QString::number(nMember+1) + str_member, container);
        QFont font;
        font.setPixelSize(12);
        font.setFamily("whitney");
        subtext->setFont(font);
        font.setPixelSize(16);

        // Get the name of the group
        QString channelName = privateChannel.name;
        if (channelName.isNull()) {
            // There is no name
            if (nMember == 1) {
                // Get the name of the other user if there is only two person
                rm->getUser([&](void *user){
                    name = new QLabel(static_cast<Api::User *>(user)->username);
                    name->setFont(font);
                    name->setFixedSize(145, 14);
                    layout->addWidget(name, 0, 1);
                }, privateChannel.recipientIds[0]);
            } else {
                // Set it with 'Unnamed'
                name = new QLabel("Unnamed");
                name->setFont(font);
                name->setFixedSize(145, 14);
                layout->addWidget(name, 0, 1);
            }
        } else {
            name = new QLabel(channelName);
            name->setFont(font);
            name->setFixedSize(145, 14);
            layout->addWidget(name, 0, 1);
        }

        // Set the icon of the channel
        QString channelIcon = privateChannel.icon;
        if (channelIcon.isNull()) {
            // Use an asset if there is none
            channelIconFileName = "res/images/png/group-icon-asset1.png";

            // Create the icon
            icon = new RoundedImage(channelIconFileName, 32, 32, 16, container);
        } else {
            // Create the icon
            icon = new RoundedImage(32, 32, 16, container);

            // Request the icon image
            channelIconFileName = channelIcon + ".png";
            rm->getImage([this](void *iconFileName) {this->setIcon(*static_cast<QString *>(iconFileName));}, "https://cdn.discordapp.com/channel-icons/" + privateChannel.id + "/" + channelIconFileName, channelIconFileName);
        }

        // Style the subtext
        subtext->setFixedSize(145, 14);

        CloseChannelButton *closeButton = new CloseChannelButton(container);
        QObject::connect(closeButton, &CloseChannelButton::clicked, this, &PrivateChannelWidget::closeChannel);

        // Add widgets to the layout
        layout->addWidget(icon, 0, 0, 2, 1);
        layout->addWidget(subtext, 1, 1);
        layout->addWidget(closeButton, 0, 2, 2, 2);

        // Create the main layout and add the container
        QHBoxLayout *mainLayout = new QHBoxLayout(this);
        mainLayout->addWidget(container);
        mainLayout->setContentsMargins(0, 0, 0, 0);

        // Style this widget
        this->setFixedSize(224, 44);
        this->setStyleSheet("border-radius: 4px;"
                            "color: #8E9297;");
    }
}

void PrivateChannelWidget::setStatus(const QString& status)
{
    statusIcon->setStatus(status);
}

void PrivateChannelWidget::setIcon(const QString& iconFileName)
{
    icon->setImage(iconFileName);
}

void PrivateChannelWidget::closeChannel()
{
    emit closeButtonClicked(id, 0, Api::DM);
}

void PrivateChannelWidget::unclicked()
{
    // Reset the stylesheet of this widget if currently clicked
    if (clicked) {
        if (statusBackground != nullptr) {
            statusBackground->setStyleSheet("border-radius: 8px;"
                                            "background-color: #2F3136;");
        }
        this->setStyleSheet("background-color: none;"
                            "border-radius: 4px;"
                            "color: #8E9297;");
        clicked = false;
    }
}

void PrivateChannelWidget::mouseReleaseEvent(QMouseEvent *event)
{
    // Emit signals when clicked to open the channel or to show infos
    if (event->button() == Qt::LeftButton) {
        emit leftClicked(id);
    } else if (event->button() == Qt::RightButton) {
        emit rightClicked(id);
    }
}

void PrivateChannelWidget::mousePressEvent(QMouseEvent *)
{
    // Widget clicked : change the stylesheet
    if (!clicked) {
        if (statusBackground != nullptr) {
            statusBackground->setStyleSheet("border-radius: 8px;"
                                            "background-color: #393D43;");
        }
        this->setStyleSheet("color: #FFF;"
                            "border-radius: 4px;"
                            "background-color: #393D43");
        clicked = true;
    }
}

void PrivateChannelWidget::enterEvent(QEvent *)
{
    // Mouse hover : change the stylesheet
    if (!clicked) {
        if (statusBackground != nullptr) {
            statusBackground->setStyleSheet("border-radius: 8px;"
                                            "background-color: #35373D;");
        }
        this->setStyleSheet("color: #DCDDDE;"
                            "border-radius: 4px;"
                            "background-color: #35373D");
    }
}

void PrivateChannelWidget::leaveEvent(QEvent *)
{
    // Reset the stylesheet if not clicked
    if (!clicked) {
        if (statusBackground != nullptr) {
            statusBackground->setStyleSheet("border-radius: 8px;"
                                            "background-color: #2F3136;");
        }
        this->setStyleSheet("background-color: none;"
                            "border-radius: 4px;"
                            "color: #8E9297;");
    }
}

} // namespace Ui
