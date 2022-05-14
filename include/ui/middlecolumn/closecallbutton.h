#pragma once

#include "api/ressourcemanager.h"

#include <QLabel>
#include <QMouseEvent>

namespace Ui {

class CloseCallButton : public QLabel
{
    Q_OBJECT
public:
    CloseCallButton(Api::RessourceManager *rm, QWidget *parent);

signals:
    void clicked();

private:
    void mouseReleaseEvent(QMouseEvent *event) override;
    void enterEvent(QEvent *) override;
    void leaveEvent(QEvent *) override;

    Api::RessourceManager *rm;
};

}
