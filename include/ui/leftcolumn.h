#pragma once

#include "homebutton.h"
#include "ui/guildwidget.h"
#include "api/guild.h"

#include <QScrollArea>
#include <QVBoxLayout>

#include <vector>

namespace Ui {

// The left column of the UI, with home button and guilds
class LeftColumn : public QScrollArea
{
    Q_OBJECT
public:
    LeftColumn(QWidget *parent);

signals:
    void cleanRightColumn();
    void homeButtonClicked();
    void guildClicked(Api::Guild& guild);

private slots:
    void clicHomeButton();
    void clicGuild(Api::Guild& guild, unsigned int id);

private:
    // Main widgets
    QVBoxLayout *layout;
    HomeButton  *homeButton;

    // Store guilds and guild widgets that we already gathered
    std::vector<Api::Guild *> *guilds;
    std::vector<GuildWidget *> guildWidgets;

    bool homePageShown; // If the home page is shown
};

} // namespace Ui
