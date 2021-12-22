#pragma once

#include "roundedimage.h"
#include "guildpill.h"
#include "api/guild.h"
#include "api/ressourcemanager.h"

#include <QFrame>
#include <QLabel>
#include <QHBoxLayout>

#include <string>

namespace Ui {

// A widget to show a guild (in the left column)
class GuildWidget : public QFrame
{
    Q_OBJECT
public:
    GuildWidget(Api::RessourceManager *rm, const Api::Guild& guildp, QWidget *parent);
    void unclicked(); // Reset the stylesheet of the widget
    void setUnread(bool unread);

    std::string  id;       // The id of the guild

signals:
    void iconRecieved(const std::string& iconFileName);
    void leftClicked(const std::string&);
    void rightClicked(const std::string&);

private slots:
    void setIcon(const std::string& guildIconFileName);

private:
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *) override;
    void enterEvent(QEvent *) override;
    void leaveEvent(QEvent *) override;

    Api::RessourceManager *rm; // To request the API

    // All the main widgets
    QHBoxLayout  *layout;
    RoundedImage *icon;
    QLabel       *textIcon;
    GuildPill    *pill;
    bool         clicked;  // If the widget is clicked
    bool         unreadMessages;
};

} // namespace Ui
