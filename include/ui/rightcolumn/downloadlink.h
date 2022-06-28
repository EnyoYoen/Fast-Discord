#pragma once

#include "ui/common/basicwidgets.h"
#include "api/request.h"

#include <QLabel>
#include <QMouseEvent>

namespace Ui {

class DownloadLink : public Label
{
    Q_OBJECT
public:
    DownloadLink(const QString& url, const QString& filename, Api::Requester *requester, QWidget *parent);

private:
    void mouseReleaseEvent(QMouseEvent *event) override;

    QString url;
    Api::Requester *requester;
};

}
