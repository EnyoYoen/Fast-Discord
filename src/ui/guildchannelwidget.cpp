#include "ui/guildchannelwidget.h"

namespace Ui {

GuildChannelWidget::GuildChannelWidget(const Api::Channel& guildChannel, unsigned int idp, QWidget *parent)
    : QWidget(parent)
{
    // Attributes initialization
    id = idp;
    channel = guildChannel;
    clicked = false;

    //  Determine the icon of this channel with its type
    std::string iconName;
    switch (channel.type) {
    case Api::GuildText:
        iconName = "0";
        break;
    case Api::GuildVoice:
        iconName = "2";
        break;
    case Api::GuildCategory:
        iconName = "4";
        break;
    case Api::GuildNews:
        iconName = "5";
        break;
    case Api::GuildStore:
        iconName = "5";
        break;
    default:
        iconName = "0";
        break;
    }

    // Set the different stylesheets
    if (channel.type == Api::GuildCategory) {
        // It is a category, there is no stylesheet
        hoverStyleSheet = clickedStyleSheet = (char *)"color: #FFF;";
    } else {
        hoverStyleSheet = (char *)"color: #DCDDDE;"
                          "border-radius: 4px;"
                          "background-color: #35373D;";
        clickedStyleSheet = (char *)"color: #FFF;"
                            "border-radius: 4px;"
                            "background-color: #393D43;";
    }

    // Create the icon
    icon = new QLabel(this);
    icon->setPixmap(QPixmap(std::string("res/images/svg/guild-channel-icon" + iconName + ".svg").c_str()));
    icon->setFixedSize(32, 32);
    icon->setStyleSheet("color: #8E9297");

    // Create the name label
    name = new QLabel((*channel.name).c_str(), this);
    name->setStyleSheet("color: #8E9297");

    // Add widgets to layout and style it
    layout = new QHBoxLayout(this);
    layout->addWidget(icon, Qt::AlignLeft);
    layout->addWidget(name, Qt::AlignLeft);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);

    // Set the size of this widget
    this->setFixedSize(232, 34);
}

void GuildChannelWidget::mouseReleaseEvent(QMouseEvent *event)
{
    // Emit signals when clicked to open the channel or to show infos
    if (event->button() == Qt::LeftButton) {
        emit leftClicked(channel, id);
    } else if (event->button() == Qt::RightButton) {
        emit rightClicked(channel); // Does nothing for now
    }
}

void GuildChannelWidget::unclicked()
{
    // Reset the stylesheet of this widget if currently clicked
    if (clicked) {
        clicked = false;
        this->setStyleSheet("color: #8E9297;"
                      "background-color: none;");
    }
}

void GuildChannelWidget::mousePressEvent(QMouseEvent *)
{
    // Widget clicked : change the stylesheet
    if (!clicked && channel.type != 2) {
        this->setStyleSheet(clickedStyleSheet);
        clicked = true;
    }
}

void GuildChannelWidget::enterEvent(QEvent *)
{
    // Mouse hover : change the stylesheet
    if (!clicked) {
        this->setStyleSheet(hoverStyleSheet);
    }
}

void GuildChannelWidget::leaveEvent(QEvent *)
{
    // Reset the stylesheet if not clicked
    if (!clicked) {
        this->setStyleSheet("color: #8E9297;"
                            "background-color: none;");
    }
}

} // namespace Ui
