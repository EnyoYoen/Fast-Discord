#pragma once

#include <QWidget>
#include <QLabel>
#include <QTextEdit>

namespace Ui {

class PopUp : public QWidget
{
    Q_OBJECT
public:
    PopUp(QWidget *content, int maximumWidth, int maximumHeight, QString icon, 
        QString title, bool titleCentered, bool closeable, QString htmlDescription, 
        QString cancelButtonStr, QString doneButtonStr, bool controlButtons, 
        bool horizontalButtons, QSize winSize, QWidget *parent);

signals:
    void cancelled();
    void done();

private:
    void mouseReleaseEvent(QMouseEvent *) override;

    QLabel *actualPopup;
};

} // namespace Ui