#pragma once

#include "roundedimage.h"
#include "api/message.h"
#include "api/request.h"

#include <QWidget>
#include <QLabel>

namespace Ui {

// A widget to show messages
class MessageWidget : public QWidget
{
    Q_OBJECT
public:
    MessageWidget(Api::Requester *requester, const Api::Message& message, bool isFirst, bool separatorBefore, QWidget *parent);

private:
    void enterEvent(QEvent *) override;
    void leaveEvent(QEvent *) override;
    void setAvatar(const std::string& avatarFileName);

    Api::Requester *requester; // To request the API

    QString hoveredTimestamp; // The timestamp we show
    RoundedImage *avatar;     // The avatar if there is one
    QLabel *timestampLabel;   // The label of the timestamp
    bool isFirst;             // Is the message is not grouped

};

} // namespace Ui
