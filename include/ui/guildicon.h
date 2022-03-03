#pragma once

#include "ui/roundedimage.h"
#include "api/ressourcemanager.h"

#include <QWidget>
#include <QLabel>

namespace Ui {

class GuildIcon : public QWidget
{
    Q_OBJECT
public:
    GuildIcon(Api::RessourceManager *rm, const std::string& guildId, std::string guildName, std::string *guildIcon, bool small, QWidget *parent);
    void setActive();
    void setInactive();

signals:
    void iconRecieved(const std::string& iconFileName);

private slots:
    void setIcon(const std::string& guildIconFileName);

private:
    RoundedImage *icon;
    QLabel       *textIcon;
    bool          small;
};

}
