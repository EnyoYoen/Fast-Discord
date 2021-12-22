#include "ui/guildwidget.h"

#include "api/guild.h"

#include <QPixmap>
#include <QBitmap>
#include <QPainter>
#include <QPainterPath>
#include <QString>

#include <fstream>
#include <string>

namespace Ui {

GuildWidget::GuildWidget(Api::RessourceManager *rmp, const Api::Guild& guild, QWidget *parent)
    : QFrame(parent)
{
    // Attributes initialization
    clicked = false;
    unreadMessages = false;
    id = *guild.id;
    rm = rmp;

    // Style the widget
    this->setMouseTracking(true);
    this->setFixedSize(60, 48);

    // Create and style the layout
    layout = new QHBoxLayout(this);
    layout->setSpacing(6);
    layout->setContentsMargins(0, 0, 0, 0);

    // Create the white pill
    pill = new GuildPill(this);
    layout->addWidget(pill);

    QObject::connect(this, SIGNAL(iconRecieved(std::string)), this, SLOT(setIcon(std::string)));

    // Create the guild icon
    std::string *guildIconFileName = guild.icon;
    if (guildIconFileName == nullptr || *guildIconFileName == "") {
        // The guild doesn't have icon : we need to create one with the name

        // Split the name for every space in it

        std::string guildName = *guild.name;
        std::vector<std::string> nameSplit;
        size_t pos = 0;
        // Loop through the name to find spaces
        while ((pos = guildName.find(' ')) != std::string::npos) {
            nameSplit.push_back(guildName.substr(0, pos));
            guildName.erase(0, pos + 1);
        }
        nameSplit.push_back(guildName);


        // The loop that will create the text of the icon

        // Variables creation
        std::string iconText;
        bool firstLetterFound;
        bool nonLetterFound;

        // Loop through the different words (word = 1 split of the name)
        for (size_t i = 0 ; i < nameSplit.size() ; i++) {
            // Variables
            std::string actualSplit = nameSplit[i]; // The actual word
            std::string firstLetter;
            firstLetterFound = false;
            nonLetterFound = false;

            // Loop through the word for every caracter
            for (size_t j = 0 ; j < actualSplit.length() ; j++) {
                if (!(actualSplit[j] >= 'a' && actualSplit[j] <= 'z')
                        && !(actualSplit[j] >= 'A' && actualSplit[j] <= 'Z')
                        && actualSplit[j] != '\'') {
                    // It is a special caracter

                    // We add the first letter of the word (if there is one)
                    // with the current 'special' caracter
                    iconText += firstLetter + actualSplit[j];

                    // We insert a new split for the next turn if we are not
                    // at the end and if this is not the only caracter
                    if (j != actualSplit.length() && actualSplit.length() > 1) {
                        nameSplit.insert(nameSplit.begin() + i + 1, actualSplit.substr(j + 1, actualSplit.length()));
                    }

                    nonLetterFound = true;
                    break;
                } else if (!firstLetterFound) {
                    // This is a letter and it is the first of the word
                    firstLetter = actualSplit[j];
                    firstLetterFound = true;
                }
            }

            // Add the first letter if there was no special caracter
            if (!nonLetterFound) iconText += firstLetter;
        }

        // Create and add the text icon

        textIcon = new QLabel(this);
        QHBoxLayout *iconTextLayout = new QHBoxLayout(textIcon);
        QLabel *text = new QLabel(iconText.c_str(), textIcon);

        iconTextLayout->setContentsMargins(0, 0, 0, 0);
        iconTextLayout->addWidget(text, 0, Qt::AlignHCenter);

        text->setFixedHeight(40);
        textIcon->setFixedSize(48, 48);
        textIcon->setStyleSheet("border-radius: 24px;"
                                "color: #DCDDDE;"
                                "background-color: #36393F;");
        layout->addWidget(textIcon);
        icon = nullptr;

    } else {
        // This guild has an icon

        // Request the icon and cache it
        *guildIconFileName += ".png";
        if (!std::ifstream(("cache/" + *guildIconFileName).c_str()).good()) {
            rm->getImage([this](void *iconFileName) {emit iconRecieved(*static_cast<std::string *>(iconFileName));}, "https://cdn.discordapp.com/icons/" + *guild.id + "/" + *guildIconFileName, *guildIconFileName);
        } else {
            *guildIconFileName = "cache/" + *guildIconFileName;

            // Create the icon and add it to the layout
            icon = new RoundedImage(*guildIconFileName, 48, 48, 24, this);
            layout->addWidget(icon);
        }
    }
}

void GuildWidget::setIcon(const std::string& guildIconFileName)
{
    icon = new RoundedImage(guildIconFileName, 48, 48, 24, this);
    layout->addWidget(icon);
}

void GuildWidget::mouseReleaseEvent(QMouseEvent *event)
{
    // Emit signals when clicked to open the channel or to show infos
    if (event->button() == Qt::LeftButton) {
        emit leftClicked(id);
    } else if (event->button() == Qt::RightButton) {
        emit rightClicked(id); // Does nothing for now
    }
}

void GuildWidget::unclicked()
{
    // Reset the stylesheet of this widget if currently clicked
    if (clicked && !icon) {
        clicked = false;
        textIcon->setStyleSheet("border-radius: 24px; color: #DCDDDE; background-color: #36393F;");
    }

    if (unreadMessages) {
        pill->setHeight(8);
    } else {
        pill->setHeight(0);
    }
}

void GuildWidget::setUnread(bool unread)
{
    unreadMessages = unread;
    if (unread) {
        pill->setHeight(8);
    } else {
        pill->setHeight(0);
    }
}

void GuildWidget::mousePressEvent(QMouseEvent *)
{
    // Widget clicked : change the stylesheet
    if (!clicked && !icon) {
        textIcon->setStyleSheet("background-color: #5865F2; color: #FFF; border-radius: 16px;");
        clicked = true;
    }
    pill->setHeight(40);
}

void GuildWidget::enterEvent(QEvent *)
{
    // Mouse hover : change the stylesheet
    if (!clicked && !icon) {
        textIcon->setStyleSheet("background-color: #5865F2; color: #FFF; border-radius: 16px;");
    }
    if (pill->height() != 40) pill->setHeight(20);
}

void GuildWidget::leaveEvent(QEvent *)
{
    // Reset the stylesheet if not clicked
    if (!clicked && !icon) {
        textIcon->setStyleSheet("border-radius: 24px; color: #DCDDDE; background-color: #36393F;");
    }
    if (pill->height() != 40) {
        if (unreadMessages) {
            pill->setHeight(8);
        } else {
            pill->setHeight(0);
        }
    }
}

} // namespace Ui
