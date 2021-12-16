#pragma once

#include <QWidget>

namespace Ui {

class GuildPill : public QWidget
{
public:
    GuildPill(QWidget *parent);
    void setHeight(unsigned int height);

private:
    void paintEvent(QPaintEvent *) override;
};

} // namespace Ui
