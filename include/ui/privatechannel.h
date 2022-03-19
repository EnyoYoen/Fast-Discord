#pragma once

#include "roundedimage.h"
#include "statusicon.h"
#include "api/ressourcemanager.h"
#include "api/objects/channel.h"
#include "api/objects/user.h"

#include <QWidget>
#include <QLabel>

namespace Ui {

// A widget for private channels (DMs and group DMs)
class PrivateChannelWidget : public QWidget
{
    Q_OBJECT
public:
    PrivateChannelWidget(Api::RessourceManager *rm, const Api::PrivateChannel& privateChannel, QWidget *parent);
    void unclicked(); // Reset the stylesheet of the widget
    void setStatus(const QString& status);

    Api::Snowflake id;

signals:
    void leftClicked(const Api::Snowflake&);
    void rightClicked(const Api::Snowflake&);
    void closeButtonClicked(const Api::Snowflake&, const Api::Snowflake&, int);

public slots:
    void closeChannel();

private:
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mousePressEvent (QMouseEvent *) override;
    void enterEvent(QEvent *) override;
    void leaveEvent(QEvent *) override;
    void setIcon(const QString& iconFileName);

    Api::RessourceManager *rm; // To request the API

    // All the main widgets
    RoundedImage        *icon;
    StatusIcon          *statusIcon;
    QLabel              *name;
    QLabel              *subtext;
    QLabel              *statusBackground;

    bool         clicked; // If the widget is clicked
};

} // namespace Ui
