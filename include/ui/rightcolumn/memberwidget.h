#pragma once

#include "ui/common/basicwidgets.h"
#include "ui/common/roundedimage.h"
#include "ui/common/statusicon.h"

namespace Ui {

class MemberWidget : public Widget
{
    Q_OBJECT
public:
    MemberWidget(Api::RessourceManager *rm, Api::User *user, QColor color, QWidget *parent);
    ~MemberWidget();
    void setStatus(const QString& status);

    Api::User *user;

signals:
    void leftClicked(Api::User *);
    void rightClicked(Api::User *);

private:
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *) override;
    void enterEvent(QEvent *) override;
    void leaveEvent(QEvent *) override;
    void setIcon(const QString& iconFileName);

    Api::RessourceManager *rm;
    QString avatarUrl;

    QHBoxLayout         *mainLayout;
    RoundedImage        *icon;
    StatusIcon          *statusIcon;
    Label               *subtext;
    Widget              *statusBackground;
    Label               *name;
};

} // namespace Ui