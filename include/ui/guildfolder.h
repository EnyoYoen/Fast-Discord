#pragma once

#include "ui/guildwidget.h"
#include "api/ressourcemanager.h"

#include <QLabel>
#include <QMouseEvent>

#include <vector>

namespace Ui {

class GuildFolder : public QLabel
{
    Q_OBJECT
public:
    GuildFolder(Api::RessourceManager *rm, Api::GuildFolder *guildFolder, const std::vector<Api::Guild *>& guilds, QWidget *parent);
    void unclicked();
    void unclickedExcept(const std::string& id);
    void setUnread(const std::string& id);

    std::vector<std::string> guildIds;

signals:
    void guildClicked(const std::string& id);

private slots:
    void propagateGuildClic(const std::string& id);

private:
    void mouseReleaseEvent(QMouseEvent *event) override;
    void enterEvent(QEvent *) override;
    void leaveEvent(QEvent *) override;

    QLabel *closedContent;
    GuildPill *pill;
    QWidget *openedContent;
    QLabel *closeButton;

    std::vector<GuildWidget *> guildWidgets;
    std::vector<QLabel *> guildsIcon;
    bool opened = false;
    bool clicked = false;
    bool unread = false;
};

}
