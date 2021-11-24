#pragma once

#include "roundedimage.h"
#include "api/message.h"

#include <QWidget>
#include <QLabel>

namespace Ui {

// A widget to show messages
class MessageWidget : public QWidget
{
    Q_OBJECT
public:
    MessageWidget(const Api::Message& message, bool isFirst, bool separatorBefore, QWidget *parent);

private:
    void enterEvent(QEvent *) override;
    void leaveEvent(QEvent *) override;

    QString hoveredTimestamp; // The timestamp we show
    QLabel *timestampLabel;   // The label of the timestamp
    bool isFirst;             // Is the message is not grouped

};

} // namespace Ui
