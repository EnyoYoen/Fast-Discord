#include "ui/fileuploadbutton.h"

#include <QFileDialog>
#include <QStandardPaths>
#include <QPainter>

namespace Ui {

FileUploadButton::FileUploadButton(QWidget *parent)
    : QLabel(parent)
{
    this->setFixedSize(40, 24);
    this->setCursor(Qt::PointingHandCursor);

    QPixmap img("res/images/svg/attach-button-icon.svg");
    this->setPixmap(img);
}

void FileUploadButton::mouseReleaseEvent(QMouseEvent *)
{
    QString defaultFolder = QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "/";
    if (defaultFolder.isEmpty()) {
        defaultFolder = ".";
    }

    QString uploadFilePath = QFileDialog::getOpenFileName(this, "File Upload", defaultFolder);
    if (!uploadFilePath.isEmpty())
        emit fileSelected(uploadFilePath);
}

void FileUploadButton::enterEvent(QEvent *)
{
    QPixmap img("res/images/svg/attach-button-icon.svg");
    QPainter qp(&img);
    qp.setCompositionMode(QPainter::CompositionMode_SourceIn);
    qp.fillRect(img.rect(),QColor(220, 221, 222));
    qp.end();
    this->setPixmap(img);
}

void FileUploadButton::leaveEvent(QEvent *)
{
    QPixmap img("res/images/svg/attach-button-icon.svg");
    QPainter qp(&img);
    qp.setCompositionMode(QPainter::CompositionMode_SourceIn);
    qp.fillRect(img.rect(),QColor(185, 187, 190));
    qp.end();
    this->setPixmap(img);
}

}