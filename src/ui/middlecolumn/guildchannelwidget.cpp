#include "ui/middlecolumn/guildchannelwidget.h"

#include <QPainter>

namespace Ui {

GuildChannelWidget::GuildChannelWidget(const Api::Channel& guildChannel, QWidget *parent)
    : Widget(parent)
{
    // Attributes initialization
    id = guildChannel.id;
    type = guildChannel.type;
    clicked = false;

    QFont font;
    font.setFamily("whitney");

    container = new Widget(this);
    container->setBorderRadius(4);

    //  Determine the icon of this channel with its type
    QString iconNumber;
    switch (type) {
    case Api::GuildText:
        iconNumber = "0";
        break;
    case Api::GuildVoice:
        iconNumber = "2";
        break;
    case Api::GuildCategory:
        iconNumber = "4";
        break;
    case Api::GuildNews:
        iconNumber = "5";
        break;
    case Api::GuildStore:
        iconNumber = "5";
        break;
    default:
        iconNumber = "0";
        break;
    }
    iconName = "res/images/svg/guild-channel-icon" + iconNumber + ".svg";

    if (type == Api::GuildCategory) {
        hoverColor = clickedColor = Settings::None;
        font.setPixelSize(11);
        font.setBold(true);
        this->setFixedSize(224, 36);
        this->setContentsMargins(0, 18, 0, 0);
    } else {
        hoverColor = Settings::BackgroundModifierHover;
        clickedColor = Settings::BackgroundModifierActive;
        font.setPixelSize(16);
        this->setFixedSize(224, 32);
    }

    QHBoxLayout *layout = new QHBoxLayout(container);

    // Create the icon
    icon = new Widget(container);
    icon->setImage(iconName);
    if (type == Api::GuildCategory)
        icon->setFixedSize(16, 16);
    else
        icon->setFixedSize(26, 26);

    // Create the name label
    name = new Label((type == Api::GuildCategory ? guildChannel.name.toUpper() : guildChannel.name), container);
    name->setFont(font);
    name->setTextColor(Settings::ChannelIcon);

    // Add widgets to layout and style it
    layout->addWidget(icon, Qt::AlignLeft);
    layout->addWidget(name, Qt::AlignLeft);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);

    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins((type == Api::GuildCategory ? 0 : 16), 0, 0, 0);
    mainLayout->addWidget(container);
}

void GuildChannelWidget::mouseReleaseEvent(QMouseEvent *event)
{
    // Emit signals when clicked to open the channel or to show infos
    if (event->button() == Qt::LeftButton) {
        emit leftClicked(id);
    } else if (event->button() == Qt::RightButton) {
        emit rightClicked(id); // Does nothing for now
    }
}

void GuildChannelWidget::unclicked()
{
    // Reset the stylesheet of this widget if currently clicked
    if (clicked) {
        clicked = false;

        if (type == Api::GuildCategory) {
            QPixmap img(iconName);
            QPainter qp(&img);
            qp.setCompositionMode(QPainter::CompositionMode_SourceIn);
            qp.fillRect(img.rect(), Settings::colors[Settings::ChannelIcon]);
            qp.end();
            icon->setPixmap(img);
        }

        container->setBackgroundColor(Settings::None);
        name->setTextColor(Settings::ChannelIcon);
    }
}

void GuildChannelWidget::mousePressEvent(QMouseEvent *)
{
    if (!clicked && type != Api::GuildVoice && type != Api::GuildCategory) {
        container->setBackgroundColor(clickedColor);
        name->setTextColor(Settings::InteractiveActive);
        clicked = true;
    }
}

void GuildChannelWidget::enterEvent(QEvent *)
{
    if (!clicked) {
        if (type == Api::GuildCategory) {
            QPixmap img(iconName);
            QPainter qp(&img);
            qp.setCompositionMode(QPainter::CompositionMode_SourceIn);
            qp.fillRect(img.rect(), Settings::colors[Settings::InteractiveHover]);
            qp.end();
            icon->setPixmap(img);
        }

        container->setBackgroundColor(hoverColor);
        name->setTextColor(Settings::InteractiveHover);
    }
}

void GuildChannelWidget::leaveEvent(QEvent *)
{
    if (!clicked) {
        if (type == Api::GuildCategory) {
            QPixmap img(iconName);
            QPainter qp(&img);
            qp.setCompositionMode(QPainter::CompositionMode_SourceIn);
            qp.fillRect(img.rect(), Settings::colors[Settings::ChannelIcon]);
            qp.end();
            icon->setPixmap(img);
        }

        container->setBackgroundColor(Settings::None);
        name->setTextColor(Settings::ChannelIcon);
    }
}

} // namespace Ui
