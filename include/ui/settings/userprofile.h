#pragma once

#include "ui/settings/profile.h"
#include "api/ressourcemanager.h"

#include <QScrollArea>
#include <QTextEdit>

namespace Ui {

class ColorLabel : public QLabel
{
    Q_OBJECT
public:
    ColorLabel(QWidget *parent)
        : QLabel(parent)
    {
        this->setFixedSize(69, 50);
        pressed = false;
        QVBoxLayout *layout = new QVBoxLayout(this);
        icon = new QLabel(this);
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
    void setColor(QString colorp)
    {
        color = colorp;
        this->setStyleSheet("background-color: " + colorp + ";"
                            "border-radius: 4px");
    }

    QString color;
    bool pressed;

signals:
    void clicked();

private:
    QLabel *icon;
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
    QString bannerColor;
    QColor defaultBannerColor;
    bool modified;
};

}