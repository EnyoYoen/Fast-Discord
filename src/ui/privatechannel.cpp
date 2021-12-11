#include "ui/privatechannel.h"

#include <QPixmap>
#include <QBitmap>
#include <QPainter>
#include <QPainterPath>
#include <QString>
#include <QGridLayout>

#include <fstream>
#include <string>

namespace Ui {

PrivateChannel::PrivateChannel(Api::Requester *requesterp, const Api::Channel& privateChannel, unsigned int idp, QWidget *parent)
    : QWidget(parent)
{
    // Attributes initialization
    requester = requesterp;
    id = idp;
    channel = const_cast<Api::Channel *>(&privateChannel);

    // I have to create an other widget otherwise
    // the background color is not applied everywhere, I don't know why
    QWidget *container = new QWidget(this);

    // Variables initialization
    std::string channelIconFileName;
    int channelType = privateChannel.type;

    if (channelType == Api::DM) {
        subtext = new QLabel("", container);

        // Get the icon of the channel
        Api::User dmUser = *(*privateChannel.recipients)[0];
        std::string *avatar = dmUser.avatar;
        if (avatar == nullptr) {
            // Use an asset if the other user doesn't have an icon
            channelIconFileName = "res/images/png/user-icon-asset0.png";

            // Create the icon
            icon = new RoundedImage(channelIconFileName, 32, 32, 16, container);
        } else {
            // Request the icon
            channelIconFileName = *dmUser.id + (avatar->rfind("a_") == 0 ? ".gif" : ".webp");
            if (!std::ifstream(("cache/" + channelIconFileName).c_str()).good()) {
                requester->getImage([this](void *iconFileName) {this->setIcon(*static_cast<std::string *>(iconFileName));}, "https://cdn.discordapp.com/avatars/" + *dmUser.id + "/" + *avatar, channelIconFileName);

                // Create the icon
                icon = new RoundedImage(32, 32, 16, container);
            } else {
                channelIconFileName = "cache/" + channelIconFileName;

                // Create the icon
                icon = new RoundedImage(channelIconFileName, 32, 32, 16, container);
            }
        }

        // Set the DM name to the other user name
        name = new QLabel((*dmUser.username).c_str(), container);
    } else if (channelType == Api::GroupDM) {
        // Get the subtext of the group DM with the number of people in it
        std::vector<Api::User *> recipients = *privateChannel.recipients;
        int n_member = recipients.size();
        std::string str_member = " member";
        // Plural
        if (n_member+1 > 1) {
            str_member += "s";
        }
        subtext = new QLabel((std::to_string(n_member+1) + str_member).c_str(), container);

        // Get the name of the group
        std::string *channelName = privateChannel.name;
        if (channelName == nullptr) {
            // There is no name
            if (recipients.size() == 1) {
                // Get the name of the other user if there is only two person
                name = new QLabel((*recipients[0]->username).c_str());
            } else {
                // Set it with 'Unnamed'
                name = new QLabel("Unnamed");
            }
        } else {
            name = new QLabel(channelName->c_str());
        }

        // Set the icon of the channel
        std::string *channelIcon = privateChannel.icon;
        if (channelIcon == nullptr) {
            // Use an asset if there is none
            channelIconFileName = "res/images/png/group-icon-asset1.png";

            // Create the icon
            icon = new RoundedImage(channelIconFileName, 32, 32, 16, container);
        } else {
            // Request the icon
            channelIconFileName = *channelIcon + ".png";
            if (!std::ifstream(("cache/" + channelIconFileName).c_str()).good()) {
                requester->getImage([this](void *iconFileName) {this->setIcon(*static_cast<std::string *>(iconFileName));}, "https://cdn.discordapp.com/channel-icons/" + *privateChannel.id + "/" + channelIconFileName, channelIconFileName);

                // Create the icon
                icon = new RoundedImage(32, 32, 16, container);
            } else {
                channelIconFileName = "cache/" + channelIconFileName;

                // Create the icon
                icon = new RoundedImage(channelIconFileName, 32, 32, 16, container);
            }
        }
    }

    // Style the widgets
    if (name) name->setFixedSize(156, 14);
    subtext->setFixedSize(156, 14);

    // Create and style the layout, and add widgets to it
    QGridLayout *layout = new QGridLayout(container);
    layout->addWidget(icon, 0, 0, 2, 2);
    layout->addWidget(name, 0, 1);
    layout->addWidget(subtext, 1, 1);
    layout->setContentsMargins(8, 0, 8, 0);
    layout->setSpacing(0);

    // Create the main layout and add the container
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->addWidget(container);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    // Style this widget
    this->setFixedSize(224, 44);
    this->setStyleSheet("border-radius: 4px;"
                        "color: #8E9297;");
}

void PrivateChannel::setIcon(const std::string& guildIconFileName)
{
    icon->setImage(guildIconFileName);
}

void PrivateChannel::unclicked()
{
    // Reset the stylesheet of this widget if currently clicked
    if (clicked) {
        clicked = false;
        this->setStyleSheet("background-color: none;"
                            "border-radius: 4px;"
                            "color: #8E9297;");
    }
}

void PrivateChannel::mouseReleaseEvent(QMouseEvent *event)
{
    // Emit signals when clicked to open the channel or to show infos
    if (event->button() == Qt::LeftButton) {
        emit leftClicked(*channel, id);
    } else if (event->button() == Qt::RightButton) {
        emit rightClicked(*channel);
    }
}

void PrivateChannel::mousePressEvent(QMouseEvent *)
{
    // Widget clicked : change the stylesheet
    if (!clicked) {
        this->setStyleSheet("color: #FFF;"
                            "border-radius: 4px;"
                            "background-color: #393D43");
        clicked = true;
    }
}

void PrivateChannel::enterEvent(QEvent *)
{
    // Mouse hover : change the stylesheet
    if (!clicked) {
        this->setStyleSheet("color: #DCDDDE;"
                            "border-radius: 4px;"
                            "background-color: #35373D");
    }
}

void PrivateChannel::leaveEvent(QEvent *)
{
    // Reset the stylesheet if not clicked
    if (!clicked) {
        this->setStyleSheet("background-color: none;"
                            "border-radius: 4px;"
                            "color: #8E9297;");
    }
}

} // namespace Ui
