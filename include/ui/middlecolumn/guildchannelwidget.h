#pragma once

#include "ui/common/basicwidgets.h"
#include "settings/settings.h"
#include "api/objects/channel.h"
#include "api/objects/snowflake.h"

#include <QWidget>
#include <QPixmap>
#include <QHBoxLayout>
#include <QLabel>
#include <QMouseEvent>

namespace Ui {

// A widget to show guild channels (in the middle column)
class GuildChannelWidget : public Widget
{
    Q_OBJECT
public:
    GuildChannelWidget(const Api::Channel& guildChannel, QWidget *parent);
    void unclicked(); // Reset the stylesheet of the widget

    Label          *name;
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
    Widget      *icon;
    Widget      *container;

    Settings::ColorEnum hoverColor;
    Settings::ColorEnum clickedColor;

    QString     iconName;
    bool        clicked; // If the widget is clicked
};

} // namespace Ui
