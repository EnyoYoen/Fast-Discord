#pragma once

#include <QLabel>

namespace Ui {

class GuildPill : public QLabel
{
public:
    GuildPill(QWidget *parent);
    void setHeight(unsigned int height);
};

} // namespace Ui
