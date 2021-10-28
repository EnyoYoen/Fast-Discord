#pragma once

#include "messagewidget.h"
#include "../../include/api/message.h"

#include <QShowEvent>
#include <QScrollArea>

#include <vector>

namespace Ui {

class MessageArea : public QScrollArea
{
    Q_OBJECT
public:
    MessageArea(const std::vector<Api::Message *>& messages);

private:
    void showEvent(QShowEvent *event) override;
};

} // namespace Ui
