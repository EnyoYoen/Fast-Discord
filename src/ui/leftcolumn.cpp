#include "ui/leftcolumn.h"

namespace Ui {

LeftColumn::LeftColumn(Api::RessourceManager *rmp, QWidget *parent)
    : QScrollArea(parent)
{
    // Attribute initialization
    rm = rmp;
    homePageShown = true;

    // Create the widgets
    QWidget *content = new QWidget(this);
    layout = new QVBoxLayout(content);
    homeButton = new HomeButton(content);
    QWidget *guildSeparator = new QWidget(content);
    QWidget *spacer = new QWidget(content);

    // Style the spacer
    spacer->setFixedHeight(6);

    // Style the guild separator
    guildSeparator->setFixedSize(32, 2);
    guildSeparator->setStyleSheet("background-color: #444649;");

    // Add widgets to the layout
    layout->addWidget(spacer);
    layout->addWidget(homeButton);
    layout->addWidget(guildSeparator);

    // Set widgets alignments
    layout->insertStretch(3, 1);
    layout->setAlignment(guildSeparator, Qt::AlignHCenter);

    // Style the layout
    layout->setSpacing(6);
    layout->setContentsMargins(0, 0, 0, 0);


    // Style this widget
    this->setWidget(content);
    this->setWidgetResizable(true);
    this->setFixedWidth(72);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setStyleSheet("background-color: #202225;"
                        "border: none;");

    // Connect the home button clic signal
    QObject::connect(homeButton, SIGNAL(clicked()), this, SLOT(clicHomeButton()));
}

void LeftColumn::displayGuilds(const std::vector<Api::Guild *>& guilds)
{
    rm->getClientSettings([this, guilds](void *clientPtr) {
        const std::vector<std::string>& positions = *reinterpret_cast<Api::ClientSettings *>(clientPtr)->guildPositions;
        const std::vector<Api::GuildFolder *>& folders = *reinterpret_cast<Api::ClientSettings *>(clientPtr)->guildFolders;

        unsigned int widgetCounter = 0;
        for (unsigned int i = 0 ; i < folders.size() ; i++) {
            Api::GuildFolder *folder = folders[i];
            std::vector<Api::Guild *> folderGuilds;
            for (unsigned int j = 0 ; j < folder->guildIds->size() ; j++) {
                for (unsigned int k = 0 ; k < guilds.size() ; k++) {
                    if ((*folder->guildIds)[j] == *(guilds[k]->id)) {
                        if (folder->intId == 0 && (folder->strId == nullptr || *folder->strId == "")) {
                            GuildWidget *guildWidget = new GuildWidget(rm, *guilds[k], this);
                            guildWidgets.push_back(guildWidget);
                            layout->insertWidget(widgetCounter + 3, guildWidget);
                            QObject::connect(guildWidget, SIGNAL(leftClicked(const std::string&)), this, SLOT(clicGuild(const std::string&)));
                            widgetCounter++;
                            break;
                        } else {
                            folderGuilds.push_back(guilds[k]);
                        }
                    }
                }
            }
            if ((folder->intId != 0) || (folder->strId != nullptr && *folder->strId != "")) {
                GuildFolder *folderWidget = new GuildFolder(rm, folder, folderGuilds, this);
                guildFolders.push_back(folderWidget);
                layout->insertWidget(widgetCounter + 3, folderWidget);
                QObject::connect(folderWidget, SIGNAL(guildClicked(const std::string&)), this, SLOT(clicGuild(const std::string&)));
                widgetCounter++;
            }
        }
        
        if (folders.empty() && !guilds.empty()) {
            for (unsigned int i = 0 ; i < guilds.size() ; i++) {
                GuildWidget *guildWidget = new GuildWidget(rm, *guilds[i], this);
                guildWidgets.push_back(guildWidget);
                layout->insertWidget(i + 3, guildWidget);
                QObject::connect(guildWidget, SIGNAL(leftClicked(const std::string&)), this, SLOT(clicGuild(const std::string&)));
            }
        }
    });
}

void LeftColumn::clicHomeButton()
{
    // The home button is clicked
    if (!homePageShown) { // Do nothing if the home page is already shown
        homePageShown = true;

        // Reset the stylesheet of the actual clicked guild
        for (size_t i = 0 ; i < guildWidgets.size() ; i++) {
            guildWidgets[i]->unclicked();
        }
        for (size_t i = 0 ; i < guildFolders.size() ; i++) {
            guildFolders[i]->unclicked();
        }

        // Emit signals
        emit cleanRightColumn();
        emit homeButtonClicked();
    }
}

void LeftColumn::clicGuild(const std::string& guildId)
{
    // A guild is clicked

    if (homePageShown) {
        // Reset the home button if clicked
        homePageShown = false;
        homeButton->unclicked();
    }

    // Reset the stylesheet of the guild widgets except the one clicked
    for (size_t i = 0 ; i < guildWidgets.size() ; i++) {
        if (guildWidgets[i]->id != guildId) {
            guildWidgets[i]->unclicked();
        }
    }
    for (size_t i = 0 ; i < guildFolders.size() ; i++) {
        guildFolders[i]->unclickedExcept(guildId);
    }

    // Emit signals
    emit cleanRightColumn();
    emit guildClicked(guildId);
}

void LeftColumn::setUnreadGuild(const std::string& guildId)
{
    for (size_t i = 0 ; i < guildWidgets.size() ; i++) {
        if (guildWidgets[i]->id == guildId) {
            guildWidgets[i]->setUnread(true);
            return;
        }
    }

    for (unsigned int i = 0 ; i < guildFolders.size() ; i++) {
        std::vector<std::string> guildIds = guildFolders[i]->guildIds;
        for (unsigned int j = 0 ; j < guildIds.size() ; j++) {
            if (guildIds[j] == guildId) {
                guildFolders[i]->setUnread(guildId);
                return;
            }
        }
    }
}

} // namespace Ui
