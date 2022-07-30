#include "ui/leftcolumn/guildwidget.h"

#include "api/objects/guild.h"

#include <QPixmap>
#include <QBitmap>
#include <QPainter>
#include <QPainterPath>
#include <QString>

namespace Ui {

GuildWidget::GuildWidget(Api::RessourceManager *rmp, const Api::Guild& guild, QWidget *parent)
    : Widget(parent)
{
    // Attributes initialization
    clicked = false;
    unreadMessages = false;
    id = guild.id;
    rm = rmp;

    // Style the widget
    this->setMouseTracking(true);
    this->setFixedSize(Settings::scale(72), Settings::scale(48));

    // Create and style the layout
    layout = new QHBoxLayout(this);
    layout->setSpacing(Settings::scale(8));
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
        pill->setFixedHeight(Settings::scale(8));
    } else {
        pill->setFixedHeight(0);
    }
}

void GuildWidget::setUnread(bool unread)
{
    unreadMessages = unread;
    if (pill->height() <= Settings::scale(8)) {
        if (unread) {
            pill->setFixedHeight(Settings::scale(8));
        } else {
            pill->setFixedHeight(0);
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
    pill->setFixedHeight(Settings::scale(40));
}

void GuildWidget::enterEvent(QEvent *)
{
    // Mouse hover : change the stylesheet
    if (!clicked) {
        icon->setActive();
    }
    if (pill->height() != Settings::scale(20)) pill->setFixedHeight(Settings::scale(20));
}

void GuildWidget::leaveEvent(QEvent *)
{
    // Reset the stylesheet if not clicked
    if (!clicked) {
        icon->setInactive();
    }
    if (pill->height() != Settings::scale(40)) {
        if (unreadMessages) {
            pill->setFixedHeight(Settings::scale(8));
        } else {
            pill->setFixedHeight(0);
        }
    }
}

} // namespace Ui
