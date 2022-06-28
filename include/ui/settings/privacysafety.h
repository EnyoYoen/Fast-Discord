#pragma once

#include "ui/common/basicwidgets.h"
#include "ui/settings/switchbutton.h"
#include "api/ressourcemanager.h"
#include "api/objects/optional.h"

#include <QScrollArea>

namespace Ui {

class PrivacySafety : public QScrollArea
{
    Q_OBJECT
public:
    PrivacySafety(Api::RessourceManager *rm, QWidget *parent);

private:
    Label *createTitle(QString text);
    
    enum class SectionType : quint8 {
        None,
        All,
        MutualFriends,
        MutualGuilds
    };
    QWidget *createSection(std::function<void(bool)> callback, QString title, QString description, optbool active, SectionType type);

    Widget *container;
    SwitchButton *allSwitch;
    SwitchButton *mutualFriendsSwitch;
    SwitchButton *mutualGuildsSwitch;
    
    Api::RessourceManager *rm;
    bool all;
    bool mutualFriends;
    bool mutualGuilds;
};

}