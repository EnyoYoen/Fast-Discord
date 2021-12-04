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
    RoundedImage(int width, int height, int radius, QWidget *parent);
    void setImage(const std::string& imagePath);
private:
    void paintEvent(QPaintEvent *) override;
    void enterEvent(QEvent *) override;
    void leaveEvent(QEvent *) override;

    // The image and its dimensions
    QMovie *animatedImage;
    QPixmap image;
    int w;
    int h;
    int r;
    bool hasImage;
};

} // namespace Ui
