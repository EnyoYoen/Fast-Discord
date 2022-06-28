#pragma once

#include "ui/common/basicwidgets.h"
#include "api/ressourcemanager.h"

#include <QScrollArea>

namespace Ui {

class AuthorizedApps : public QScrollArea
{
    Q_OBJECT
public:
    AuthorizedApps(Api::RessourceManager *rm, QWidget *parent);

private slots:
    void empty();

private:
    Widget *container;
    QVBoxLayout *layout;

    Api::RessourceManager *rm;
};

}