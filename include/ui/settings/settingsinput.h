#pragma once

#include "ui/common/basicwidgets.h"

#include <QObject>
#include <QWidget>
#include <QLineEdit>
#include <QKeyEvent>

namespace Ui {

class SettingsInput : public Widget
{
    Q_OBJECT
public:
    SettingsInput(QString placeholder, QString text, bool readOnly, bool password, QWidget *parent);

    QLineEdit *input;

private:
    void mouseReleaseEvent(QMouseEvent *) override;
};

} // namespace Ui
