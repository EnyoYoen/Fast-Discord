#pragma once

#include "ui/rightcolumn/memberwidget.h" 

namespace Ui {

MemberWidget::MemberWidget(Api::RessourceManager *rmp, Api::User *userp, QColor color, QWidget *parent)
    : Widget(parent)
{
    rm = rmp;
    user = userp;
    statusIcon = nullptr;
    statusBackground = nullptr;
    
    subtext = new Label(this);
    subtext->move(Settings::scale(52), Settings::scale(24));
    QFont font;
    font.setPixelSize(Settings::scale(13));
    font.setFamily("whitney");
    subtext->setFont(font);
    subtext->setTextColor(Settings::ChannelsDefault);
    
    QString avatar = user->avatar;
    if (avatar.isNull()) {
        QString channelIconFileName = "res/images/png/user-icon-asset0.png";
        icon = new RoundedImage(channelIconFileName, Settings::scale(32), Settings::scale(32), Settings::scale(16), this);
        icon->move(Settings::scale(8), Settings::scale(6));
    } else {
        icon = new RoundedImage(Settings::scale(32), Settings::scale(32), Settings::scale(16), this);
        QString channelIconFileName = user->id + (avatar.indexOf("a_") == 0 ? ".gif" : ".png");
        rm->getImage([this](void *iconFileName) {this->setIcon(*static_cast<QString *>(iconFileName));}, "https://cdn.discordapp.com/avatars/" + user->id + "/" + avatar, channelIconFileName);
    }

    statusBackground = new Widget(this);
    statusBackground->setFixedSize(Settings::scale(16), Settings::scale(16));
    statusBackground->move(Settings::scale(28), Settings::scale(24));
    statusBackground->setBorderRadius(Settings::scale(8));
    statusBackground->setBackgroundColor(Settings::BackgroundSecondary);

    statusIcon = new StatusIcon(this);
    statusIcon->move(Settings::scale(31), Settings::scale(27));

    name = new Label(user->username, this);
    font.setPixelSize(Settings::scale(15));
    name->setFont(font);
    if (color.isValid())
        name->setTextColor(color);
    else
        name->setTextColor(Settings::ChannelsDefault);
    name->move(Settings::scale(52), (subtext->text.isEmpty() ? Settings::scale(16) : Settings::scale(8)));

    name->setFixedSize(Settings::scale(145), Settings::scale(20));
    subtext->setFixedSize(Settings::scale(145), 0);

    mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    this->setFixedSize(Settings::scale(224), Settings::scale(44));
    this->setBorderRadius(Settings::scale(4));
}

void MemberWidget::setStatus(const QString& status)
{
    statusIcon->setStatus(status);
}

void MemberWidget::setIcon(const QString& iconFileName)
{
    icon->setImage(iconFileName);
    icon->move(Settings::scale(8), Settings::scale(6));
}

void MemberWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        emit leftClicked(user);
    } else if (event->button() == Qt::RightButton) {
        emit rightClicked(user);
    }

    if (statusBackground != nullptr) {
        statusBackground->setBackgroundColor(Settings::StatusBackgroundHover);
    }
    this->setBackgroundColor(Settings::BackgroundModifierHover);
    name->setTextColor(Settings::InteractiveHover);
    subtext->setTextColor(Settings::InteractiveHover);
}

void MemberWidget::mousePressEvent(QMouseEvent *)
{
    if (statusBackground != nullptr) {
        statusBackground->setBackgroundColor(Settings::StatusBackgroundActive);
    }
    this->setBackgroundColor(Settings::BackgroundModifierActive);
    name->setTextColor(Settings::InteractiveActive);
    subtext->setTextColor(Settings::InteractiveActive);
}

void MemberWidget::enterEvent(QEvent *)
{
    if (statusBackground != nullptr) {
        statusBackground->setBackgroundColor(Settings::StatusBackgroundHover);
    }
    this->setBackgroundColor(Settings::BackgroundModifierHover);
    name->setTextColor(Settings::InteractiveHover);
    subtext->setTextColor(Settings::InteractiveHover);
}

void MemberWidget::leaveEvent(QEvent *)
{
    if (statusBackground != nullptr) {
        statusBackground->setBackgroundColor(Settings::BackgroundSecondary);
    }
    this->setBackgroundColor(Settings::BackgroundSecondary);
    name->setTextColor(Settings::ChannelsDefault);
    subtext->setTextColor(Settings::ChannelsDefault);
}

} // namespace Ui