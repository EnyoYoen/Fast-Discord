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

    // Connect the signal to setup the column guilds
    QObject::connect(this, SIGNAL(guildsRecieved(std::vector<Api::Guild *> *)), this, SLOT(displayGuilds(std::vector<Api::Guild *> *)));

    // Request guilds
    Api::Request::getGuilds([this](void *guilds) {
        emit guildsRecieved(static_cast<std::vector<Api::Guild *> *>(guilds));
    });

    // Set widgets alignments
    layout->insertStretch(-1, 100);
    layout->setAlignment(homeButton, Qt::AlignHCenter);
    layout->setAlignment(guildSeparator, Qt::AlignHCenter);

    // Style the layout
    layout->setSpacing(6);
    layout->setContentsMargins(0, 0, 0, 0);

    // Style this widget
    this->setFixedWidth(70);
    this->setStyleSheet("background-color: #202225;"
                        "border: none;");

    // Connect the home button clic signal
    QObject::connect(homeButton, SIGNAL(clicked()), this, SLOT(clicHomeButton()));
}

void LeftColumn::displayGuilds(std::vector<Api::Guild *> *guildsp)
{
    // Add guilds
    guilds = guildsp;

    // Loop through every guild object
    for (size_t i = 0 ; i < guilds->size() ; i++) {
        // Create the guild widget and add it the guild storage and the layout
        GuildWidget *guildWidget = new GuildWidget(*(*guilds)[i], i, this);
        guildWidgets.push_back(guildWidget);

        layout->insertWidget(i + 3, guildWidget);
        layout->setAlignment(guildWidget, Qt::AlignHCenter);

        // Connect the clic signal
        QObject::connect(guildWidget, SIGNAL(leftClicked(Api::Guild&, unsigned int)), this, SLOT(clicGuild(Api::Guild&, unsigned int)));
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
