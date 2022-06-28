#pragma once

#include "ui/common/basicwidgets.h"
#include "api/ressourcemanager.h"

#include <QLabel>

namespace Ui {

class CallWidget : public Widget
{
    Q_OBJECT
public:
    CallWidget(Api::RessourceManager *rm, QWidget *parent);
    void call(QString channel, QString guild);

private:
    Label *name;

    Api::RessourceManager *rm;
};

} // namespace Ui
