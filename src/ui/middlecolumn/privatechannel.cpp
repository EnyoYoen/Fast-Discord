#include "ui/middlecolumn/privatechannel.h"

#include <QPixmap>
#include <QBitmap>
#include <QPainter>
#include <QPainterPath>
#include <QString>

namespace Ui {

PrivateChannelWidget::PrivateChannelWidget(Api::RessourceManager *rmp, const Api::PrivateChannel& privateChannel, QWidget *parent)
    : Widget(parent)
{
    // Attributes initialization
    rm = rmp;
    id = privateChannel.id;
    statusIcon = nullptr;
    statusBackground = nullptr;
    userCounter = 0;
    clicked = false;

    // Variables initialization
    QString channelIconFileName;
    int channelType = privateChannel.type;

    if (channelType == Api::DM) {
        rm->getUser([&](void *user){
            subtext = new Label(this);
            subtext->move(Settings::scale(52), Settings::scale(24));
            QFont font;
            font.setPixelSize(Settings::scale(13));
            font.setFamily("whitney");
            subtext->setFont(font);
            subtext->setTextColor(Settings::ChannelsDefault);

            Api::User *dmUser = static_cast<Api::User *>(user);
            QString avatar = dmUser->avatar;
            if (avatar.isNull()) {
                // Use an asset if the other user doesn't have an icon
                channelIconFileName = "res/images/png/user-icon-asset0.png";

                // Create the icon
                icon = new RoundedImage(channelIconFileName, Settings::scale(32), Settings::scale(32), Settings::scale(16), this);
                icon->move(Settings::scale(8), Settings::scale(6));
            } else {
                // Create the icon
                icon = new RoundedImage(Settings::scale(32), Settings::scale(32), Settings::scale(16), this);

                // Request the icon image
                channelIconFileName = dmUser->id + (avatar.indexOf("a_") == 0 ? ".gif" : ".png");
                rm->getImage([this](void *iconFileName) {this->setIcon(*static_cast<QString *>(iconFileName));}, "https://cdn.discordapp.com/avatars/" + dmUser->id + "/" + avatar, channelIconFileName);
            }

            // Set the background of the status icon
            statusBackground = new Widget(this);
            statusBackground->setFixedSize(Settings::scale(16), Settings::scale(16));
            statusBackground->move(Settings::scale(28), Settings::scale(24));
            statusBackground->setBorderRadius(Settings::scale(8));
            statusBackground->setBackgroundColor(Settings::BackgroundSecondary);

            // Set the status icon
            statusIcon = new StatusIcon(this);
            statusIcon->move(Settings::scale(31), Settings::scale(27));

            // Set the DM name to the other user name
            name = new Label(dmUser->username, this);
            font.setPixelSize(Settings::scale(15));
            name->setFont(font);
            name->setTextColor(Settings::ChannelsDefault);
            name->move(Settings::scale(52), (subtext->text.isEmpty() ? Settings::scale(16) : Settings::scale(8)));

            // Style the widgets
            if (name) name->setFixedSize(Settings::scale(145), Settings::scale(20));
            subtext->setFixedSize(Settings::scale(145), 0);

            closeButton = new CloseChannelButton(this);
            closeButton->hide();
            closeButton->move(Settings::scale(200), Settings::scale(3));
            QObject::connect(closeButton, &CloseChannelButton::clicked, this, &PrivateChannelWidget::closeChannel);

            // Create the main layout
            mainLayout = new QHBoxLayout(this);
            mainLayout->setContentsMargins(0, 0, 0, 0);

            // Style this widget
            this->setFixedSize(Settings::scale(224), Settings::scale(44));
            this->setBorderRadius(Settings::scale(4));
        }, privateChannel.recipientIds[0]);
    } else if (channelType == Api::GroupDM) {
        // Get the subtext of the group DM with the number of people in it
        nMembers = privateChannel.recipientIds.size();
        QString str_member = " Member";
        // Plural
        if (nMembers + 1 > 1) {
            str_member += "s";
        }
        subtext = new Label(QString::number(nMembers+1) + str_member, this);
        subtext->move(Settings::scale(52), Settings::scale(24));
        QFont font;
        font.setPixelSize(Settings::scale(12));
        font.setFamily("whitney");
        subtext->setFont(font);
        subtext->setTextColor(Settings::ChannelsDefault);
        font.setPixelSize(Settings::scale(15));

        // Get the name of the group
        QString channelName = privateChannel.name;
        if (channelName.isNull()) {
            // There is no name
            if (nMembers == 0) {
                // Set it with 'Unnamed'
                name = new Label("Unnamed", this);
                name->setFont(font);
                name->setFixedSize(Settings::scale(145), Settings::scale(20));
                name->move(Settings::scale(52), Settings::scale(8));
                name->setTextColor(Settings::ChannelsDefault);
            } else {
                // Get the name of the other users if there is more than one person
                for (unsigned int i = 0 ; i < nMembers ; i++)
                    rm->getUser([this](void *u){this->userReceiver(u);}, privateChannel.recipientIds[i]);
            }
        } else {
            name = new Label(channelName, this);
            name->setFont(font);
            name->setFixedSize(Settings::scale(145), Settings::scale(20));
            name->move(Settings::scale(52), Settings::scale(8));
            name->setTextColor(Settings::ChannelsDefault);
        }

        // Set the icon of the channel
        QString channelIcon = privateChannel.icon;
        if (channelIcon.isNull()) {
            // Use an asset if there is none
            channelIconFileName = "res/images/png/group-icon-asset1.png";

            // Create the icon
            icon = new RoundedImage(channelIconFileName, Settings::scale(32), Settings::scale(32), Settings::scale(16), this);
            icon->move(Settings::scale(8), Settings::scale(6));
        } else {
            // Create the icon
            icon = new RoundedImage(Settings::scale(32), Settings::scale(32), Settings::scale(16), this);

            // Request the icon image
            channelIconFileName = channelIcon + ".png";
            rm->getImage([this](void *iconFileName) {this->setIcon(*static_cast<QString *>(iconFileName));}, "https://cdn.discordapp.com/channel-icons/" + privateChannel.id + "/" + channelIconFileName, channelIconFileName);
        }

        // Style the subtext
        subtext->setFixedSize(Settings::scale(145), Settings::scale(14));

        closeButton = new CloseChannelButton(this);
        closeButton->hide();
        closeButton->move(Settings::scale(200), Settings::scale(3));
        QObject::connect(closeButton, &CloseChannelButton::clicked, this, &PrivateChannelWidget::closeChannel);

        // Create the main layout
        mainLayout = new QHBoxLayout(this);
        mainLayout->setContentsMargins(0, 0, 0, 0);

        // Style this widget
        this->setFixedSize(Settings::scale(224), Settings::scale(44));
        this->setBorderRadius(Settings::scale(4));
    }
}

void PrivateChannelWidget::setStatus(const QString& status)
{
    statusIcon->setStatus(status);
}

void PrivateChannelWidget::setIcon(const QString& iconFileName)
{
    icon->setImage(iconFileName);
    icon->move(Settings::scale(8), Settings::scale(6));
}

void PrivateChannelWidget::userReceiver(void *user)
{
    users.append(reinterpret_cast<Api::User *>(user));
    if (++userCounter == nMembers) {
        QString nameStr;
        for (int i = 0 ; i < users.size() ; ) {
            nameStr += users[i]->username;
            if (++i != users.size()) {
                nameStr += ", ";
            }
        }
        QFont font;
        font.setPixelSize(Settings::scale(16));
        font.setFamily("whitney");
        name = new Label(nameStr, this);
        name->setFont(font);
        name->setFixedSize(Settings::scale(145), Settings::scale(20));
        name->move(Settings::scale(52), Settings::scale(8));
        name->setTextColor(Settings::ChannelsDefault);
    }
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
            statusBackground->setBackgroundColor(Settings::BackgroundSecondary);
        }
        this->setBackgroundColor(Settings::None);
        name->setTextColor(Settings::ChannelsDefault);
        subtext->setTextColor(Settings::ChannelsDefault);
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
            statusBackground->setBackgroundColor(Settings::StatusBackgroundActive);
        }
        this->setBackgroundColor(Settings::BackgroundModifierActive);
        name->setTextColor(Settings::InteractiveActive);
        subtext->setTextColor(Settings::InteractiveActive);
        clicked = true;
    }
}

void PrivateChannelWidget::enterEvent(QEvent *)
{
    // Mouse hover : change the stylesheet
    closeButton->show();
    if (!clicked) {
        if (statusBackground != nullptr) {
            statusBackground->setBackgroundColor(Settings::StatusBackgroundHover);
        }
        this->setBackgroundColor(Settings::BackgroundModifierHover);
        name->setTextColor(Settings::InteractiveHover);
        subtext->setTextColor(Settings::InteractiveHover);
    }
}

void PrivateChannelWidget::leaveEvent(QEvent *)
{
    // Reset the stylesheet if not clicked
    closeButton->hide();
    if (!clicked) {
        if (statusBackground != nullptr) {
            statusBackground->setBackgroundColor(Settings::BackgroundSecondary);
        }
        this->setBackgroundColor(Settings::BackgroundSecondary);
        name->setTextColor(Settings::ChannelsDefault);
        subtext->setTextColor(Settings::ChannelsDefault);
    }
}

} // namespace Ui
