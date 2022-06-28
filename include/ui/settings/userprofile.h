#pragma once

#include "ui/settings/profile.h"
#include "api/ressourcemanager.h"

#include <QLabel>
#include <QScrollArea>
#include <QTextEdit>

namespace Ui {

class ColorLabel : public Widget
{
    Q_OBJECT
public:
    ColorLabel(QWidget *parent)
        : Widget(parent)
    {
        this->setFixedSize(69, 50);
        pressed = false;
        QVBoxLayout *layout = new QVBoxLayout(this);
        icon = new Widget(this);
        icon->setPixmap(QPixmap("res/images/svg/check-icon.svg"));
        icon->hide();
        layout->addWidget(icon, 0, Qt::AlignCenter);
    }
    void setClicked()
    {
        pressed = true;
        icon->show();
    }
    void setUnclicked()
    {
        pressed = false;
        icon->hide();
    }
    void setColor(QColor colorp)
    {
        color = colorp;
        this->setBackgroundColor(colorp);
        this->setBorderRadius(4);
    }

    QColor color;
    bool pressed;

signals:
    void clicked();

private:
    Widget *icon;
    void mouseReleaseEvent(QMouseEvent *event) override
    {
        if (event->button() == Qt::LeftButton) {
            icon->show();
            emit clicked();
            pressed = true;
        }
    }
};

class UserProfile : public QScrollArea
{
    Q_OBJECT
public:
    UserProfile(Api::RessourceManager *rm, QWidget *parent);

private:
    void profileChanged();

    QVBoxLayout *layout;
    Profile *profile;
    QTextEdit *aboutTextEdit;
    ColorLabel *customColor;
    ColorLabel *defaultColor;

    Api::RessourceManager *rm;
    QString bio;
    QString avatarFile;
    QString bannerFile;
    QColor bannerColor;
    QColor defaultBannerColor;
    bool modified;
};

}