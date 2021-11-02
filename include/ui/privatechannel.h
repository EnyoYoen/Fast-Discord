#pragma once

#include "roundedimage.h"
#include "../api/channel.h"

#include <QWidget>
#include <QLabel>

namespace Ui {

class PrivateChannel : public QWidget
{
    Q_OBJECT
public:
    PrivateChannel(const Api::Channel& privateChannel, unsigned int idp, QWidget *parent = nullptr);
    void unclicked();

signals:
    void leftClicked(Api::Channel&, unsigned int);
    void rightClicked(Api::Channel&);

private:
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mousePressEvent (QMouseEvent *) override;
    void enterEvent(QEvent *) override;
    void leaveEvent(QEvent *) override;

    Api::Channel *channel;
    RoundedImage *icon;
    QLabel       *name;
    QLabel       *subtext;
    unsigned int id;
    bool         clicked;
};

} // namespace Ui
