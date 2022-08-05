#pragma once

#include "ui/common/basicwidgets.h"
#include "ui/settings/components/menubutton.h"

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
    Label *createMenuTitle(char *title);
    Widget *createSeparator();

    QVector<MenuButton *> buttons;
    Widget *scrollWidget;
    MenuButton::ButtonType actualType;
};

} // namespace Ui