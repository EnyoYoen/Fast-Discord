#include "ui/guildfolder.h"

#include "ui/guildpill.h"

#include <QHBoxLayout>
#include <QGridLayout>
#include <QPainter>

namespace Ui {

GuildFolder::GuildFolder(Api::RessourceManager *rm, const Api::GuildFolder *guildFolder, const QVector<Api::Guild *>& guilds, QWidget *parent)
    : QLabel(parent)
{
    int color = guildFolder->color;
    guildIds = guildFolder->guildIds;

    QHBoxLayout *closedLayout = new QHBoxLayout(this);
    closedLayout->setSpacing(8);
    closedLayout->setContentsMargins(0, 0, 0, 0);

    closedContent = new QLabel(this);
    QGridLayout *contentLayout = new QGridLayout(closedContent);
    closedContent->setFixedSize(48, 48);
    closedContent->setStyleSheet("background-color: rgba(" 
                                 + QString::number((color & 0x00FF0000) >> 16)
                                 + ", " + QString::number((color & 0x0000FF00) >> 8)
                                 + ", " + QString::number(color & 0x000000FF) + ", 0.4);"
                                 "border-radius: 16px;");

    pill = new GuildPill(this);
    closedLayout->addWidget(pill);
    closedLayout->addWidget(closedContent);


    openedContent = new QWidget(this);
    openedContent->hide();
    openedContent->setFixedHeight(guilds.size() * (48 + 10) + 48 - 10);
    openedContent->setStyleSheet("background-color: rgba(" 
                                 + QString::number((color & 0x00FF0000) >> 16)
                                 + ", " + QString::number((color & 0x0000FF00) >> 8)
                                 + ", " + QString::number(color & 0x000000FF) + ", 0.4);"
                                 "border-radius: 16px;");

    QPixmap img("res/images/svg/folder-icon.svg");
    QPainter qp(&img);
    qp.setCompositionMode(QPainter::CompositionMode_SourceIn);
    qp.fillRect(img.rect(),QColor(color));
    qp.end();
    closeButton = new QLabel(openedContent);
    closeButton->setPixmap(img);
    closeButton->setFixedSize(72, 48);

    QVBoxLayout *openedLayout = new QVBoxLayout(openedContent);
    openedLayout->setSpacing(10);
    openedLayout->setContentsMargins(0, 0, 0, 10);
    openedLayout->addWidget(closeButton);

    for (unsigned int i = 0 ; i < guilds.size() ; i++) {
        Api::Guild actualGuild = *guilds[i];

        GuildWidget *guildWidget = new GuildWidget(rm, *guilds[i], openedContent);
        QObject::connect(guildWidget, &GuildWidget::leftClicked, this, &GuildFolder::propagateGuildClic);
        openedLayout->addWidget(guildWidget);
        guildWidgets.push_back(guildWidget);

        if (i < 4) {
            contentLayout->addWidget(new GuildIcon(rm, actualGuild.id, actualGuild.name, actualGuild.icon, true, closedContent), (i < 2 ? 0 : 1), i%2);
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
        pill->setHeight(8);
    else 
        pill->setHeight(0);

    for (unsigned int i = 0 ; i < guildWidgets.size() ; i++)
        guildWidgets[i]->unclicked();
}

void GuildFolder::unclickedExcept(const Api::Snowflake& id)
{
    clicked = false;
    for (size_t i = 0 ; i < guildWidgets.size() ; i++) {
        if (guildWidgets[i]->id != id) {
            guildWidgets[i]->unclicked();
        } else {
            clicked = true;
        }
    }
    if (!clicked) {
        if (unread)
            pill->setHeight(8);
        else 
            pill->setHeight(0);
    }
}

void GuildFolder::setUnread(const Api::Snowflake& id)
{
    unread = true;
    if (!clicked) 
        pill->setHeight(8);
    
    for (unsigned int i = 0 ; i < guildIds.size() ; i++) {
        if (guildWidgets[i]->id == id)
            guildWidgets[i]->setUnread(true);
    }

}

void GuildFolder::propagateGuildClic(const Api::Snowflake& id)
{
    clicked = true;
    pill->setHeight(40);

    emit guildClicked(id);
}

void GuildFolder::enterEvent(QEvent *)
{
    if (!clicked)
        pill->setHeight(20);
}

void GuildFolder::leaveEvent(QEvent *)
{
    if (clicked)
        pill->setHeight(40);
    else if (unread)
        pill->setHeight(8);
    else
        pill->setHeight(0);
}

} // namespace Ui