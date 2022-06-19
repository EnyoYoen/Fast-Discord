#pragma once

#include "api/ressourcemanager.h"

#include <QScrollArea>

namespace Ui {

class Connections : public QScrollArea
{
    Q_OBJECT
public:
    Connections(Api::RessourceManager *rm, QWidget *parent);

private:
    void empty();

    QVBoxLayout *layout;

    Api::RessourceManager *rm;
};

}