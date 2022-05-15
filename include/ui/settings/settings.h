#pragma once

#include "api/ressourcemanager.h"

#include <QWidget>

namespace Ui {

class Settings : public QWidget
{
    Q_OBJECT
public:
    Settings(Api::RessourceManager *rm, QWidget *parent);

signals:
    void closeClicked();

private:
    Api::RessourceManager *rm;
};

} // namespace Ui