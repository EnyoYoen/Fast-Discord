#pragma once

#include "ui/leftcolumn/homebutton.h"
#include "ui/leftcolumn/guildwidget.h"
#include "ui/leftcolumn/guildfolder.h"
#include "api/ressourcemanager.h"
#include "api/objects/guild.h"
#include "api/objects/snowflake.h"

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
    void setUnreadGuild(const Api::Snowflake& guildId);
    void addGuild(const Api::GuildGateway *guild);
    void updateGuild(const Api::Guild *guild);
    void deleteGuild(const Api::Guild *guild);

private slots:
    void clicHomeButton();
    void clicGuild(const Api::Snowflake&);

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
