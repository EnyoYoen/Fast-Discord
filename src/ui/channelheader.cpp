#include "ui/channelheader.h"

namespace Ui {


ChannelHeader::ChannelHeader(Api::RessourceManager *rmp, QWidget *parent) 
    : rm(rmp), QWidget(parent)
{
    layout = new QHBoxLayout(this);
    layout->setSpacing(8);
    layout->setContentsMargins(16, 0, 0, 0);

    this->setFixedHeight(48);
    this->setStyleSheet("background-color: #36393F;");
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

    QLabel *name = new QLabel((channelType == Api::GroupDM ? channelName : channelName.mid(1)));
    QFont font;
    font.setPixelSize(16);
    font.setFamily("whitney");
    font.setBold(true);
    name->setFont(font);
    name->setStyleSheet("color: #DCDDDE;");

    layout->insertWidget(0, name);
    layout->insertWidget(0, icon);

    this->show();
}

} // namespace Ui