#include "ui/downloadlink.h"

#include <QFileDialog>
#include <QStandardPaths>

namespace Ui {

DownloadLink::DownloadLink(const std::string& urlp, const std::string& filename, Api::Requester *request, QWidget *parent)
    : QLabel(filename.c_str(), parent)
{
    url = std::string(urlp);
    requester = request;

    this->setCursor(Qt::PointingHandCursor);
    this->setStyleSheet("color: #00AFF4;");
}

void DownloadLink::mouseReleaseEvent(QMouseEvent *)
{
    QString downloadsFolder = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation) + "/";
    if (downloadsFolder.isEmpty()) {
        QDir dir("download/");
        if (!dir.exists()) dir.mkpath(".");
        downloadsFolder = "download/";
    }

    downloadsFolder = QFileDialog::getExistingDirectory(this, "Download", downloadsFolder) + "/";

    if (downloadsFolder != "/")
        requester->getFile(url, downloadsFolder.toStdString() + url.substr(url.find_last_of('/') + 1));
}

}
