#include "ui/downloadbutton.h"

namespace Ui {

DownloadButton::DownloadButton(const std::string& urlp, Api::Requester *request, QWidget *parent)
    : QLabel(parent)
{
    url = urlp;
    requester = request;

    this->setFixedSize(24, 24);
    this->setPixmap(QPixmap("res/images/svg/download-icon.svg"));
}

void DownloadButton::mouseReleaseEvent(QMouseEvent *event)
{
    requester->getFile(url, url.substr(url.find_last_of('/') + 1));
}

}
