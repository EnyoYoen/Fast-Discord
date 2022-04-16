#include "ui/downloadbutton.h"

#include <QFileDialog>
#include <QStandardPaths>
#include <QPainter>

namespace Ui {

DownloadButton::DownloadButton(const QString& urlp, Api::Requester *request, QWidget *parent)
    : QLabel(parent)
{
    url = QString(urlp);
    requester = request;

    this->setFixedSize(24, 24);
    this->setCursor(Qt::PointingHandCursor);

    QPixmap img("res/images/svg/download-icon.svg");
    QPainter qp(&img);
    qp.setCompositionMode(QPainter::CompositionMode_SourceIn);
    qp.fillRect(img.rect(),QColor(185, 187, 190));
    qp.end();
    this->setPixmap(img);
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

void DownloadButton::enterEvent(QEvent *)
{
    QPixmap img("res/images/svg/download-icon.svg");
    QPainter qp(&img);
    qp.setCompositionMode(QPainter::CompositionMode_SourceIn);
    qp.fillRect(img.rect(),QColor(220, 221, 222));
    qp.end();
    this->setPixmap(img);
}

void DownloadButton::leaveEvent(QEvent *)
{
    QPixmap img("res/images/svg/download-icon.svg");
    QPainter qp(&img);
    qp.setCompositionMode(QPainter::CompositionMode_SourceIn);
    qp.fillRect(img.rect(),QColor(185, 187, 190));
    qp.end();
    this->setPixmap(img);
}

}
