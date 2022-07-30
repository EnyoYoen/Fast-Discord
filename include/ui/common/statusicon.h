#pragma once

#include "ui/common/basicwidgets.h"

#include <QWidget>
#include <QPaintEvent>

namespace Ui {

class StatusIcon : public Widget
{
public:
    StatusIcon(QWidget *parent);
    void setStatus(const QString& status);
};

} // namespace Ui
