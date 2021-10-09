#include "../include/ui/roundedimage.h"

#include <QPainter>

#include <string>

namespace Ui {

RoundedImage::RoundedImage(const std::string& imagePath, int width, int height, int radius)
{
    h = height;
    w = width;
    r = radius;
    setFixedSize(width, height);

    image = QPixmap(imagePath.c_str()).scaled(w, h, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    setPixmap(image);
}

void RoundedImage::paintEvent(QPaintEvent */*event*/)
{
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
