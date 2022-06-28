#include "ui/rightcolumn/downloadlink.h"

#include <QFileDialog>
#include <QStandardPaths>

namespace Ui {

DownloadLink::DownloadLink(const QString& urlp, const QString& filename, Api::Requester *request, QWidget *parent)
    : Label(filename, parent)
{
    QFont font;
    font.setPixelSize(16);
    font.setFamily("whitney");
    url = QString(urlp);
    requester = request;

    this->setFixedSize(QFontMetrics(font).width(filename), 20);
    this->setFont(font);
    this->setCursor(Qt::PointingHandCursor);
    this->setTextColor(Settings::Link);
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
        requester->getFile(url, downloadsFolder + url.right(url.size() - url.lastIndexOf('/')));
}

}
