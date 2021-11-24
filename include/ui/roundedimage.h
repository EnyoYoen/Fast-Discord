#pragma once

#include <QPaintEvent>
#include <QLabel>

namespace Ui {

// A class to make easy rounded images
class RoundedImage : public QLabel
{
    Q_OBJECT
public:
    RoundedImage(const std::string& imagePath, int width, int height, int radius, QWidget *parent);
private:
    void paintEvent(QPaintEvent *) override;

    // The image and its dimensions
    QPixmap image;
    int w;
    int h;
    int r;
};

} // namespace Ui
