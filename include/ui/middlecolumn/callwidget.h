#pragma once

#include "api/ressourcemanager.h"

#include <QLabel>

namespace Ui {

class CallWidget : public QLabel
{
    Q_OBJECT
public:
    CallWidget(Api::RessourceManager *rm, QWidget *parent);
    void call(QString channel, QString guild);

private:
    QLabel *name;

    Api::RessourceManager *rm;
};

} // namespace Ui
