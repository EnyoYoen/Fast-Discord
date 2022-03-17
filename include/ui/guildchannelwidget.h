#pragma once

#include "api/channel.h"
#include "api/snowflake.h"

#include <QWidget>
#include <QPixmap>
#include <QHBoxLayout>
#include <QLabel>
#include <QMouseEvent>

namespace Ui {

// A widget to show guild channels (in the middle column)
class GuildChannelWidget : public QWidget
{
    Q_OBJECT
public:
    GuildChannelWidget(const Api::Channel& guildChannel, QWidget *parent);
    void unclicked(); // Reset the stylesheet of the widget

    Api::Snowflake id;     // The id that we assign to the widget

signals:
    void leftClicked(const Api::Snowflake& id);
    void rightClicked(const Api::Snowflake& id);

private:
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *) override;
    void enterEvent(QEvent *) override;
    void leaveEvent(QEvent *) override;

    // All the main widgets
    QHBoxLayout *layout;
    QLabel      *icon;
    QLabel      *name;

    // Different stylesheets when the widget is clicked or the mouse is hover
    char        *hoverStyleSheet;
    char        *clickedStyleSheet;

    int         type;
    bool        clicked; // If the widget is clicked
};

} // namespace Ui
