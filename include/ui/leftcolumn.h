#pragma once

#include "ui/homebutton.h"
#include "ui/guildwidget.h"
#include "ui/guildfolder.h"
#include "api/guild.h"
#include "api/ressourcemanager.h"
#include "api/snowflake.h"

#include <QScrollArea>
#include <QVBoxLayout>

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
    void guildClicked(const Api::Snowflake&);

public slots:
    void displayGuilds(const QVector<Api::Guild *>& guilds);

private slots:
    void clicHomeButton();
    void clicGuild(const Api::Snowflake&);
    void setUnreadGuild(const Api::Snowflake& guildId);

private:
    // Main widgets
    QVBoxLayout *layout;
    HomeButton  *homeButton;

    Api::RessourceManager *rm; // To request the API

    QVector<GuildWidget *> guildWidgets;
    QVector<GuildFolder *> guildFolders;

    bool homePageShown; // If the home page is shown
};

} // namespace Ui
