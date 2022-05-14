#pragma once

#include "ui/leftcolumn/guildwidget.h"
#include "api/ressourcemanager.h"
#include "api/objects/snowflake.h"

#include <QLabel>
#include <QMouseEvent>

namespace Ui {

class GuildFolder : public QLabel
{
    Q_OBJECT
public:
    GuildFolder(Api::RessourceManager *rm, const Api::GuildFolder *guildFolder, const QVector<Api::Guild *>& guilds, QWidget *parent);
    void unclicked();
    void unclickedExcept(const Api::Snowflake& id);
    void setUnread(const Api::Snowflake& id);

    QVector<Api::Snowflake> guildIds;

signals:
    void guildClicked(const Api::Snowflake& id);

private slots:
    void propagateGuildClic(const Api::Snowflake& id);

private:
    void mouseReleaseEvent(QMouseEvent *event) override;
    void enterEvent(QEvent *) override;
    void leaveEvent(QEvent *) override;

    QLabel *closedContent;
    GuildPill *pill;
    QWidget *openedContent;
    QLabel *closeButton;

    QVector<GuildWidget *> guildWidgets;
    QVector<QLabel *> guildsIcon;
    bool opened = false;
    bool clicked = false;
    bool unread = false;
};

}
