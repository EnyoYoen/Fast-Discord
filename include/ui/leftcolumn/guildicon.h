#pragma once

#include "ui/common/basicwidgets.h"
#include "ui/common/roundedimage.h"
#include "api/ressourcemanager.h"

#include <QWidget>
#include <QLabel>

namespace Ui {

class TextIcon;

class GuildIcon : public Widget
{
    Q_OBJECT
public:
    GuildIcon(Api::RessourceManager *rm, const Api::Snowflake& guildId, QString guildName, const QString& guildIcon, bool small, QWidget *parent);
    void setActive();
    void setInactive();

signals:
    void iconRecieved(const QString& iconFileName);

private slots:
    void const setIcon(const QString& guildIconFileName);

private:
    RoundedImage *icon;
    TextIcon     *textIcon;
    bool          small;
};

}
