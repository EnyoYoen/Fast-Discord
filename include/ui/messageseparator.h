#pragma once

#include <QWidget>
#include <QDate>

namespace Ui {

class MessageSeparator : public QWidget
{
    Q_OBJECT
public:
    MessageSeparator(QDate date, QWidget *parent = nullptr);

};

} // namespace Ui
