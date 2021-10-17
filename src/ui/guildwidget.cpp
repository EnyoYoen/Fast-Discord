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

GuildWidget::GuildWidget(const Api::Guild& guildP, QWidget *parent) : QFrame(parent)
{
    guild = Api::Guild(guildP);

    //this->setProperty("guild", true);
    setFixedSize(48, 48);

    layout = new QVBoxLayout();

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

        textIcon = new QLabel(iconText.c_str());
        layout->addWidget(textIcon);

    } else {
        *guildIconFileName += ".png";
        if (!std::ifstream(("cache/" + *guildIconFileName).c_str()).good()) {
            Api::Request::requestFile("https://cdn.discordapp.com/icons/" + *guild.id + "/" + *guildIconFileName, "cache/" + *guildIconFileName);
        }
        *guildIconFileName = "cache/" + *guildIconFileName;

        icon = new RoundedImage(*guildIconFileName, 48, 48, 24);
        layout->addWidget(icon);
    }

    layout->setSpacing(0);
    setLayout(layout);

    setStyleSheet("border-radius: 24px;"
                  "color: #DCDDDE;"
                  "background-color: #36393F");
}

void GuildWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        emit leftClicked(guild);
    } else if (event->button() == Qt::RightButton) {
        emit rightClicked(guild);
    }
}

} // namespace Ui
