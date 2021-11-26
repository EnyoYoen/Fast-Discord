#pragma once

#include "roundedimage.h"
#include "api/channel.h"

#include <QWidget>
#include <QLabel>

namespace Ui {

// A widget for private channels (DMs and group DMs)
class PrivateChannel : public QWidget
{
    Q_OBJECT
public:
    PrivateChannel(const Api::Channel& privateChannel, unsigned int idp, QWidget *parent);
    void unclicked(); // Reset the stylesheet of the widget

signals:
    void leftClicked(Api::Channel&, unsigned int);
    void rightClicked(Api::Channel&);

private:
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mousePressEvent (QMouseEvent *) override;
    void enterEvent(QEvent *) override;
    void leaveEvent(QEvent *) override;

    // All the main widgets
    Api::Channel *channel;
    RoundedImage *icon;
    QLabel       *name;
    QLabel       *subtext;

    unsigned int id;      // The id that we assign to the widget
    bool         clicked; // If the widget is clicked
};

} // namespace Ui
