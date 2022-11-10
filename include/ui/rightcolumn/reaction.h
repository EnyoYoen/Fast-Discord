#pragma once

#include "settings/settings.h"
#include "ui/common/basicwidgets.h"
#include "api/objects/emoji.h"

#include <QMouseEvent>

namespace Ui {

class Reaction : public Widget
{
public:
    Reaction(Api::RessourceManager *rm, const Api::Reaction& reaction, const Api::Snowflake& channelId, const Api::Snowflake& messageId, QWidget *parent);

    void addReaction(bool me);
    bool removeReaction(bool me, bool all);
    
    Api::Reaction reaction;

private:
    void mouseReleaseEvent(QMouseEvent *event) override;
    void enterEvent(QEvent *) override;
    void leaveEvent(QEvent *) override;

    Label *countLabel;
    Api::Snowflake channelId;
    Api::Snowflake messageId;
    Api::RessourceManager *rm;
    int count;
    bool active;
};

} // namespace Ui