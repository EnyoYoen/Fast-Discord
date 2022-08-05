#pragma once

#include "api/ressourcemanager.h"

#include <QScrollArea>

namespace Ui {

class MyAccount : public QScrollArea
{
    Q_OBJECT
public:
    MyAccount(Api::RessourceManager *rm, QWidget *parent);

private:
    Api::RessourceManager *rm;
};

}