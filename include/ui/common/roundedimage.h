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
    void setRoundedImage(const QString& imagePath);
    void clear();

private:
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);

    int w, h;
};

} // namespace Ui
