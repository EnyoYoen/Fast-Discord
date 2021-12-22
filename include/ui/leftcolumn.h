#pragma once

#include "homebutton.h"
#include "ui/guildwidget.h"
#include "api/guild.h"
#include "api/ressourcemanager.h"

#include <QScrollArea>
#include <QVBoxLayout>

#include <vector>

namespace Ui {

// The left column of the UI, with home button and guilds
class LeftColumn : public QScrollArea
{
    Q_OBJECT
public:
    LeftColumn(Api::RessourceManager *rm, QWidget *parent);

signals:
    void cleanRightColumn();
    void homeButtonClicked();
    void guildClicked(const std::string&);

public slots:
    void displayGuilds(const std::vector<Api::Guild *>& guilds);

private slots:
    void clicHomeButton();
    void clicGuild(const std::string&);
    void setUnreadGuild(const std::string& guildId);

private:
    // Main widgets
    QVBoxLayout *layout;
    HomeButton  *homeButton;

    Api::RessourceManager *rm; // To request the API

    std::vector<GuildWidget *> guildWidgets;

    bool homePageShown; // If the home page is shown
};

} // namespace Ui
