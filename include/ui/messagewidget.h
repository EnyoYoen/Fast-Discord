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
    MessageWidget(const Api::Message& message, QWidget *parent = nullptr);

/*signals:*/

private:
    QLabel *content;
    QLabel *name;
    QLabel *date;
    RoundedImage *avatar;

};

} // namespace Ui
