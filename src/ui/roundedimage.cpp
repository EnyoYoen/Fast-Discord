#include "ui/roundedimage.h"

#include <QPainter>
#include <QPainterPath>
#include <QMovie>
#include <QFileInfo>

#include <string>
#include <iostream>

namespace Ui {

RoundedImage::RoundedImage(const std::string& imagePath, int width, int height, int radius, QWidget *parent)
    : QLabel(parent)
{
    // Attributes initialization
    h = height;
    w = width;
    r = radius;
    hasImage = true;

    // Determine if the image is animated and set the image to the widget
    if (imagePath.compare(imagePath.length() - 4, 4, ".gif") == 0) {
        // This is an animated image
        image = QPixmap(imagePath.c_str()).scaled(w, h, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        animatedImage = new QMovie(imagePath.c_str());
        animatedImage->start();
        animatedImage->setPaused(true);
        this->setMovie(animatedImage);
    } else {
        // Not animated
        image = QPixmap(imagePath.c_str()).scaled(w, h, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        this->setPixmap(image);
        animatedImage = nullptr;
    }

    // Set the size of the widget
    this->setFixedSize(width, height);
}

RoundedImage::RoundedImage(int width, int height, int radius, QWidget *parent)
    : QLabel(parent)
{
    hasImage = false;
    // Attributes initialization
    h = height;
    w = width;
    r = radius;

    // Set the size of the widget
    this->setFixedSize(width, height);
}

void RoundedImage::setImage(const std::string& imagePath)
{
    hasImage = true;

    // Determine if the image is animated and set the image to the widget
    if (imagePath.compare(imagePath.length() - 4, 4, ".gif") == 0) {
        // This is an animated image
        image = QPixmap(imagePath.c_str()).scaled(w, h, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        animatedImage = new QMovie(imagePath.c_str());
        animatedImage->start();
        animatedImage->setPaused(true);
        this->setMovie(animatedImage);
    } else {
        // Not animated
        image = QPixmap(imagePath.c_str()).scaled(w, h, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        if (image.isNull())
            image = QPixmap(imagePath.c_str(), "JPG").scaled(w, h, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        this->setPixmap(image);
        animatedImage = nullptr;
    }
}

void RoundedImage::paintEvent(QPaintEvent *)
{
    if (hasImage) {
        // Make the image rounded with some antialiasing
        QPainter p(this);
        p.setRenderHint(QPainter::Antialiasing);
        p.setRenderHint(QPainter::SmoothPixmapTransform);

        if (animatedImage != nullptr) {
            QPixmap target = animatedImage->currentPixmap().scaled(w, h, Qt::KeepAspectRatio, Qt::SmoothTransformation);
            QBrush brush = QBrush(target);
            p.setPen(Qt::NoPen);
            p.setBrush(target);
            p.drawRoundedRect(0, 0, h, w, r, r);
        } else {
            QPainterPath path = QPainterPath();
            path.addRoundedRect(0, 0, h, w, r, r);

            p.setClipPath(path);
            p.drawPixmap(0, 0, image);
        }
    }
}

void RoundedImage::enterEvent(QEvent *)
{
    if (animatedImage != nullptr && animatedImage->state() == QMovie::Paused) animatedImage->setPaused(false);
}

void RoundedImage::leaveEvent(QEvent *)
{
    if (animatedImage != nullptr && animatedImage->state() != QMovie::Paused) {
        animatedImage->stop();
        animatedImage->start();
        animatedImage->setPaused(true);
    }
}

} // namespace Ui
