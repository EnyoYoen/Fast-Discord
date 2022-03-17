#pragma once

#include "api/request.h"

#include <QLabel>
#include <QMouseEvent>

namespace Ui {

class DownloadButton : public QLabel
{
    Q_OBJECT
public:
    DownloadButton(const QString& url, Api::Requester *requester, QWidget *parent);

private:
    void mouseReleaseEvent(QMouseEvent *event) override;

    QString url;
    Api::Requester *requester;
};

}
