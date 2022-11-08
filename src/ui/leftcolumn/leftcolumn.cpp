#include "ui/leftcolumn/leftcolumn.h"

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
    Widget *guildSeparator = new Widget(content);

    // Style the guild separator
    guildSeparator->setFixedSize(Settings::scale(32), Settings::scale(2));
    guildSeparator->setBackgroundColor(Settings::BackgroundModifierAccent);

    // Add widgets to the layout
    layout->addSpacing(Settings::scale(6));
    layout->addWidget(homeButton);
    layout->addWidget(guildSeparator);

    // Set widgets alignments
    layout->insertStretch(3, 1);
    layout->setAlignment(guildSeparator, Qt::AlignHCenter);

    // Style the layout
    layout->setSpacing(Settings::scale(8));
    layout->setContentsMargins(0, 0, 0, 0);


    // Style this widget
    this->setWidget(content);
    this->setWidgetResizable(true);
    this->setFixedWidth(Settings::scale(72));
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setStyleSheet("border: none;"
                        "background-color:" + Settings::colors[Settings::BackgroundTertiary].name());

    // Connect the home button clic signal
    QObject::connect(homeButton, &HomeButton::clicked, this, &LeftColumn::clicHomeButton);
}

void LeftColumn::displayGuilds(const QVector<Api::Guild *>& guilds)
{
    rm->getClientSettings([this, guilds](Api::CallbackStruct cb) {
        const QVector<QString>& positions = reinterpret_cast<Api::ClientSettings *>(cb.data)->guildPositions;
        const QVector<Api::GuildFolder *>& folders = reinterpret_cast<Api::ClientSettings *>(cb.data)->guildFolders;

        unsigned int widgetCounter = 0;
        for (int i = 0 ; i < folders.size() ; i++) {
            Api::GuildFolder *folder = folders[i];
            QVector<Api::Guild *> folderGuilds;
            for (int j = 0 ; j < folder->guildIds.size() ; j++) {
                for (int k = 0 ; k < guilds.size() ; k++) {
                    if (folder->guildIds[j] == guilds[k]->id) {
                        if (folder->intId == 0 && folder->strId.isNull()) {
                            GuildWidget *guildWidget = new GuildWidget(rm, *guilds[k], this);
                            guildWidgets.push_back(guildWidget);
                            layout->insertWidget(widgetCounter + 3, guildWidget);
                            QObject::connect(guildWidget, &GuildWidget::leftClicked, this, &LeftColumn::clicGuild);
                            widgetCounter++;
                            break;
                        } else {
                            folderGuilds.push_back(guilds[k]);
                        }
                    }
                }
            }
            if (folder->intId != 0 || !folder->strId.isNull()) {
                GuildFolder *folderWidget = new GuildFolder(rm, folder, folderGuilds, this);
                guildFolders.push_back(folderWidget);
                layout->insertWidget(widgetCounter + 3, folderWidget);
                QObject::connect(folderWidget, &GuildFolder::guildClicked, this, &LeftColumn::clicGuild);
                widgetCounter++;
            }
        }
        
        if (folders.empty() && !guilds.empty()) {
            for (int i = 0 ; i < guilds.size() ; i++) {
                GuildWidget *guildWidget = new GuildWidget(rm, *guilds[i], this);
                guildWidgets.push_back(guildWidget);
                layout->insertWidget(i + 3, guildWidget);
                QObject::connect(guildWidget, &GuildWidget::leftClicked, this, &LeftColumn::clicGuild);
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
        for (int i = 0 ; i < guildWidgets.size() ; i++) {
            guildWidgets[i]->unclicked();
        }
        for (int i = 0 ; i < guildFolders.size() ; i++) {
            guildFolders[i]->unclicked();
        }

        // Emit signals
        emit cleanRightColumn();
        emit homeButtonClicked();
    }
}

void LeftColumn::clicGuild(const Api::Snowflake& guildId)
{
    // A guild is clicked

    if (homePageShown) {
        // Reset the home button if clicked
        homePageShown = false;
        homeButton->unclicked();
    }

    // Reset the stylesheet of the guild widgets except the one clicked
    for (int i = 0 ; i < guildWidgets.size() ; i++) {
        if (guildWidgets[i]->id != guildId) {
            guildWidgets[i]->unclicked();
        }
    }
    for (int i = 0 ; i < guildFolders.size() ; i++) {
        guildFolders[i]->unclickedExcept(guildId);
    }

    // Emit signals
    emit cleanRightColumn();
    emit guildClicked(guildId);
}

void LeftColumn::setUnreadGuild(const Api::Snowflake& guildId)
{
    for (int i = 0 ; i < guildWidgets.size() ; i++) {
        if (guildWidgets[i]->id == guildId) {
            guildWidgets[i]->setUnread(true);
            return;
        }
    }

    for (int i = 0 ; i < guildFolders.size() ; i++) {
        QVector<Api::Snowflake> guildIds = guildFolders[i]->guildIds;
        for (int j = 0 ; j < guildIds.size() ; j++) {
            if (guildIds[j] == guildId) {
                guildFolders[i]->setUnread(guildId);
                return;
            }
        }
    }
}

} // namespace Ui
