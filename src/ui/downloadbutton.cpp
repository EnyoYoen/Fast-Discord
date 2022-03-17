#include "ui/downloadbutton.h"

#include <QFileDialog>
#include <QStandardPaths>

namespace Ui {

DownloadButton::DownloadButton(const QString& urlp, Api::Requester *request, QWidget *parent)
    : QLabel(parent)
{
    url = QString(urlp);
    requester = request;

    this->setFixedSize(24, 24);
    this->setCursor(Qt::PointingHandCursor);
    this->setPixmap(QPixmap("res/images/svg/download-icon.svg"));
}

void DownloadButton::mouseReleaseEvent(QMouseEvent *)
{
    QString downloadsFolder = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation) + "/";
    if (downloadsFolder.isEmpty()) {
        QDir dir("download/");
        if (!dir.exists()) dir.mkpath(".");
        downloadsFolder = "download/";
    }

    downloadsFolder = QFileDialog::getExistingDirectory(this, "Download", downloadsFolder) + "/";

    if (downloadsFolder != "/")
        requester->getFile(url, downloadsFolder + url.left(url.lastIndexOf('/') + 1));
}

}
