#pragma once

#include "ui/leftcolumn/guildpill.h"

#include <QLabel>

namespace Ui {

// The button at the top left corner of the app
class HomeButton : public QWidget
{
    Q_OBJECT
public:
    HomeButton(QWidget *parent);
    void unclicked(); // Reset the stylesheet of the widget

signals:
    void clicked();

private:
    void mouseReleaseEvent(QMouseEvent *) override;
    void mousePressEvent(QMouseEvent *) override;
    void enterEvent(QEvent *) override;
    void leaveEvent(QEvent *) override;

    GuildPill *pill;
    Widget    *image;

    bool clic; // If the widget is clicked
};

} // namespace Ui
