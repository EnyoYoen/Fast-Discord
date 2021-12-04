#pragma once

#include "api/channel.h"

#include <QWidget>
#include <QPixmap>
#include <QHBoxLayout>
#include <QLabel>
#include <QMouseEvent>

#include <string>

namespace Ui {

// A widget to show guild channels (in the middle column)
class GuildChannelWidget : public QWidget
{
    Q_OBJECT
public:
    GuildChannelWidget(const Api::Channel& guildChannel, unsigned int idp, QWidget *parent);
    void unclicked(); // Reset the stylesheet of the widget

signals:
    void leftClicked(Api::Channel&, unsigned int);
    void rightClicked(Api::Channel&);

private:
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *) override;
    void enterEvent(QEvent *) override;
    void leaveEvent(QEvent *) override;

    // All the main widgets
    Api::Channel channel;
    QHBoxLayout  *layout;
    QLabel       *icon;
    QLabel       *name;

    // Different stylesheets when the widget is clicked or the mouse is hover
    char         *hoverStyleSheet;
    char         *clickedStyleSheet;

    unsigned int id;      // The id that we assign to the widget
    bool         clicked; // If the widget is clicked
};

} // namespace Ui
