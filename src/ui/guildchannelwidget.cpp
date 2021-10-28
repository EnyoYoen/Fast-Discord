#include "../../include/ui/guildchannelwidget.h"

namespace Ui {

GuildChannelWidget::GuildChannelWidget(const Api::Channel& guildChannel, QWidget *parent) : QWidget(parent)
{
    setFixedSize(232, 34);

    channel = guildChannel;
    std::string iconName;
    switch (channel.type) {
    case 0:
        iconName = "1";
        break;
    case 2:
        iconName = "2";
        break;
    case 4:
        iconName = "7";
        break;
    case 5:
        iconName = "4";
        break;
    case 6:
        iconName = "5";
        break;
    default:
        iconName = "1";
        break;
    }

    icon = new QLabel();
    icon->setPixmap(QPixmap(std::string("res/images/svg/guild-channel-icon" + iconName + ".svg").c_str()));
    icon->setFixedSize(32, 32);
    icon->setStyleSheet("color: #8E9297");

    name = new QLabel((*channel.name).c_str());
    name->setStyleSheet("color: #8E9297");

    layout = new QHBoxLayout();
    layout->addWidget(icon, Qt::AlignLeft);
    layout->addWidget(name, Qt::AlignLeft);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);

    setLayout(layout);
}

void GuildChannelWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        emit leftClicked(channel);
    } else if (event->button() == Qt::RightButton) {
        emit rightClicked(channel);
    }
}

} // namespace Ui
