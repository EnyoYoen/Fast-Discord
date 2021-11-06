#pragma once

#include "roundedimage.h"
#include "../api/message.h"

#include <QWidget>
#include <QLabel>

namespace Ui {

class MessageWidget : public QWidget
{
    Q_OBJECT
public:
    MessageWidget(const Api::Message& message, bool isFirst, bool separatorBefore, QWidget *parent = nullptr);

private:
    void enterEvent(QEvent *) override;
    void leaveEvent(QEvent *) override;

    QString hoveredTimestamp;
    QLabel *timestampLabel;
    bool isFirst;

};

} // namespace Ui
