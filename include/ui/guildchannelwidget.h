#pragma once

#include "../api/channel.h"

#include <QWidget>
#include <QPixmap>
#include <QHBoxLayout>
#include <QLabel>
#include <QMouseEvent>

#include <string>

namespace Ui {

class GuildChannelWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GuildChannelWidget(const Api::Channel& guildChannel, unsigned int idp, QWidget *parent = nullptr);
    void unclicked();

signals:
    void leftClicked(Api::Channel&, unsigned int);
    void rightClicked(Api::Channel&);

private:
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *) override;
    void enterEvent(QEvent *) override;
    void leaveEvent(QEvent *) override;

    Api::Channel channel;
    QHBoxLayout  *layout;
    QLabel       *icon;
    QLabel       *name;
    char         *hoverStyleSheet;
    char         *clickedStyleSheet;
    unsigned int id;
    bool         clicked;
};

} // namespace Ui
