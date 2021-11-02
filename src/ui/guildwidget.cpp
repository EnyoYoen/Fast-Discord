#include "../../include/ui/guildwidget.h"

#include "../../include/api/request.h"
#include "../../include/api/guild.h"

#include <QPixmap>
#include <QBitmap>
#include <QPainter>
#include <QPainterPath>
#include <QString>

#include <fstream>
#include <string>

namespace Ui {

GuildWidget::GuildWidget(const Api::Guild& guildP, unsigned int idp, QWidget *parent) : QFrame(parent)
{
    clicked = false;
    id = idp;
    guild = Api::Guild(guildP);

    setMouseTracking(true);
    setFixedSize(48, 48);

    QVBoxLayout *layout = new QVBoxLayout();
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setAlignment(Qt::AlignCenter);
    setLayout(layout);

    std::string *guildIconFileName = guild.icon;
    if (guildIconFileName == nullptr) {
        std::string guildName = *guild.name;
        std::vector<std::string> nameSplit;

        size_t pos = 0;
        while ((pos = guildName.find(' ')) != std::string::npos) {
            nameSplit.push_back(guildName.substr(0, pos));
            guildName.erase(0, pos + 1);
        }
        nameSplit.push_back(guildName);

        std::string iconText;
        bool firstLetterFound;
        bool nonLetterFound;
        for (size_t i = 0 ; i < nameSplit.size() ; i++) {
            std::string actualSplit = nameSplit[i];
            std::string firstLetter;
            firstLetterFound = false;
            nonLetterFound = false;

            for (size_t j = 0 ; j < actualSplit.length() ; j++) {
                if (!(actualSplit[j] > 'a' && actualSplit[j] < 'z') && !(actualSplit[j] > 'A' && actualSplit[j] < 'Z')) {
                    iconText += firstLetter + actualSplit[j];
                    if (j != actualSplit.length()) nameSplit.insert(nameSplit.begin() + i + 1, actualSplit.substr(j + 1, actualSplit.length()));
                    i--;
                    nonLetterFound = true;
                    break;
                } else if (!firstLetterFound) {
                    firstLetter = actualSplit[j];
                    firstLetterFound = true;
                }
            }

            if (!nonLetterFound) iconText += firstLetter;
        }

        layout->addWidget(new QLabel(iconText.c_str()));
        icon = nullptr;

        setStyleSheet("border-radius: 24px;"
                      "color: #DCDDDE;"
                      "background-color: #36393F;");
    } else {
        *guildIconFileName += ".png";
        if (!std::ifstream(("cache/" + *guildIconFileName).c_str()).good()) {
            Api::Request::requestFile("https://cdn.discordapp.com/icons/" + *guild.id + "/" + *guildIconFileName, "cache/" + *guildIconFileName);
        }
        *guildIconFileName = "cache/" + *guildIconFileName;

        icon = new RoundedImage(*guildIconFileName, 48, 48, 24);
        layout->addWidget(icon);
    }
}

void GuildWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        emit leftClicked(guild, id);
    } else if (event->button() == Qt::RightButton) {
        emit rightClicked(guild);
    }
}

void GuildWidget::unclicked()
{
    if (clicked && !icon) {
        clicked = false;
        setStyleSheet("border-radius: 24px; color: #DCDDDE; background-color: #36393F;");
    }
}

void GuildWidget::mousePressEvent(QMouseEvent *)
{
    if (!clicked && !icon) {
        setStyleSheet("background-color: #5865F2; color: #FFF; border-radius: 16px;");
        clicked = true;
    }
}

void GuildWidget::enterEvent(QEvent *)
{
    if (!clicked && !icon) {
        setStyleSheet("background-color: #5865F2; color: #FFF; border-radius: 16px;");
    }
}

void GuildWidget::leaveEvent(QEvent *)
{
    if (!clicked && !icon) {
        setStyleSheet("border-radius: 24px; color: #DCDDDE; background-color: #36393F;");
    }
}

} // namespace Ui
