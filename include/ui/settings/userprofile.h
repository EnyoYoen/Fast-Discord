#pragma once

#include "api/ressourcemanager.h"

#include <QScrollArea>

namespace Ui {

class UserProfile : public QScrollArea
{
    Q_OBJECT
public:
    UserProfile(Api::RessourceManager *rm, QWidget *parent);

private:
    Api::RessourceManager *rm;
};

}