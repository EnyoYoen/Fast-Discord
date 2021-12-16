#pragma once

#include "homebutton.h"
#include "ui/guildwidget.h"
#include "api/guild.h"
#include "api/request.h"

#include <QScrollArea>
#include <QVBoxLayout>

#include <vector>

namespace Ui {

// The left column of the UI, with home button and guilds
class LeftColumn : public QScrollArea
{
    Q_OBJECT
public:
    LeftColumn(Api::Requester *requester, QWidget *parent);
    void displayGuilds(std::vector<Api::Guild *> *guilds);

signals:
    void cleanRightColumn();
    void homeButtonClicked();
    void guildClicked(Api::Guild& guild);

private slots:
    void clicHomeButton();
    void clicGuild(Api::Guild& guild, unsigned int id);
    void setUnreadGuild(const std::string& guildId);

private:
    // Main widgets
    QVBoxLayout *layout;
    HomeButton  *homeButton;

    Api::Requester *requester; // To request the API

    // Store guilds and guild widgets that we already gathered
    std::vector<Api::Guild *> *guilds;
    std::vector<GuildWidget *> guildWidgets;

    bool homePageShown; // If the home page is shown
};

} // namespace Ui
