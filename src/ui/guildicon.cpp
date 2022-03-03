#include "ui/guildicon.h"

#include <QHBoxLayout>

#include <fstream>

namespace Ui {

GuildIcon::GuildIcon(Api::RessourceManager *rm, const std::string& guildId, std::string guildName, std::string *guildIcon, bool smallp, QWidget *parent)
    : QWidget(parent)
{
    small = smallp;
    QObject::connect(this, SIGNAL(iconRecieved(std::string)), this, SLOT(setIcon(std::string)));

    // Create the guild icon
    if (guildIcon == nullptr || *guildIcon == "") {
        // The guild doesn't have icon : we need to create one with the name

        // Split the name for every space in it

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

        text->setFixedHeight(small ? 10 : 40);
        textIcon->setFixedSize(small ? 16 : 48, small ? 16 : 48);
        textIcon->setStyleSheet(("border-radius: " + std::to_string(small ? 8 : 24) + "px;"
                                 "color: #DCDDDE;"
                                 + (small ? "background-color: #5865F2;" : "background-color: #36393F;")).c_str());
        icon = nullptr;

    } else {
        // This guild has an icon

        // Request the icon and cache it
        std::string guildIconFileName(*guildIcon);
        guildIconFileName += ".png";
        if (!std::ifstream(("cache/" + guildIconFileName).c_str()).good()) {
            icon = new RoundedImage(small ? 16 : 48, small ? 16 : 48, small ? 8 : 24, this);
            rm->getImage([this](void *iconFileName) {emit iconRecieved(*static_cast<std::string *>(iconFileName));}, "https://cdn.discordapp.com/icons/" + guildId + "/" + guildIconFileName, guildIconFileName);
        } else {
            guildIconFileName = "cache/" + guildIconFileName;

            // Create the icon and add it to the layout
            icon = new RoundedImage(guildIconFileName, small ? 16 : 48, small ? 16 : 48, small ? 8 : 24, this);
        }
        icon->setStyleSheet("background-color: none;");
    }
}

void GuildIcon::setIcon(const std::string& guildIconFileName)
{
    icon->setImage(guildIconFileName);
}

void GuildIcon::setActive()
{
    if (!icon)
        textIcon->setStyleSheet(("border-radius: " + std::to_string(small ? 6 : 16) + "px;"
                                 "color: #FFF;"
                                 "background-color: #5865F2;").c_str());
}

void GuildIcon::setInactive()
{
    if (!icon)
        textIcon->setStyleSheet(("border-radius: " + std::to_string(small ? 8 : 24) + "px;"
                                 "color: #DCDDDE;"
                                 "background-color: #36393F;").c_str());
}

}