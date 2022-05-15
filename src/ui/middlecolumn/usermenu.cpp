#include "ui/middlecolumn/usermenu.h"

#include "api/request.h"

#include <QHBoxLayout>

namespace Ui {

UserMenu::UserMenu(Api::RessourceManager *rmp, const Api::Client *client, QWidget *parent)
    : QWidget(parent)
{
    // Set the ressource manager
    rm = rmp;

    deaf = false;
    muted = false;

    // Create the layout
    QHBoxLayout *mainLayout = new QHBoxLayout(this);

    // I have to create an other widget otherwise
    // the background color is not applied everywhere, I don't know why
    QWidget *container = new QWidget(this);
    QHBoxLayout *layout = new QHBoxLayout(container);

    // Get the icon of the actual user
    QString channelIconFileName;
    if (client->avatar.isNull()) {
        // Use an asset if the user doesn't have an icon
        channelIconFileName = "res/images/png/user-icon-asset0.png";

        avatar = new RoundedImage(channelIconFileName, 32, 32, 16, container);
    } else {
        avatar = new RoundedImage(32, 32, 16, container);

        // Request the icon
        channelIconFileName = client->id + (client->avatar.indexOf("a_") == 0 ? ".gif" : ".png");
        rm->getImage([this](void *iconFileName) {this->setIcon(*static_cast<QString *>(iconFileName));}, "https://cdn.discordapp.com/avatars/" + client->id + "/" + client->avatar, channelIconFileName);
    }

    // Set the background of the status icon
    QLabel *statusBackground = new QLabel(this);
    statusBackground->setFixedSize(16, 16);
    statusBackground->move(28, 29);
    statusBackground->setStyleSheet("border-radius: 8px;"
                                    "background-color: #292B2F;");

    // Set the status icon
    statusIcon = new QLabel(this);
    statusIcon->setFixedSize(10, 10);
    statusIcon->setStyleSheet("border-radius: 5px;");
    statusIcon->move(31, 32);
    rm->getClientSettings([&](void *settingsPtr){
        QString status = reinterpret_cast<Api::ClientSettings *>(settingsPtr)->status;
        if (status == "online") statusIcon->setStyleSheet("border-radius: 5px;"
                                                          "background-color: rgb(0, 224, 71);");
        else if (status == "idle") statusIcon->setStyleSheet("border-radius: 5px;"
                                                             "background-color: rgb(255, 169, 21);");
        else if (status == "dnd") statusIcon->setStyleSheet("border-radius: 5px;"
                                                            "background-color: rgb(255, 48, 51);");
        else statusIcon->setStyleSheet("border-radius: 5px;"
                                       "background-color: rgb(90, 90, 90);");
    });

    QFont font;
    font.setPixelSize(14);
    font.setFamily("whitney");

    // Create the widgets of the user menu
    QWidget *userInfos = new QWidget(container);
    QVBoxLayout *userInfosLayout = new QVBoxLayout(userInfos);
    QLabel *name = new QLabel(client->username, userInfos);
    name->setFont(font);
    font.setPixelSize(12);

    // Style the name label
    name->setFixedSize(84, 18);
    name->setStyleSheet("color: #DCDDDE;");

    // Create and style the discriminator label
    QLabel *discriminator = new QLabel("#" + client->discriminator, userInfos);
    discriminator->setFont(font);
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
    UserMenuButton *settingsButton = new UserMenuButton(SettingsButton, container);
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
    QObject::connect(deafenButton, &UserMenuButton::leftClicked, this, &UserMenu::clicButton);
    QObject::connect(muteButton, &UserMenuButton::leftClicked, this, &UserMenu::clicButton);
    QObject::connect(settingsButton, &UserMenuButton::leftClicked, this, &UserMenu::clicButton);
}

void UserMenu::setIcon(const QString& iconFileName)
{
    avatar->setImage(iconFileName);
}

void UserMenu::clicButton(int type, bool active)
{
    if (type == Deafen) {
        // Mute when deafen pressed
        muteButton->setClicked(active);
        deaf = active;
        emit voiceStateChanged((deaf ? true : muted), deaf);
    } else if (type == Mute) {
        if (!deaf)
            muted = active;
            emit voiceStateChanged((deaf ? true : muted), deaf);
    } else if (type == SettingsButton) {
        emit parametersClicked();
    }
}

} // namespace Ui
