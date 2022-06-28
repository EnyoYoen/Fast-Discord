#include "ui/rightcolumn/fileuploadbutton.h"

#include "settings/settings.h"

#include <QFileDialog>
#include <QStandardPaths>
#include <QPainter>

namespace Ui {

FileUploadButton::FileUploadButton(QWidget *parent)
    : QLabel(parent)
{
    QPixmap img("res/images/svg/attach-button-icon.svg");
    QPainter qp(&img);
    qp.setCompositionMode(QPainter::CompositionMode_SourceIn);
    qp.fillRect(img.rect(), Settings::colors[Settings::InteractiveNormal]);
    qp.end();
    this->setPixmap(img);
    this->setFixedSize(40, 24);
    this->setCursor(Qt::PointingHandCursor);
}

void FileUploadButton::mouseReleaseEvent(QMouseEvent *)
{
    QPixmap img("res/images/svg/attach-button-icon.svg");
    QPainter qp(&img);
    qp.setCompositionMode(QPainter::CompositionMode_SourceIn);
    qp.fillRect(img.rect(), Settings::colors[Settings::InteractiveNormal]);
    qp.end();
    this->setPixmap(img);

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
    qp.fillRect(img.rect(), Settings::colors[Settings::InteractiveHover]);
    qp.end();
    this->setPixmap(img);
}

void FileUploadButton::leaveEvent(QEvent *)
{
    QPixmap img("res/images/svg/attach-button-icon.svg");
    QPainter qp(&img);
    qp.setCompositionMode(QPainter::CompositionMode_SourceIn);
    qp.fillRect(img.rect(), Settings::colors[Settings::InteractiveNormal]);
    qp.end();
    this->setPixmap(img);
}

}