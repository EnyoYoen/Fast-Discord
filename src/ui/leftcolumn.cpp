#include "ui/leftcolumn.h"

#include "api/request.h"

namespace Ui {

LeftColumn::LeftColumn(QWidget *parent)
    : QScrollArea(parent)
{
    // Attribute initialization
    homePageShown = true;

    // Create the widgets
    layout = new QVBoxLayout(this);
    homeButton = new HomeButton(this);
    QWidget *guildSeparator = new QWidget(this);

    // Style the guild separator
    guildSeparator->setFixedSize(32, 2);
    guildSeparator->setStyleSheet("background-color: #444649;");

    // Add widgets to the layout
    layout->addWidget(homeButton);
    layout->addWidget(guildSeparator);

    // Request and add guilds
    guilds = Api::Request::getGuilds();
    // Loop through every guild object
    for (size_t i = 0 ; i < guilds->size() ; i++) {
        // Create the guild widget and add it the guild storage and the layout
        GuildWidget *guildWidget = new GuildWidget(*(*guilds)[i], i, this);
        guildWidgets.push_back(guildWidget);

        layout->insertWidget(i + 2, guildWidget);
        layout->setAlignment(guildWidget, Qt::AlignHCenter);

        // Connect the clic signal
        QObject::connect(guildWidget, SIGNAL(leftClicked(Api::Guild&, unsigned int)), this, SLOT(clicGuild(Api::Guild&, unsigned int)));
    }

    // Set widgets position
    layout->insertStretch(-1, 100);
    layout->setAlignment(homeButton, Qt::AlignHCenter);
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

void LeftColumn::clicGuild(Api::Guild& guild, unsigned int id)
{
    // A guild is clicked

    if (homePageShown) {
        // Reset the home button if clicked
        homePageShown = false;
        homeButton->unclicked();
    }

    // Reset the stylesheet of the guild widgets except the one clicked
    for (size_t i = 0 ; i < guildWidgets.size() ; i++) {
        if (i != id) {
            guildWidgets[i]->unclicked();
        }
    }

    // Emit signals
    emit cleanRightColumn();
    emit guildClicked(guild);
}

} // namespace Ui
