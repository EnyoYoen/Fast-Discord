#pragma once

#include "roundedimage.h"
#include "api/guild.h"

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
    GuildWidget(const Api::Guild& guildP, unsigned int idp, QWidget *parent);
    void unclicked(); // Reset the stylesheet of the widget

signals:
    void leftClicked(Api::Guild&, unsigned int);
    void rightClicked(Api::Guild&);

private:
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *) override;
    void enterEvent(QEvent *) override;
    void leaveEvent(QEvent *) override;
    void setIcon(const std::string& guildIconFileName);

    // All the main widgets
    QVBoxLayout *layout;
    RoundedImage *icon;
    Api::Guild   guild;

    unsigned int id;      // The id that we assign to the widget
    bool         clicked; // If the widget is clicked
};

} // namespace Ui
