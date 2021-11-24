#include "ui/roundedimage.h"

#include <QPainter>

#include <string>

namespace Ui {

RoundedImage::RoundedImage(const std::string& imagePath, int width, int height, int radius, QWidget *parent)
    : QLabel(parent)
{
    // Attributes initialization
    h = height;
    w = width;
    r = radius;

    // Set the image to the widget
    image = QPixmap(imagePath.c_str()).scaled(w, h, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    this->setPixmap(image);

    // Set the size of the widget
    this->setFixedSize(width, height);
}

void RoundedImage::paintEvent(QPaintEvent *)
{
    // Make the image rounded with some antialiasing

    QPainter p(this);

    QPixmap target = QPixmap(w, h);
    target.fill(Qt::transparent);

    p.setRenderHint(QPainter::Antialiasing);
    p.setRenderHint(QPainter::SmoothPixmapTransform);

    QPainterPath path = QPainterPath();
    path.addRoundedRect(0, 0, h, w, r, r);

    p.setClipPath(path);
    p.drawPixmap(0, 0, image);
}

} // namespace Ui
