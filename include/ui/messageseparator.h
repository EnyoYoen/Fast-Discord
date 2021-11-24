#pragma once

#include <QWidget>
#include <QDate>

namespace Ui {

// The separator between two messages if they are not sent on the same day
class MessageSeparator : public QWidget
{
    Q_OBJECT
public:
    MessageSeparator(QDate date, QWidget *parent);
};

} // namespace Ui
