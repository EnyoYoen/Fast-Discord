#pragma once

#include "roundedimage.h"
#include "../api/channel.h"

#include <QWidget>
#include <QLabel>
#include <QGridLayout>

namespace Ui {

class PrivateChannel : public QWidget
{
    Q_OBJECT
public:
    PrivateChannel(const Api::Channel& privateChannel, QWidget *parent = nullptr);

signals:
    void leftClicked(Api::Channel&);
    void rightClicked(Api::Channel&);

private:
    void mouseReleaseEvent(QMouseEvent *event) override;

    Api::Channel *channel;
    RoundedImage *icon;
    QLabel       *name;
    QLabel       *subtext;
    QGridLayout  *layout;
};

} // namespace Ui
