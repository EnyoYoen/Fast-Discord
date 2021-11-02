#include "../../include/ui/guildchannelwidget.h"

namespace Ui {

GuildChannelWidget::GuildChannelWidget(const Api::Channel& guildChannel, unsigned int idp, QWidget *parent) : QWidget(parent)
{
    clicked = false;
    id = idp;
    channel = guildChannel;
    setFixedSize(232, 34);

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

    if (channel.type == 4) {
        hoverStyleSheet = clickedStyleSheet = (char *)"color: #FFF;";
    } else {
        hoverStyleSheet = (char *)"color: #DCDDDE;"
                          "border-radius: 4px;"
                          "background-color: #35373D;";
        clickedStyleSheet = (char *)"color: #FFF;"
                            "border-radius: 4px;"
                            "background-color: #393D43;";
    }

    icon = new QLabel(this);
    icon->setPixmap(QPixmap(std::string("res/images/svg/guild-channel-icon" + iconName + ".svg").c_str()));
    icon->setFixedSize(32, 32);
    icon->setStyleSheet("color: #8E9297");

    name = new QLabel((*channel.name).c_str(), this);
    name->setStyleSheet("color: #8E9297");

    layout = new QHBoxLayout(this);
    layout->addWidget(icon, Qt::AlignLeft);
    layout->addWidget(name, Qt::AlignLeft);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);

    setLayout(layout);
}

void GuildChannelWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        emit leftClicked(channel, id);
    } else if (event->button() == Qt::RightButton) {
        emit rightClicked(channel);
    }
}

void GuildChannelWidget::unclicked()
{
    if (clicked) {
        clicked = false;
        setStyleSheet("color: #8E9297;"
                      "background-color: none;");
    }
}

void GuildChannelWidget::mousePressEvent(QMouseEvent *)
{
    if (!clicked && channel.type != 2) {
        setStyleSheet(clickedStyleSheet);
        clicked = true;
    }
}

void GuildChannelWidget::enterEvent(QEvent *)
{
    if (!clicked) {
        setStyleSheet(hoverStyleSheet);
    }
}

void GuildChannelWidget::leaveEvent(QEvent *)
{
    if (!clicked) {
        setStyleSheet("color: #8E9297;"
                      "background-color: none;");
    }
}

} // namespace Ui
