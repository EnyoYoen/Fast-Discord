#include "ui/leftcolumn/guildicon.h"

#include <QHBoxLayout>

namespace Ui {

GuildIcon::GuildIcon(Api::RessourceManager *rm, const Api::Snowflake& guildId, QString guildName, const QString& guildIcon, bool smallp, QWidget *parent)
    : QWidget(parent)
{
    small = smallp;
    QObject::connect(this, &GuildIcon::iconRecieved, this, &GuildIcon::setIcon);

    // Create the guild icon
    if (guildIcon.isNull()) {
        // The guild doesn't have icon : we need to create one with the name

        // Split the name for every space in it
        QStringList nameSplit(guildName.split(' '));

        // The loop that will create the text of the icon

        // Variables creation
        QString iconText;
        bool firstLetterFound;
        bool nonLetterFound;

        // Loop through the different words (word = 1 split of the name)
        for (size_t i = 0 ; i < nameSplit.size() ; i++) {
            // Variables
            QString actualSplit = nameSplit[i]; // The actual word
            QString firstLetter;
            firstLetterFound = false;
            nonLetterFound = false;

            // Loop through the word for every caracter
            for (uint j = 0 ; j < actualSplit.length() ; j++) {
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
                        nameSplit.insert(nameSplit.begin() + i + 1, actualSplit.mid(j + 1, actualSplit.length()));
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
        QLabel *text = new QLabel(iconText, textIcon);
        QFont font;
        font.setFamily("whitney");
        if (small) {
            font.setPixelSize(6);
        } else {
            font.setPixelSize(16);
            QFontMetrics metrics(font);
            int fontWidth = metrics.width(iconText);
            if (fontWidth > 48) {
                font.setPixelSize(10);
                text->setAlignment(Qt::AlignCenter);
            } else {
                font.setPixelSize(16);
            }
        }
        text->setFont(font);

        iconTextLayout->setContentsMargins(0, 0, 0, 0);
        iconTextLayout->addWidget(text, 0, Qt::AlignHCenter);

        text->setFixedHeight(small ? 10 : 16);
        textIcon->setFixedSize(small ? 16 : 48, small ? 16 : 48);
        textIcon->setStyleSheet("border-radius: " + QString::number(small ? 8 : 24) + "px;"
                                "color: #DCDDDE;"
                                "background-color: #36393F;");
        icon = nullptr;

    } else {
        // This guild has an icon

        // Request the icon and cache it
        QString guildIconFileName(guildIcon);
        guildIconFileName += ".png";
        if (!QFile::exists("cache/" + guildIconFileName)) {
            icon = new RoundedImage(small ? 16 : 48, small ? 16 : 48, small ? 8 : 24, this);
            rm->getImage([this](void *iconFileName) {emit iconRecieved(*static_cast<QString *>(iconFileName));}, "https://cdn.discordapp.com/icons/" + guildId + "/" + guildIconFileName, guildIconFileName);
        } else {
            guildIconFileName = "cache/" + guildIconFileName;

            // Create the icon and add it to the layout
            icon = new RoundedImage(guildIconFileName, small ? 16 : 48, small ? 16 : 48, small ? 8 : 24, this);
        }
        icon->setStyleSheet("background-color: none;");
    }
}

void const GuildIcon::setIcon(const QString& guildIconFileName)
{
    icon->setImage(guildIconFileName);
}

void GuildIcon::setActive()
{
    if (!icon)
        textIcon->setStyleSheet("border-radius: " + QString::number(small ? 6 : 16) + "px;"
                                "color: #FFF;"
                                "background-color: #5865F2;");
}

void GuildIcon::setInactive()
{
    if (!icon)
        textIcon->setStyleSheet("border-radius: " + QString::number(small ? 8 : 24) + "px;"
                                "color: #DCDDDE;"
                                "background-color: #36393F;");
}

}