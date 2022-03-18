#include "ui/guildwidget.h"

#include "api/objects/guild.h"

#include <QPixmap>
#include <QBitmap>
#include <QPainter>
#include <QPainterPath>
#include <QString>

#include <fstream>

namespace Ui {

GuildWidget::GuildWidget(Api::RessourceManager *rmp, const Api::Guild& guild, QWidget *parent)
    : QFrame(parent)
{
    // Attributes initialization
    clicked = false;
    unreadMessages = false;
    id = guild.id;
    rm = rmp;

    // Style the widget
    this->setMouseTracking(true);
    this->setFixedSize(60, 48);
    this->setStyleSheet("background-color: none;");

    // Create and style the layout
    layout = new QHBoxLayout(this);
    layout->setSpacing(6);
    layout->setContentsMargins(0, 0, 0, 0);

    // Create the white pill
    pill = new GuildPill(this);
    layout->addWidget(pill);
    
    icon = new GuildIcon(rm, guild.id, guild.name, guild.icon, false, this);
    layout->addWidget(icon);
}

void GuildWidget::mouseReleaseEvent(QMouseEvent *event)
{
    // Emit signals when clicked to open the channel or to show infos
    if (event->button() == Qt::LeftButton) {
        emit leftClicked(id);
    } else if (event->button() == Qt::RightButton) {
        emit rightClicked(id); // Does nothing for now
    }
}

void GuildWidget::unclicked()
{
    // Reset the stylesheet of this widget if currently clicked
    if (clicked) {
        clicked = false;
        icon->setInactive();
    }

    if (unreadMessages) {
        pill->setHeight(8);
    } else {
        pill->setHeight(0);
    }
}

void GuildWidget::setUnread(bool unread)
{
    unreadMessages = unread;
    if (pill->height() <= 8) {
        if (unread) {
            pill->setHeight(8);
        } else {
            pill->setHeight(0);
        }
    }
}

void GuildWidget::mousePressEvent(QMouseEvent *)
{
    // Widget clicked : change the stylesheet
    if (!clicked) {
        icon->setActive();
        clicked = true;
    }
    pill->setHeight(40);
}

void GuildWidget::enterEvent(QEvent *)
{
    // Mouse hover : change the stylesheet
    if (!clicked) {
        icon->setActive();
    }
    if (pill->height() != 40) pill->setHeight(20);
}

void GuildWidget::leaveEvent(QEvent *)
{
    // Reset the stylesheet if not clicked
    if (!clicked) {
        icon->setInactive();
    }
    if (pill->height() != 40) {
        if (unreadMessages) {
            pill->setHeight(8);
        } else {
            pill->setHeight(0);
        }
    }
}

} // namespace Ui
