#pragma once

#include "api/request.h"

#include <QLabel>
#include <QMouseEvent>

namespace Ui {

class DownloadButton : public QLabel
{
    Q_OBJECT
public:
    DownloadButton(const std::string& url, Api::Requester *requester, QWidget *parent);

private:
    void mouseReleaseEvent(QMouseEvent *event) override;

    std::string url;
    Api::Requester *requester;
};

}
