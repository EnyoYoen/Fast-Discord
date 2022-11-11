#pragma once

#include "ui/common/basicwidgets.h"
#include "ui/common/roundedimage.h"
#include "ui/leftcolumn/guildpill.h"
#include "ui/leftcolumn/guildicon.h"
#include "api/ressourcemanager.h"
#include "api/objects/guild.h"
#include "api/objects/snowflake.h"

#include <QFrame>
#include <QLabel>
#include <QHBoxLayout>

namespace Ui {

// A widget to show a guild (in the left column)
class GuildWidget : public Widget
{
    Q_OBJECT
public:
    GuildWidget(Api::RessourceManager *rm, const Api::Guild& guildp, QWidget *parent);
    void unclicked(); // Reset the stylesheet of the widget
    void setUnread(bool unread);

    Api::Snowflake id; // The id of the guild
    bool           clicked;
    bool           unreadMessages;

signals:
    void leftClicked(const Api::Snowflake&);
    void rightClicked(const Api::Snowflake&);

private:
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *) override;
    void enterEvent(QEvent *) override;
    void leaveEvent(QEvent *) override;

    Api::RessourceManager *rm; // To request the API

    // All the main widgets
    QHBoxLayout *layout;
    GuildIcon   *icon;  
    GuildPill   *pill;
};

} // namespace Ui
