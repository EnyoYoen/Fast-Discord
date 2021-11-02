#pragma once

#include "roundedimage.h"
#include "../api/guild.h"

#include <QFrame>
#include <QLabel>
#include <QVBoxLayout>

#include <string>

namespace Ui {

class GuildWidget : public QFrame
{
    Q_OBJECT
public:
    GuildWidget(const Api::Guild& guildP, unsigned int idp, QWidget *parent = nullptr);
    void unclicked();

signals:
    void leftClicked(Api::Guild&, unsigned int);
    void rightClicked(Api::Guild&);

private:
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *) override;
    void enterEvent(QEvent *) override;
    void leaveEvent(QEvent *) override;

    RoundedImage *icon;
    Api::Guild   guild;
    unsigned int id;
    bool clicked;
};

} // namespace Ui
