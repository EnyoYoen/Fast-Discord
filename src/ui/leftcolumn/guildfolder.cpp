#include "ui/leftcolumn/guildfolder.h"

#include "ui/leftcolumn/guildpill.h"

#include <QHBoxLayout>
#include <QGridLayout>
#include <QPainter>

namespace Ui {

GuildFolder::GuildFolder(Api::RessourceManager *rm, const Api::GuildFolder *guildFolder, const QVector<Api::Guild *>& guilds, QWidget *parent)
    : Widget(parent)
{
    int color = guildFolder->color;
    guildIds = guildFolder->guildIds;

    QHBoxLayout *closedLayout = new QHBoxLayout(this);
    closedLayout->setSpacing(8);
    closedLayout->setContentsMargins(0, 0, 0, 0);

    closedContent = new Widget(this);
    closedContent->setFixedSize(48, 48);
    closedContent->setBorderRadius(16);
    closedContent->setBackgroundColor(QColor((color & 0x00FF0000) >> 16, (color & 0x0000FF00) >> 8, color & 0x000000FF, 101));
    QGridLayout *contentLayout = new QGridLayout(closedContent);
    contentLayout->setContentsMargins(6, 6, 0, 0);
    contentLayout->setSpacing(0);

    pill = new GuildPill(this);
    closedLayout->addWidget(pill);
    closedLayout->addWidget(closedContent);


    openedContent = new Widget(this);
    openedContent->hide();
    openedContent->setFixedHeight(guilds.size() * (48 + 10) + 48 - 10);
    openedContent->setBorderRadius(16);
    openedContent->setBackgroundColor(QColor((color & 0x00FF0000) >> 16, (color & 0x0000FF00) >> 8, color & 0x000000FF, 101));

    QPixmap img("res/images/svg/folder-icon.svg");
    QPainter qp(&img);
    qp.setCompositionMode(QPainter::CompositionMode_SourceIn);
    qp.fillRect(img.rect(),QColor(color));
    qp.end();
    closeButton = new Widget(openedContent);
    closeButton->setPixmap(img);
    closeButton->setFixedSize(72, 48);

    QVBoxLayout *openedLayout = new QVBoxLayout(openedContent);
    openedLayout->setSpacing(10);
    openedLayout->setContentsMargins(0, 0, 0, 10);
    openedLayout->addWidget(closeButton);

    for (int i = 0 ; i < guilds.size() ; i++) {
        Api::Guild actualGuild = *guilds[i];

        GuildWidget *guildWidget = new GuildWidget(rm, *guilds[i], openedContent);
        QObject::connect(guildWidget, &GuildWidget::leftClicked, this, &GuildFolder::propagateGuildClic);
        openedLayout->addWidget(guildWidget);
        guildWidgets.push_back(guildWidget);

        if (i < 4) {
            contentLayout->addWidget(new GuildIcon(rm, actualGuild.id, actualGuild.name, actualGuild.icon, closedContent, this), (i < 2 ? 0 : 1), i%2);
        }
    }

    this->setFixedSize(72, 48);
    this->setContentsMargins(0, 0, 12, 0);
}

void GuildFolder::mouseReleaseEvent(QMouseEvent *event)
{
    if (!opened) {
        this->setFixedHeight(guildWidgets.size() * (48 + 10) + 48);
        this->setContentsMargins(0, 0, 0, 10);

        closedContent->hide();
        pill->hide();
        openedContent->show();

        opened = !opened;
    } else if (closeButton->contentsRect().contains(event->pos())) {
        this->setFixedSize(72, 48);
        this->setContentsMargins(0, 0, 12, 0);

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
        pill->setFixedHeight(8);
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
            pill->setFixedHeight(8);
        else 
            pill->setFixedHeight(0);
    }
}

void GuildFolder::setUnread(const Api::Snowflake& id)
{
    unread = true;
    if (!clicked) 
        pill->setFixedHeight(8);
    
    for (int i = 0 ; i < guildWidgets.size() ; i++) {
        if (guildWidgets[i]->id == id)
            guildWidgets[i]->setUnread(true);
    }

}

void GuildFolder::propagateGuildClic(const Api::Snowflake& id)
{
    clicked = true;
    pill->setFixedHeight(40);

    emit guildClicked(id);
}

void GuildFolder::enterEvent(QEvent *)
{
    if (!clicked)
        pill->setFixedHeight(20);
}

void GuildFolder::leaveEvent(QEvent *)
{
    if (clicked)
        pill->setFixedHeight(40);
    else if (unread)
        pill->setFixedHeight(8);
    else
        pill->setFixedHeight(0);
}

} // namespace Ui