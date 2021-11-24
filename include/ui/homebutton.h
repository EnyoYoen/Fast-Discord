#pragma once

#include <QLabel>

namespace Ui {

// The button at the top left corner of the app
class HomeButton : public QLabel
{
    Q_OBJECT
public:
    HomeButton(QWidget *parent = nullptr);
    void unclicked(); // Reset the stylesheet of the widget

signals:
    void clicked();

private:
    void mouseReleaseEvent(QMouseEvent *) override;
    void mousePressEvent(QMouseEvent *) override;
    void enterEvent(QEvent *) override;
    void leaveEvent(QEvent *) override;

    bool clic; // If the widget is clicked
};

} // namespace Ui
