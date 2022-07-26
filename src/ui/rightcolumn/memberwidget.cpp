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
    subtext->move(52, 24);
    QFont font;
    font.setPixelSize(13);
    font.setFamily("whitney");
    subtext->setFont(font);
    subtext->setTextColor(Settings::ChannelsDefault);
    
    QString avatar = user->avatar;
    if (avatar.isNull()) {
        QString channelIconFileName = "res/images/png/user-icon-asset0.png";
        icon = new RoundedImage(channelIconFileName, 32, 32, 16, this);
        icon->move(8, 6);
    } else {
        icon = new RoundedImage(32, 32, 16, this);
        QString channelIconFileName = user->id + (avatar.indexOf("a_") == 0 ? ".gif" : ".png");
        rm->getImage([this](void *iconFileName) {this->setIcon(*static_cast<QString *>(iconFileName));}, "https://cdn.discordapp.com/avatars/" + user->id + "/" + avatar, channelIconFileName);
    }

    statusBackground = new Widget(this);
    statusBackground->setFixedSize(16, 16);
    statusBackground->move(28, 24);
    statusBackground->setBorderRadius(8);
    statusBackground->setBackgroundColor(Settings::BackgroundSecondary);

    statusIcon = new StatusIcon(this);
    statusIcon->move(31, 27);

    name = new Label(user->username, this);
    font.setPixelSize(15);
    name->setFont(font);
    if (color.isValid())
        name->setTextColor(color);
    else
        name->setTextColor(Settings::ChannelsDefault);
    name->move(52, (subtext->text.isEmpty() ? 16 : 8));

    name->setFixedSize(145, 20);
    subtext->setFixedSize(145, 0);

    mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    this->setFixedSize(224, 44);
    this->setBorderRadius(4);
}

void MemberWidget::setStatus(const QString& status)
{
    statusIcon->setStatus(status);
}

void MemberWidget::setIcon(const QString& iconFileName)
{
    icon->setImage(iconFileName);
    icon->move(8, 6);
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