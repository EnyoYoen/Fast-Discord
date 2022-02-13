#pragma once

#include "api/request.h"

#include <QLabel>
#include <QMouseEvent>

namespace Ui {

class DownloadLink : public QLabel
{
    Q_OBJECT
public:
    DownloadLink(const std::string& url, const std::string& filename, Api::Requester *requester, QWidget *parent);

private:
    void mouseReleaseEvent(QMouseEvent *event) override;

    std::string url;
    Api::Requester *requester;
};

}
