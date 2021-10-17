#include "../../include/ui/privatechannel.h"

#include "../../include/api/request.h"

#include <QPixmap>
#include <QBitmap>
#include <QPainter>
#include <QPainterPath>
#include <QString>

#include <fstream>
#include <string>

namespace Ui {

PrivateChannel::PrivateChannel(const Api::Channel& privateChannel, QWidget *parent) : QWidget(parent)
{
    channel = const_cast<Api::Channel *>(&privateChannel);

    //this->setProperty("private-channel", true);
    this->setFixedSize(224, 44);

    std::string channelIconFileName;
    int channelType = privateChannel.type;

    if (channelType == 1) {
        subtext = new QLabel("");

        Api::User dmUser = (*privateChannel.recipients)[0];
        std::string *avatar = dmUser.avatar;
        if (avatar == nullptr) {
            channelIconFileName = "res/images/png/user-icon-asset.png";
        } else {
            channelIconFileName = *dmUser.id + (avatar->rfind("a_") == 0 ? ".gif" : ".webp");
            if (!std::ifstream(("cache/" + channelIconFileName).c_str()).good()) {
                Api::Request::requestFile("https://cdn.discordapp.com/avatars/" + *dmUser.id + "/" + *avatar, "cache/" + channelIconFileName);
            }
            channelIconFileName = "cache/" + channelIconFileName;
        }
        name = new QLabel((*dmUser.username).c_str());
    } else if (channelType == 3) {
        std::vector<Api::User> recipients = *privateChannel.recipients;
        int n_member = recipients.size();
        std::string str_member = " member";

        if (n_member+1 > 1) {
            str_member += "s";
        }
        subtext = new QLabel((std::to_string(n_member+1) + str_member).c_str());

        std::string *channelName = privateChannel.name;
        if (channelName == nullptr) {
            if (recipients.size() == 1) {
                name = new QLabel((*recipients[0].username).c_str());
            } else {
                name = new QLabel("No name");
            }
        } else {
            name = new QLabel(channelName->c_str());
        }

        std::string *channelIcon = privateChannel.icon;
        if (channelIcon == nullptr) {
            channelIconFileName = "res/images/png/group-icon-asset1.png";
        } else {
            channelIconFileName = *channelIcon + ".png";
            if (!std::ifstream(("cache/" + channelIconFileName).c_str()).good()) {
                Api::Request::requestFile("https://cdn.discordapp.com/channel-icons/" + *privateChannel.id + "/" + channelIconFileName, "cache/" + channelIconFileName);
            }
            channelIconFileName = "cache/" + channelIconFileName;
        }
    }

    name->setFixedSize(156, 14);
    subtext->setFixedSize(156, 14);

    icon = new RoundedImage(channelIconFileName, 32, 32, 16);

    layout = new QGridLayout();
    layout->addWidget(icon, 0, 0, 2, 2);
    layout->addWidget(name, 0, 1);
    layout->addWidget(subtext, 1, 1);
    layout->setSpacing(0);

    this->setLayout(layout);

    this->setStyleSheet("border-radius: 4px;"
                        "color: #8E9297;");
}

void PrivateChannel::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        emit leftClicked(*channel);
    } else if (event->button() == Qt::RightButton) {
        emit rightClicked(*channel);
    }
}

} // namespace Ui
