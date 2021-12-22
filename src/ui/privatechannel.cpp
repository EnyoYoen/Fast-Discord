#include "ui/privatechannel.h"

#include <QPixmap>
#include <QBitmap>
#include <QPainter>
#include <QPainterPath>
#include <QString>
#include <QGridLayout>

#include <fstream>
#include <string>
#include <vector>
#include <iostream>

namespace Ui {

PrivateChannelWidget::PrivateChannelWidget(Api::RessourceManager *rmp, const Api::PrivateChannel& privateChannel, QWidget *parent)
    : QWidget(parent)
{
    // Attributes initialization
    rm = rmp;
    id = *privateChannel.id;

    // I have to create an other widget otherwise
    // the background color is not applied everywhere, I don't know why
    QWidget *container = new QWidget(this);

    // Create and style the layout
    QGridLayout *layout = new QGridLayout(container);
    layout->setContentsMargins(8, 0, 8, 0);
    layout->setSpacing(0);

    // Variables initialization
    std::string channelIconFileName;
    int channelType = privateChannel.type;

    if (channelType == Api::DM) {
        rm->getUser([&](void *user){
            subtext = new QLabel("", container);

            Api::User *dmUser = static_cast<Api::User *>(user);
            std::string *avatar = dmUser->avatar;
            if (avatar == nullptr || *avatar == "") {
                // Use an asset if the other user doesn't have an icon
                channelIconFileName = "res/images/png/user-icon-asset0.png";

                // Create the icon
                icon = new RoundedImage(channelIconFileName, 32, 32, 16, container);
            } else {
                // Create the icon
                icon = new RoundedImage(32, 32, 16, container);

                // Request the icon image
                channelIconFileName = *dmUser->id + (avatar->rfind("a_") == 0 ? ".gif" : ".webp");
                rm->getImage([this](void *iconFileName) {this->setIcon(*static_cast<std::string *>(iconFileName));}, "https://cdn.discordapp.com/avatars/" + *dmUser->id + "/" + *avatar, channelIconFileName);
            }

            // Set the background of the status icon
            QLabel *statusBackground = new QLabel(this);
            statusBackground->setFixedSize(16, 16);
            statusBackground->move(28, 24);
            statusBackground->setStyleSheet("border-radius: 8px;"
                                            "background-color: #2F3136;");

            // Set the status icon
            statusIcon = new QLabel(this);
            statusIcon->setFixedSize(10, 10);
            statusIcon->move(31, 27);
            statusIcon->setStyleSheet(QString(("border-radius: 5px;"
                                      "background-image: url(res/images/svg/" + status + "-icon.svg);"
                                      "background-repeat: no-repeat;"
                                      "background-position: center;").c_str()));

            // Set the DM name to the other user name
            name = new QLabel((*dmUser->username).c_str(), container);

            // Style the widgets
            if (name) name->setFixedSize(156, 14);
            subtext->setFixedSize(156, 14);

            // Add widgets to the layout
            layout->addWidget(icon, 0, 0, 2, 2);
            layout->addWidget(name, 0, 1);
            layout->addWidget(subtext, 1, 1);

            // Create the main layout and add the container
            QHBoxLayout *mainLayout = new QHBoxLayout(this);
            mainLayout->addWidget(container);
            mainLayout->setContentsMargins(0, 0, 0, 0);

            // Style this widget
            this->setFixedSize(224, 44);
            this->setStyleSheet("border-radius: 4px;"
                                "color: #8E9297;");
        }, (*privateChannel.recipientIds)[0]);
    } else if (channelType == Api::GroupDM) {
        // Get the subtext of the group DM with the number of people in it
        int nMember = privateChannel.recipientIds->size();
        std::string str_member = " member";
        // Plural
        if (nMember + 1 > 1) {
            str_member += "s";
        }
        subtext = new QLabel((std::to_string(nMember+1) + str_member).c_str(), container);

        // Get the name of the group
        std::string *channelName = privateChannel.name;
        if (channelName == nullptr || *channelName == "") {
            // There is no name
            if (nMember == 1) {
                // Get the name of the other user if there is only two person
                rm->getUser([&](void *user){
                    name = new QLabel((*static_cast<Api::User *>(user)->username).c_str());
                    name->setFixedSize(156, 14);
                    layout->addWidget(name, 0, 1);
                }, (*privateChannel.recipientIds)[0]);
            } else {
                // Set it with 'Unnamed'
                name = new QLabel("Unnamed");
                name->setFixedSize(156, 14);
                layout->addWidget(name, 0, 1);
            }
        } else {
            name = new QLabel(channelName->c_str());
            name->setFixedSize(156, 14);
            layout->addWidget(name, 0, 1);
        }

        // Set the icon of the channel
        std::string *channelIcon = privateChannel.icon;
        if (channelIcon == nullptr || *channelIcon == "") {
            // Use an asset if there is none
            channelIconFileName = "res/images/png/group-icon-asset1.png";

            // Create the icon
            icon = new RoundedImage(channelIconFileName, 32, 32, 16, container);
        } else {
            // Create the icon
            icon = new RoundedImage(32, 32, 16, container);

            // Request the icon image
            channelIconFileName = *channelIcon + ".png";
            rm->getImage([this](void *iconFileName) {this->setIcon(*static_cast<std::string *>(iconFileName));}, "https://cdn.discordapp.com/channel-icons/" + *privateChannel.id + "/" + channelIconFileName, channelIconFileName);
        }

        // Style the subtext
        subtext->setFixedSize(156, 14);

        // Add widgets to the layout
        layout->addWidget(icon, 0, 0, 2, 2);
        layout->addWidget(subtext, 1, 1);

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

void PrivateChannelWidget::setStatus(std::string *statusp)
{
    if (statusp != nullptr) {
        if (statusIcon == nullptr) {
            status = *statusp;
        } else {
            statusIcon->setStyleSheet(QString(("border-radius: 5px;"
                                      "background-image: url(res/images/svg/" + *statusp + "-icon.svg);"
                                      "background-repeat: no-repeat;"
                                      "background-position: center;").c_str()));
        }
    }
}

void PrivateChannelWidget::setIcon(const std::string& guildIconFileName)
{
    icon->setImage(guildIconFileName);
}

void PrivateChannelWidget::unclicked()
{
    // Reset the stylesheet of this widget if currently clicked
    if (clicked) {
        clicked = false;
        this->setStyleSheet("background-color: none;"
                            "border-radius: 4px;"
                            "color: #8E9297;");
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
        this->setStyleSheet("color: #DCDDDE;"
                            "border-radius: 4px;"
                            "background-color: #35373D");
    }
}

void PrivateChannelWidget::leaveEvent(QEvent *)
{
    // Reset the stylesheet if not clicked
    if (!clicked) {
        this->setStyleSheet("background-color: none;"
                            "border-radius: 4px;"
                            "color: #8E9297;");
    }
}

} // namespace Ui
