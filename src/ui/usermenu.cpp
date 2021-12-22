#include "ui/usermenu.h"

#include "api/request.h"

#include <QHBoxLayout>

#include <fstream>

namespace Ui {

UserMenu::UserMenu(Api::RessourceManager *rmp, const Api::Client *client, QWidget *parent)
    : QWidget(parent)
{
    // Set the ressource manager
    rm = rmp;

    // Create the layout
    QHBoxLayout *mainLayout = new QHBoxLayout(this);

    // I have to create an other widget otherwise
    // the background color is not applied everywhere, I don't know why
    QWidget *container = new QWidget(this);
    QHBoxLayout *layout = new QHBoxLayout(container);
    this->setAttribute(Qt::WA_StyledBackground);

    // Get the icon of the actual user
    std::string channelIconFileName;
    if (client->avatar == nullptr || *client->avatar == "") {
        // Use an asset if the user doesn't have an icon
        channelIconFileName = "res/images/png/user-icon-asset0.png";

        avatar = new RoundedImage(channelIconFileName, 32, 32, 16, container);
    } else {
        avatar = new RoundedImage(32, 32, 16, container);

        // Request the icon
        channelIconFileName = *client->id + (client->avatar->rfind("a_") == 0 ? ".gif" : ".webp");
        rm->getImage([this](void *iconFileName) {this->setIcon(*static_cast<std::string *>(iconFileName));}, "https://cdn.discordapp.com/avatars/" + *client->id + "/" + *client->avatar, channelIconFileName);
    }

    // Create the widgets of the user menu
    QWidget *userInfos = new QWidget(container);
    QVBoxLayout *userInfosLayout = new QVBoxLayout(userInfos);
    QLabel *name = new QLabel((*client->username).c_str(), userInfos);

    // Style the name label
    name->setFixedSize(84, 18);
    name->setStyleSheet("color: #DCDDDE;");

    // Create and style the discriminator label
    QLabel *discriminator = new QLabel(("#" + *client->discriminator).c_str(), userInfos);
    discriminator->setFixedSize(84, 13);
    discriminator->setStyleSheet("color: #B9BBBE;");

    // Add the widgets and style the user infos layout
    userInfosLayout->addWidget(name);
    userInfosLayout->addWidget(discriminator);
    userInfosLayout->setContentsMargins(8, 10, 4, 10);

    // Add the icon and the infos of the user to the layout and style it
    layout->addWidget(avatar);
    layout->addWidget(userInfos);
    layout->setContentsMargins(8, 0, 8, 0);
    layout->setSpacing(0);
    layout->addStretch(100);

    // Add the buttons of the user menu
    muteButton = new UserMenuButton(Mute, container);
    UserMenuButton *deafenButton = new UserMenuButton(Deafen, container);
    UserMenuButton *settingsButton = new UserMenuButton(Settings, container);
    layout->addWidget(muteButton);
    layout->addWidget(deafenButton);
    layout->addWidget(settingsButton);

    // Create the main layout and add the container
    mainLayout->addWidget(container);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    // Style the user menu
    this->setFixedHeight(53);
    this->setStyleSheet("background-color: #292B2F;");

    // Connect buttons clicked signals
    QObject::connect(deafenButton, SIGNAL(leftClicked(int, bool)), this, SLOT(clicButton(int, bool)));
    QObject::connect(settingsButton, SIGNAL(leftClicked(int, bool)), this, SLOT(clicButton(int, bool)));
}

void UserMenu::setIcon(const std::string& iconFileName)
{
    avatar->setImage(iconFileName);
}

void UserMenu::clicButton(int type, bool active)
{
    if (type == Deafen) {
        // Mute when deafen pressed
        muteButton->setClicked(active);
    } else if (type == Settings) {
        // TODO : settings menu not implemented
    }
}

} // namespace Ui
