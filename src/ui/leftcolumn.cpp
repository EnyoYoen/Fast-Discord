#include "ui/leftcolumn.h"

namespace Ui {

LeftColumn::LeftColumn(Api::RessourceManager *rmp, QWidget *parent)
    : QScrollArea(parent)
{
    // Attribute initialization
    rm = rmp;
    homePageShown = true;

    // Create the widgets
    layout = new QVBoxLayout(this);
    homeButton = new HomeButton(this);
    QWidget *guildSeparator = new QWidget(this);
    QWidget *spacer = new QWidget(this);

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
    layout->insertStretch(-1, 100);
    layout->setAlignment(guildSeparator, Qt::AlignHCenter);

    // Style the layout
    layout->setSpacing(6);
    layout->setContentsMargins(0, 0, 0, 0);

    // Style this widget
    this->setFixedWidth(72);
    this->setStyleSheet("background-color: #202225;"
                        "border: none;");

    // Connect the home button clic signal
    QObject::connect(homeButton, SIGNAL(clicked()), this, SLOT(clicHomeButton()));
}

void LeftColumn::displayGuilds(const std::vector<Api::Guild *>& guilds)
{
    // Loop through every guild object
    for (size_t i = 0 ; i < guilds.size() ; i++) {
        // Create the guild widget and add it the guild storage and the layout
        GuildWidget *guildWidget = new GuildWidget(rm, *guilds[i], this);
        guildWidgets.push_back(guildWidget);

        layout->insertWidget(i + 3, guildWidget);

        // Connect the clic signal
        QObject::connect(guildWidget, SIGNAL(leftClicked(const std::string&)), this, SLOT(clicGuild(const std::string&)));
    }
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

    // Emit signals
    emit cleanRightColumn();
    emit guildClicked(guildId);
}

void LeftColumn::setUnreadGuild(const std::string& guildId)
{
    rm->getGuilds([&](void *guilds){
        for (size_t i = 0 ; i < static_cast<std::vector<Api::Guild *> *>(guilds)->size() ; i++) {
            if (*(*static_cast<std::vector<Api::Guild *> *>(guilds))[i]->id == guildId)
                guildWidgets[i]->setUnread(true);
        }
    });
}

} // namespace Ui
