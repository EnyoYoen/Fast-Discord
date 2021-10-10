#pragma once

#include <QPaintEvent>
#include <QLabel>

namespace Ui {

class RoundedImage : public QLabel
{
    Q_OBJECT
public:
    RoundedImage(const std::string& imagePath, int width, int height, int radius);
private:
    void paintEvent(QPaintEvent *) override;

    QPixmap image;
    int w;
    int h;
    int r;
};

} // namespace Ui
