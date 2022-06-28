#include "ui/rightcolumn/channelheader.h"

#include <QLabel>

namespace Ui {


ChannelHeader::ChannelHeader(Api::RessourceManager *rmp, QWidget *parent) 
    : rm(rmp), Widget(parent)
{
    layout = new QHBoxLayout(this);
    layout->setSpacing(8);
    layout->setContentsMargins(16, 0, 0, 0);

    this->setFixedHeight(48);
    this->setBackgroundColor(Settings::BackgroundPrimary);
    this->hide();
}

void ChannelHeader::close()
{
    this->hide();
    layout->removeItem(layout->itemAt(0));
    layout->removeItem(layout->itemAt(0));
    layout->removeItem(layout->itemAt(0));
}

void ChannelHeader::openChannel(const QString& channelName, int channelType)
{
    QLabel *icon = new QLabel(this);
    QString iconName;
    if (channelType == Api::DM) {
        iconName = "at-icon";
    } else if (channelType == Api::GroupDM) {
        iconName = "group-dm-icon";
    } else {
        char number;
        switch (channelType) {
        case Api::GuildText:
            number = '0';
            break;
        case Api::GuildVoice:
            number = '2';
            break;
        case Api::GuildCategory:
            number = '4';
            break;
        case Api::GuildNews:
            number = '5';
            break;
        case Api::GuildStore:
            number = '5';
            break;
        default:
            number = '0';
            break;
        }
        iconName = "guild-channel-icon";
        iconName += number;
    }
    iconName += ".svg";
    icon->setPixmap(QPixmap("res/images/svg/" + iconName));
    icon->setFixedSize(24, 24);

    Label *name = new Label((channelType == Api::GroupDM ? channelName : channelName.mid(1)), nullptr);
    QFont font;
    font.setPixelSize(16);
    font.setFamily("whitney");
    font.setBold(true);
    name->setFont(font);
    name->setTextColor(Settings::TextNormal);

    layout->insertWidget(0, name);
    layout->insertWidget(0, icon);

    this->show();
}

} // namespace Ui