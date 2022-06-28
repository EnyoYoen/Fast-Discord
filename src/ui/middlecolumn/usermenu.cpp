#include "ui/middlecolumn/usermenu.h"

#include "api/objects/client.h"
#include "api/request.h"

#include <QHBoxLayout>

namespace Ui {

UserMenu::UserMenu(Api::RessourceManager *rmp, QWidget *parent)
    : Widget(parent)
{
    // Set the ressource manager
    rm = rmp;

    deaf = false;
    muted = false;

    rm->getClient([this](void *clientPtr){
        Api::Client *client = reinterpret_cast<Api::Client *>(clientPtr);

        // Create the layout
        QHBoxLayout *layout = new QHBoxLayout(this);

        // Get the icon of the actual user
        QString channelIconFileName;
        if (client->avatar.isNull()) {
            // Use an asset if the user doesn't have an icon
            channelIconFileName = "res/images/png/user-icon-asset0.png";

            avatar = new RoundedImage(channelIconFileName, 32, 32, 16, this);
        } else {
            avatar = new RoundedImage(32, 32, 16, this);

            // Request the icon
            channelIconFileName = client->id + (client->avatar.indexOf("a_") == 0 ? ".gif" : ".png");
            rm->getImage([this](void *iconFileName) {this->setIcon(*static_cast<QString *>(iconFileName));}, "https://cdn.discordapp.com/avatars/" + client->id + "/" + client->avatar, channelIconFileName);
        }

        // Set the background of the status icon
        Widget *statusBackground = new Widget(this);
        statusBackground->setFixedSize(16, 16);
        statusBackground->move(28, 29);
        statusBackground->setBorderRadius(8);
        statusBackground->setBackgroundColor(Settings::BackgroundSecondaryAlt);
        statusBackground->show();

        // Set the status icon
        statusIcon = new Label(this);
        statusIcon->setFixedSize(10, 10);
        statusIcon->setBorderRadius(5);
        statusIcon->move(31, 32);
        statusIcon->show();
        rm->getClientSettings([&](void *settingsPtr){
            QString status = reinterpret_cast<Api::ClientSettings *>(settingsPtr)->status;
            if (status == "online") statusIcon->setBackgroundColor(Settings::StatusOnline);
            else if (status == "idle") statusIcon->setBackgroundColor(Settings::StatusIdle);
            else if (status == "dnd") statusIcon->setBackgroundColor(Settings::StatusDND);
            else statusIcon->setBackgroundColor(Settings::StatusOffline);
        });

        QFont font;
        font.setPixelSize(14);
        font.setFamily("whitney");

        // Create the widgets of the user menu
        Widget *userInfos = new Widget(this);
        userInfos->setBackgroundColor(Settings::BackgroundSecondaryAlt);
        QVBoxLayout *userInfosLayout = new QVBoxLayout(userInfos);
        Label *name = new Label(client->username, userInfos);
        name->setFont(font);
        font.setPixelSize(12);

        // Style the name label
        name->setFixedSize(84, 18);
        name->setTextColor(Settings::TextNormal);

        // Create and style the discriminator label
        Label *discriminator = new Label("#" + client->discriminator, userInfos);
        discriminator->setFont(font);
        discriminator->setFixedSize(84, 13);
        discriminator->setTextColor(Settings::HeaderSecondary);

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
        muteButton = new UserMenuButton(Mute, this);
        UserMenuButton *deafenButton = new UserMenuButton(Deafen, this);
        UserMenuButton *settingsButton = new UserMenuButton(SettingsButton, this);
        layout->addWidget(muteButton);
        layout->addWidget(deafenButton);
        layout->addWidget(settingsButton);

        // Style the user menu
        this->setFixedHeight(53);
        this->setBackgroundColor(Settings::BackgroundSecondaryAlt);

        // Connect buttons clicked signals
        QObject::connect(deafenButton, &UserMenuButton::leftClicked, this, &UserMenu::clicButton);
        QObject::connect(muteButton, &UserMenuButton::leftClicked, this, &UserMenu::clicButton);
        QObject::connect(settingsButton, &UserMenuButton::leftClicked, this, &UserMenu::clicButton);
    });
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
