#pragma once

#include "api/objects/channel.h"
#include "api/objects/snowflake.h"

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

    QLabel         *name;
    Api::Snowflake  id;     // The id that we assign to the widget
    int             type;

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

    // Different stylesheets when the widget is clicked or the mouse is hover
    char        *hoverStyleSheet;
    char        *clickedStyleSheet;

    QString     iconName;
    bool        clicked; // If the widget is clicked
};

} // namespace Ui
