#pragma once

#include "roundedimage.h"
#include "api/guild.h"
#include "api/request.h"

#include <QFrame>
#include <QLabel>
#include <QVBoxLayout>

#include <string>

namespace Ui {

// A widget to show a guild (in the left column)
class GuildWidget : public QFrame
{
    Q_OBJECT
public:
    GuildWidget(Api::Requester *requester, const Api::Guild& guildp, unsigned int idp, QWidget *parent);
    void unclicked(); // Reset the stylesheet of the widget

signals:
    void iconRecieved(const std::string& iconFileName);
    void leftClicked(Api::Guild&, unsigned int);
    void rightClicked(Api::Guild&);

private slots:
    void setIcon(const std::string& guildIconFileName);

private:
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *) override;
    void enterEvent(QEvent *) override;
    void leaveEvent(QEvent *) override;

    Api::Requester *requester; // To request the API

    // All the main widgets
    QVBoxLayout *layout;
    RoundedImage *icon;
    Api::Guild   guild;

    unsigned int id;      // The id that we assign to the widget
    bool         clicked; // If the widget is clicked
};

} // namespace Ui
