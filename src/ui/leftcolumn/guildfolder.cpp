#include "ui/leftcolumn/guildfolder.h"

#include "ui/leftcolumn/guildpill.h"

#include <QHBoxLayout>
#include <QGridLayout>
#include <QPainter>

namespace Ui {

GuildFolder::GuildFolder(Api::RessourceManager *rmp, const Api::GuildFolder *guildFolder, const QVector<Api::Guild *>& guilds, QWidget *parent)
    : Widget(parent)
{
    rm = rmp;
    int color = guildFolder->color;
    guildIds = guildFolder->guildIds;

    QHBoxLayout *closedLayout = new QHBoxLayout(this);
    closedLayout->setSpacing(Settings::scale(8));
    closedLayout->setContentsMargins(0, 0, 0, 0);

    closedContent = new Widget(this);
    closedContent->setFixedSize(Settings::scale(48), Settings::scale(48));
    closedContent->setBorderRadius(Settings::scale(16));
    closedContent->setBackgroundColor(QColor((color & 0x00FF0000) >> 16, (color & 0x0000FF00) >> 8, color & 0x000000FF, 101));
    contentLayout = new QGridLayout(closedContent);
    contentLayout->setContentsMargins(6, 6, 0, 0);
    contentLayout->setSpacing(0);

    pill = new GuildPill(this);
    closedLayout->addWidget(pill);
    closedLayout->addWidget(closedContent);


    openedContent = new Widget(this);
    openedContent->hide();
    openedContent->setFixedHeight(guilds.size() * (Settings::scale(48) + Settings::scale(10)) + Settings::scale(48) - Settings::scale(10));
    openedContent->setBorderRadius(Settings::scale(16));
    openedContent->setBackgroundColor(QColor((color & 0x00FF0000) >> 16, (color & 0x0000FF00) >> 8, color & 0x000000FF, 101));

    QPixmap img(":folder-icon.svg");
    QPainter qp(&img);
    qp.setCompositionMode(QPainter::CompositionMode_SourceIn);
    qp.fillRect(img.rect(),QColor(color));
    qp.end();
    closeButton = new Widget(openedContent);
    closeButton->setPixmap(img);
    closeButton->setFixedSize(Settings::scale(72), Settings::scale(48));

    openedLayout = new QVBoxLayout(openedContent);
    openedLayout->setSpacing(Settings::scale(10));
    openedLayout->setContentsMargins(0, 0, 0, Settings::scale(10));
    openedLayout->addWidget(closeButton);

    for (int i = 0 ; i < guilds.size() ; i++) {
        Api::Guild actualGuild = *guilds[i];

        GuildWidget *guildWidget = new GuildWidget(rm, *guilds[i], openedContent);
        QObject::connect(guildWidget, &GuildWidget::leftClicked, this, &GuildFolder::propagateGuildClic);
        openedLayout->addWidget(guildWidget);
        guildWidgets.push_back(guildWidget);

        if (i < 4) {
            GuildIcon *icon = new GuildIcon(rm, actualGuild.id, actualGuild.name, actualGuild.icon, true, closedContent);
            guildsIcon.append(icon);
            contentLayout->addWidget(icon, (i < 2 ? 0 : 1), i%2);
        }
    }

    this->setFixedSize(Settings::scale(72), Settings::scale(48));
    this->setContentsMargins(0, 0, Settings::scale(12), 0);
}

void GuildFolder::mouseReleaseEvent(QMouseEvent *event)
{
    if (!opened) {
        this->setFixedHeight(guildWidgets.size() * (Settings::scale(48) + Settings::scale(10)) + Settings::scale(48));
        this->setContentsMargins(0, 0, 0, Settings::scale(10));

        closedContent->hide();
        pill->hide();
        openedContent->show();

        opened = !opened;
    } else if (closeButton->contentsRect().contains(event->pos())) {
        this->setFixedSize(Settings::scale(72), Settings::scale(48));
        this->setContentsMargins(0, 0, Settings::scale(12), 0);

        openedContent->hide();
        closedContent->show();
        pill->show();

        opened = !opened;
    }
}

void GuildFolder::unclicked()
{
    clicked = false;
    if (unread)
        pill->setFixedHeight(Settings::scale(8));
    else 
        pill->setFixedHeight(0);

    for (int i = 0 ; i < guildWidgets.size() ; i++)
        guildWidgets[i]->unclicked();
}

void GuildFolder::unclickedExcept(const Api::Snowflake& id)
{
    clicked = false;
    for (int i = 0 ; i < guildWidgets.size() ; i++) {
        if (guildWidgets[i]->id != id) {
            guildWidgets[i]->unclicked();
        } else {
            clicked = true;
        }
    }
    if (!clicked) {
        if (unread)
            pill->setFixedHeight(Settings::scale(8));
        else 
            pill->setFixedHeight(0);
    }
}

bool GuildFolder::setUnread(const Api::Snowflake& id)
{
    unread = true;
    if (!clicked) 
        pill->setFixedHeight(8);
    
    for (int i = 0 ; i < guildWidgets.size() ; i++) {
        if (guildWidgets[i]->id == id) {
            guildWidgets[i]->setUnread(true);
            return true;
        }
    }

    return false;
}

bool GuildFolder::updateGuild(const Api::Guild *guild)
{
    for (int i = 0 ; i < guildWidgets.size() ; i++) {
        if (guildWidgets[i]->id == guild->id) {
            GuildWidget *newGuild = new GuildWidget(rm, *guild, this);
            guildWidgets[i]->deleteLater();
            guildWidgets.replace(i, newGuild);
            openedLayout->insertWidget(i+1, newGuild);
            QObject::connect(newGuild, &GuildWidget::leftClicked, this, &GuildFolder::propagateGuildClic);

            if (i < 4) {
                GuildIcon *newIcon = new GuildIcon(rm, guild->id, guild->name, guild->icon, true, closedContent);
                guildsIcon[i]->deleteLater();
                guildsIcon.replace(i, newIcon);
                contentLayout->addWidget(newIcon, (i < 2 ? 0 : 1), i%2);
            }

            return true;
        }
    }
    return false;
}

bool GuildFolder::deleteGuild(const Api::Snowflake& id)
{
    for (int i = 0 ; i < guildWidgets.size() ; i++) {
        if (guildWidgets[i]->id == id) {
            if (guildWidgets.size() == 1) {
                return true;
            } else {
                int c = 0;
                for (GuildWidget *guildWidget : guildWidgets) {
                    if (guildWidget->unreadMessages) c++;
                }

                GuildWidget *widget = guildWidgets[i];
                if (widget->clicked || widget->unreadMessages) {
                    if ((clicked && widget->clicked) || !clicked) {
                        if (unread) {
                            if (widget->unreadMessages && c == 1)
                                pill->setFixedHeight(0);
                            else
                                pill->setFixedHeight(8);
                        } else {
                            pill->setFixedHeight(0);
                        }
                    }
                }

                widget->deleteLater();
                guildsIcon[i]->deleteLater();
                guildWidgets.remove(i);
                guildsIcon.remove(i);

                return false;
            }
        }
    }
    return false;
}

void GuildFolder::propagateGuildClic(const Api::Snowflake& id)
{
    clicked = true;
    pill->setFixedHeight(Settings::scale(40));

    emit guildClicked(id);
}

void GuildFolder::enterEvent(QEvent *)
{
    if (!clicked)
        pill->setFixedHeight(Settings::scale(20));
}

void GuildFolder::leaveEvent(QEvent *)
{
    if (clicked)
        pill->setFixedHeight(Settings::scale(40));
    else if (unread)
        pill->setFixedHeight(Settings::scale(8));
    else
        pill->setFixedHeight(0);
}

} // namespace Ui