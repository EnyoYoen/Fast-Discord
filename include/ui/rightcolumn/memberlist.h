#pragma once

#include "ui/rightcolumn/memberwidget.h"
#include "api/ressourcemanager.h"
#include "api/objects/snowflake.h"
#include "api/objects/guildmember.h"

#include <QScrollArea>

namespace Ui {

class MemberList : public QScrollArea
{
    Q_OBJECT
public:
    MemberList(Api::RessourceManager *rm, const QVector<Api::Snowflake>& recipientIds, QWidget *parent);
    void setMembers(Api::GuildMemberGateway members);

signals:
    void loadMoreMembers();

private:
    void showEvent(QShowEvent *);

    QWidget *createTitle(QString name);
    void userReceiver(void *userPtr);

    QVBoxLayout *layout;

    QVector<MemberWidget *> members;
    QVector<Api::User *> users;
    QVector<Api::Role *> roles;
    QList<QString> groupsIds;
    Api::RessourceManager *rm;
    int nMembers;
    int actualNMembers;
    int tempScrollBarValue;
    int tempScrollBarRange;
    bool emitScrollBarLow;
    bool shown;
};

} // namespace Ui