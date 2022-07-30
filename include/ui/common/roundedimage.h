#pragma once

#include "ui/common/basicwidgets.h"

#include <QPaintEvent>
#include <QLabel>

namespace Ui {

// A class to make easy rounded images
class RoundedImage : public Widget
{
    Q_OBJECT
public:
    RoundedImage(const QString& imagePath, int width, int height, int radius, QWidget *parent);
    RoundedImage(int width, int height, int radius, QWidget *parent);
    void changeSize(int width, int height, int radius);
    void setRoundedImage(const QString& imagePath);
    void clear();

private:
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);

    QString imgPath;
};

} // namespace Ui
