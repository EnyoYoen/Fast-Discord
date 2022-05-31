#pragma once

#include "api/ressourcemanager.h"

#include <QWidget>
#include <QHBoxLayout>

namespace Ui {

class Settings : public QWidget
{
    Q_OBJECT
public:
    Settings(Api::RessourceManager *rm, QWidget *parent);

signals:
    void closeClicked();

private:
    QHBoxLayout *layout;
    Api::RessourceManager *rm;
};

} // namespace Ui