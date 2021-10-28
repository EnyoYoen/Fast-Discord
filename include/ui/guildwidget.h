#pragma once

#include "roundedimage.h"
#include "../api/guild.h"

#include <QFrame>
#include <QLabel>
#include <QVBoxLayout>

namespace Ui {

class GuildWidget : public QFrame
{
    Q_OBJECT
public:
    GuildWidget(const Api::Guild& guildP, QWidget *parent = nullptr);

signals:
    void leftClicked(Api::Guild&);
    void rightClicked(Api::Guild&);

private:
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mousePressEvent (QMouseEvent *) override;
    void enterEvent(QEvent *) override;
    void leaveEvent(QEvent *) override;

    Api::Guild   guild;
    RoundedImage *icon;
    QVBoxLayout  *layout;
    QLabel       *textIcon;
    bool         clicked;
};

} // namespace Ui
