#include "ui/common/roundedimage.h"

#include <QPainter>
#include <QPainterPath>
#include <QMovie>
#include <QFileInfo>

namespace Ui {

RoundedImage::RoundedImage(const QString& imagePath, int width, int height, int radius, QWidget *parent)
    : Widget(parent)
{
    imgPath = imagePath;
    if (imagePath.mid(imagePath.length() - 4, 4).compare(".gif") == 0) {
        this->setMovie(new QMovie(imagePath));
        this->mov->start();
        this->mov->setPaused(true);
    } else {
        this->setPixmap(QPixmap(imagePath).scaled(width, height, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }

    this->setBorderRadius(radius);
    this->setFixedSize(width, height);
}

RoundedImage::RoundedImage(int width, int height, int radius, QWidget *parent)
    : Widget(parent)
{
    this->setBorderRadius(radius);
    this->setFixedSize(width, height);
}

void RoundedImage::changeSize(int width, int height, int radius)
{
    if (imgPath.mid(imgPath.length() - 4, 4).compare(".gif") == 0) {
        this->setMovie(new QMovie(imgPath));
        this->mov->start();
        this->mov->setPaused(true);
    } else {
        this->setPixmap(QPixmap(imgPath).scaled(width, height, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }

    this->setBorderRadius(radius);
    this->setFixedSize(width, height);
}

void RoundedImage::setRoundedImage(const QString& imagePath)
{
    imgPath = imagePath;
    if (imagePath.mid(imagePath.length() - 4, 4).compare(".gif") == 0) {
        this->setMovie(new QMovie(imagePath));
        this->mov->start();
        this->mov->setPaused(true);
    } else {
        this->setImage(imagePath);
    }
}

void RoundedImage::clear()
{
    this->setPixmap(QPixmap());
    this->mov = nullptr;
}

void RoundedImage::enterEvent(QEvent *)
{
    if (this->mov != nullptr && this->mov->state() == QMovie::Paused) this->mov->setPaused(false);
}

void RoundedImage::leaveEvent(QEvent *)
{
    if (this->mov != nullptr && this->mov->state() != QMovie::Paused) {
        this->mov->stop();
        this->mov->start();
        this->mov->setPaused(true);
    }
}

} // namespace Ui
