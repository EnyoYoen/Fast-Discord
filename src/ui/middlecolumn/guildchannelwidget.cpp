#include "ui/middlecolumn/guildchannelwidget.h"

#include <QPainter>

namespace Ui {

GuildChannelWidget::GuildChannelWidget(const Api::Channel& guildChannel, QWidget *parent)
    : QWidget(parent)
{
    // Attributes initialization
    id = guildChannel.id;
    type = guildChannel.type;
    clicked = false;

    QFont font;
    font.setFamily("whitney");

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

    // Set the different stylesheets
    if (type == Api::GuildCategory) {
        // It is a category, there is no stylesheet
        hoverStyleSheet = clickedStyleSheet = (char *)"color: #FFF;";
        font.setPixelSize(11);
        font.setBold(true);
        this->setFixedSize(224, 36);
        this->setContentsMargins(0, 18, 0, 0);
    } else {
        hoverStyleSheet = (char *)"color: #DCDDDE;"
                          "border-radius: 4px;"
                          "background-color: #35373D;";
        clickedStyleSheet = (char *)"color: #FFF;"
                            "border-radius: 4px;"
                            "background-color: #393D43;";
        font.setPixelSize(16);
        this->setFixedSize(224, 32);
    }

    // I have to create an other widget otherwise
    // the background color is not applied everywhere, I don't know why
    QWidget *container = new QWidget(this);
    QHBoxLayout *layout = new QHBoxLayout(container);

    // Create the icon
    icon = new QLabel(this);
    icon->setPixmap(iconName);
    if (type == Api::GuildCategory)
        icon->setFixedSize(16, 16);
    else
        icon->setFixedSize(26, 26);
    icon->setStyleSheet("color: #8E9297");

    // Create the name label
    name = new QLabel((type == Api::GuildCategory ? guildChannel.name.toUpper() : guildChannel.name), this);
    name->setFont(font);
    name->setStyleSheet("color: #8E9297");

    // Add widgets to layout and style it
    layout->addWidget(icon, Qt::AlignLeft);
    layout->addWidget(name, Qt::AlignLeft);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);

    // Create the main layout and add the container
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->addWidget(container);
    mainLayout->setContentsMargins((type == Api::GuildCategory ? 0 : 16), 0, 0, 0);
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
        QPixmap img(iconName);
        QPainter qp(&img);
        qp.setCompositionMode(QPainter::CompositionMode_SourceIn);
        qp.fillRect(img.rect(),QColor(142, 146, 151));
        qp.end();
        this->icon->setPixmap(img);
        this->setStyleSheet("color: #8E9297;"
                            "background-color: none;");
        this->name->setStyleSheet("color: #8E9297;");
    }
}

void GuildChannelWidget::mousePressEvent(QMouseEvent *)
{
    // Widget clicked : change the stylesheet
    if (!clicked && type != Api::GuildVoice && type != Api::GuildCategory) {
        this->setStyleSheet(clickedStyleSheet);
        this->name->setStyleSheet("color: #FFF;");
        clicked = true;
    }
}

void GuildChannelWidget::enterEvent(QEvent *)
{
    // Mouse hover : change the stylesheet
    if (!clicked) {
        QPixmap img(iconName);
        QPainter qp(&img);
        qp.setCompositionMode(QPainter::CompositionMode_SourceIn);
        qp.fillRect(img.rect(),QColor(220, 221, 222));
        qp.end();
        this->icon->setPixmap(img);

        this->setStyleSheet(hoverStyleSheet);
        this->name->setStyleSheet("color: #DCDDDE;");
    }
}

void GuildChannelWidget::leaveEvent(QEvent *)
{
    // Reset the stylesheet if not clicked
    if (!clicked) {
        QPixmap img(iconName);
        QPainter qp(&img);
        qp.setCompositionMode(QPainter::CompositionMode_SourceIn);
        qp.fillRect(img.rect(),QColor(142, 146, 151));
        qp.end();
        this->icon->setPixmap(img);

        this->setStyleSheet("color: #8E9297;"
                            "background-color: none;");
        this->name->setStyleSheet("color: #8E9297;");
    }
}

} // namespace Ui
