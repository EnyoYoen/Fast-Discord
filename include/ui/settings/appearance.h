#pragma once

#include "api/ressourcemanager.h"

#include <QScrollArea>

namespace Ui {

class Appearance : public QScrollArea
{
public:
    Appearance(Api::RessourceManager *rm, QWidget *parent);

private:
    QVector<Api::Message *> messages;
};

}