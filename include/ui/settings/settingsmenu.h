#pragma once

#include "ui/common/basicwidgets.h"
#include "ui/settings/components/scrollmenu.h"
#include "api/ressourcemanager.h"

#include <QWidget>
#include <QHBoxLayout>

namespace Ui {

class SettingsMenu : public Widget
{
    Q_OBJECT
public:
    SettingsMenu(Api::RessourceManager *rm, QWidget *parent);

    void updateTheme();

signals:
    void closeClicked();

private:
    QHBoxLayout *layout;
    ScrollMenu *scrollMenu;
    QWidget *menu = nullptr;
    Api::RessourceManager *rm;
};

} // namespace Ui