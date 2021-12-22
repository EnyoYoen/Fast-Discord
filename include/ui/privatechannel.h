#pragma once

#include "roundedimage.h"
#include "api/channel.h"
#include "api/ressourcemanager.h"
#include "api/user.h"

#include <QWidget>
#include <QLabel>

#include <queue>

namespace Ui {

// A widget for private channels (DMs and group DMs)
class PrivateChannelWidget : public QWidget
{
    Q_OBJECT
public:
    PrivateChannelWidget(Api::RessourceManager *rm, const Api::PrivateChannel& privateChannel, QWidget *parent);
    void unclicked(); // Reset the stylesheet of the widget
    void setStatus(std::string *status);

    std::string id;

signals:
    void leftClicked(const std::string&);
    void rightClicked(const std::string&);

private:
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mousePressEvent (QMouseEvent *) override;
    void enterEvent(QEvent *) override;
    void leaveEvent(QEvent *) override;
    void setIcon(const std::string& iconFileName);

    Api::RessourceManager *rm; // To request the API

    // All the main widgets
    RoundedImage        *icon;
    QLabel              *name;
    QLabel              *subtext;
    QLabel              *statusIcon = nullptr;

    std::string  status;
    bool         clicked; // If the widget is clicked
};

} // namespace Ui
