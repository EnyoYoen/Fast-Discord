#pragma once

#include "api/ressourcemanager.h"
#include "ui/common/basicwidgets.h"

#include <QScrollArea>

namespace Ui {

class Accessibility : public QScrollArea
{
public:
    Accessibility(Api::RessourceManager *rm, QWidget *parent);

private:
    Widget *createAvatar(QString status);
    Widget *createSeparator();
};

}