#pragma once

#include "ui/settings/menubutton.h"

#include "QScrollArea"

namespace Ui {

class ScrollMenu : public QScrollArea
{
    Q_OBJECT
public:
    ScrollMenu(QWidget *parent);

signals:
    void buttonClicked(MenuButton::ButtonType type);

private slots:
    void resetButtons(MenuButton::ButtonType type);

private:
    QLabel *createMenuTitle(char *title);
    QWidget *createSeparator();

    QVector<MenuButton *> buttons;
    QWidget *scrollWidget;
    MenuButton::ButtonType actualType;
};

} // namespace Ui