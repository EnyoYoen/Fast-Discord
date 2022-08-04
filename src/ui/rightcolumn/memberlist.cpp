#include "ui/rightcolumn/memberlist.h"

#include "ui/common/basicwidgets.h"

#include <QVBoxLayout>
#include <QScrollBar>

#include <algorithm>

namespace Ui {

MemberList::MemberList(Api::RessourceManager *rmp, const QVector<Api::Snowflake>& recipientIds, QWidget *parent)
    : QScrollArea()
{
    guildId = 0;
    emitScrollBarLow = true;
    shown = false;
    rm = rmp;

    QWidget *container = new QWidget(this);
    layout = new QVBoxLayout(container);
    layout->setContentsMargins(Settings::scale(8), 0, Settings::scale(8), 0);
    layout->setSpacing(Settings::scale(2));

    if (!recipientIds.isEmpty()) {
        nMembers = recipientIds.size() + 1;
        actualNMembers = 0;

        layout->addWidget(createTitle("MEMBERS - " + QString::number(nMembers)), 0, Qt::AlignLeft);

        rm->getClient([this](void *clientPtr){
            Api::Client *client = reinterpret_cast<Api::Client *>(clientPtr);
            Api::User user{client->username, client->discriminator, client->avatar, client->banner, client->locale, client->email, client->id, client->accentColor, client->flags, client->purchasedFlags, client->publicFlags, (optbool)Optional::None, (optbool)Optional::None, (optbool)Optional::None, (optbool)Optional::None};
            userReceiver(reinterpret_cast<void *>(new Api::User(user)));
        });

        for (int i = 0 ; i < recipientIds.size() ; i++) {
            rm->getUser([this](void *userPtr){userReceiver(userPtr);}, recipientIds[i]);
        }

        layout->addStretch(1);
    }

    QObject::connect(this->verticalScrollBar(), &QScrollBar::valueChanged, [this](int value){
        tempScrollBarValue = value;
        tempScrollBarRange = this->verticalScrollBar()->maximum() - this->verticalScrollBar()->minimum();
        if (value > tempScrollBarRange * 0.9 && emitScrollBarLow && shown) {
            emitScrollBarLow = false;
            emit loadMoreMembers();
        }
    });
    QObject::connect(this->verticalScrollBar(), &QScrollBar::rangeChanged, [this](int min, int max){
        if (shown) {
            this->verticalScrollBar()->setValue(max - min - tempScrollBarRange + tempScrollBarValue);
            emitScrollBarLow = true;
        }
    });
    
    this->setFixedWidth(Settings::scale(240));
    this->setWidget(container);
    this->setWidgetResizable(true);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setStyleSheet("* {background-color:" + Settings::colors[Settings::BackgroundSecondary].name() + "; border: none;}"
                        "QScrollBar::handle:vertical {border: none; border-radius: " + QString::number(Settings::scale(2)) + "px; background-color: " + Settings::colors[Settings::BackgroundTertiary].name() + ";}"
                        "QScrollBar:vertical {border: none; background-color: " + Settings::colors[Settings::BackgroundSecondary].name() + "; border-radius: " + QString::number(Settings::scale(2)) + "px; width: " + QString::number(Settings::scale(4)) + "px;}"
                        "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {border:none; background: none; height: 0;}"
                        "QScrollBar:left-arrow:vertical, QScrollBar::right-arrow:vertical {background: none;}"
                        "QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {background: none;}");
}

QWidget *MemberList::createTitle(QString name)
{
    QWidget *titleContainer = new QWidget(this);
    titleContainer->setFixedHeight(Settings::scale(40));
    QHBoxLayout *layout = new QHBoxLayout(titleContainer);
    layout->setContentsMargins(0, Settings::scale(24), 0, 0);

    QFont font;
    font.setBold(true);
    font.setPixelSize(Settings::scale(12));
    font.setFamily("whitney");
    Label *title = new Label(name, titleContainer);
    title->setFixedSize(QFontMetrics(font).horizontalAdvance(name), Settings::scale(16));
    title->setFont(font);
    title->setTextColor(Settings::ChannelsDefault);

    layout->addWidget(title);

    return titleContainer;
}

void MemberList::userReceiver(void *userPtr)
{
    actualNMembers++;
    users.append(reinterpret_cast<Api::User *>(userPtr));
    if (actualNMembers == nMembers) {
        std::sort(users.begin(), users.end(),
            [](const Api::User *a, const Api::User *b) {
                return a->username < b->username;
            });
        for (int i = 0 ; i < nMembers ; i++) {
            layout->insertWidget(1 + i, new MemberWidget(rm, users[i], QColor(), this));
        }
    }
}

void MemberList::setMembers(Api::GuildMemberGateway members)
{
    for (int i = 0 ; i < members.structs.size() ; i++) {
        if (members.ops[i] == Api::GuildMemberOp::Sync) {
            guildId = members.guildId;
            rm->getGuilds([members, i, this](void *guildsPtr){
                QVector<Api::Guild *> guilds = *reinterpret_cast<QVector<Api::Guild *> *>(guildsPtr);
                for (int i = 0 ; i < guilds.size() ; i++) {
                    if (guilds[i]->id == members.guildId) {
                        roles = guilds[i]->roles;
                        break;
                    }
                }

                Api::GuildMemberSync *sync = reinterpret_cast<Api::GuildMemberSync *>(members.structs[i]);

                groupsIds = sync->groupsMembers.keys();
                std::sort(groupsIds.begin(), groupsIds.end(), [this](QString a, QString b) -> bool {
                    if (a == "online" && b == "offline") {
                        return true;
                    } else if (a == "offline" && b == "online") {
                        return false;
                    } else if (a == "offline" || a == "online") {
                        return false;
                    } else if (b == "offline" || b == "online") {
                        return true;
                    }

                    for (int i = 0 ; i < roles.size() ; i++) {
                        if (a == roles[i]->id) {
                            for (int j = 0 ; j < roles.size() ; j++) {
                                if (b == roles[j]->id) {
                                    return roles[i]->position > roles[j]->position;
                                }
                            }
                        }
                    }

                    return true; // Should not be reached
                });
                
                for (int i = 0 ; i < groupsIds.size() ; i++) {
                    QColor color;
                    bool found = false;
                    for (int j = 0 ; j < roles.size() ; j++) {
                        if (groupsIds[i] == roles[j]->id) {
                            color = QColor((roles[j]->color & 0x00FF0000) >> 16, (roles[j]->color & 0x0000FF00) >> 8, roles[j]->color & 0x000000FF);
                            color = QColor::fromHsv(color.hue(), color.saturation() * (Settings::customColorSaturation ? Settings::saturation : 1.0f), color.value());
                            layout->addWidget(createTitle(roles[j]->name.toUpper() + " - " + QString::number(sync->groupsCount[groupsIds[i]])), 0, Qt::AlignLeft);
                            found = true;
                            break;
                        }
                    }
                    if (!found) {
                        layout->addWidget(createTitle(groupsIds[i].toUpper() + " - " + QString::number(sync->groupsCount[groupsIds[i]])), 0, Qt::AlignLeft);
                    }

                    QVector<Api::GuildMember *> groupMembers = sync->groupsMembers[groupsIds[i]];
                    for (int j = 0 ; j < groupMembers.size() ; j++) {
                        QColor colorTmp = color;
                        if (!color.isValid()) {
                            Api::GuildMember *member = groupMembers[j];
                            rm->getGuilds([members, member, &colorTmp](void *guildsPtr){
                                QVector<Api::Guild *> guilds = *reinterpret_cast<QVector<Api::Guild *> *>(guildsPtr);
                                QVector<Api::Snowflake> rolesIds = member->roles;
                                for (int i = 0 ; i < guilds.size() ; i++) {
                                    if (guilds[i]->id == members.guildId) {
                                        QVector<Api::Role *> guildRoles = guilds[i]->roles;
                                        Api::Role *highestRole = nullptr;
                                        for (int j = 0 ; j < guildRoles.size() ; j++) {
                                            for (int k = 0 ; k < rolesIds.size() ; k++) {
                                                if (guildRoles[j]->id == QString::number(rolesIds[k].value) && (highestRole == nullptr || highestRole->position < guildRoles[j]->position))
                                                    highestRole = guildRoles[j];
                                            }
                                        }
                                        if (highestRole) {
                                            qint32 colorInt = highestRole->color;
                                            colorTmp = QColor((colorInt & 0x00FF0000) >> 16, (colorInt & 0x0000FF00) >> 8, (colorInt & 0x000000FF));
                                            colorTmp = QColor::fromHsv(colorTmp.hue(), colorTmp.saturation() * (Settings::customColorSaturation ? Settings::saturation : 1.0f), colorTmp.value());
                                        }
                                        break;
                                    }
                                }
                            });
                        }
                        MemberWidget *a = new MemberWidget(rm, groupMembers[j]->user, (members.guildId == 0 ? QColor() : color), this);
                        a->setStatus(groupMembers[j]->presence->status);
                        layout->addWidget(a);
                    }
                }
                layout->addStretch(1);

                this->verticalScrollBar()->setValue(0);
            });
        } else {
            if (members.guildId != guildId) return;

            if (members.ops[i] == Api::GuildMemberOp::Update) {
                Api::GuildMemberUpdate *update = reinterpret_cast<Api::GuildMemberUpdate *>(members.structs[i]);
                QLayoutItem *item = layout->itemAt(update->index);
                layout->removeItem(item);
                item->widget()->deleteLater();

                if (update->memberOrGroup->member) {
                    Api::GuildMember *member = reinterpret_cast<Api::GuildMember *>(update->memberOrGroup->content);
                    rm->getGuilds([this, update, members, member](void *guildsPtr){
                        QVector<Api::Guild *> guilds = *reinterpret_cast<QVector<Api::Guild *> *>(guildsPtr);
                        QVector<Api::Snowflake> rolesIds = member->roles;
                        for (int i = 0 ; i < guilds.size() ; i++) {
                            if (guilds[i]->id == members.guildId) {
                                QVector<Api::Role *> guildRoles = guilds[i]->roles;
                                Api::Role *highestRole = nullptr;
                                for (int j = 0 ; j < guildRoles.size() ; j++) {
                                    for (int k = 0 ; k < rolesIds.size() ; k++) {
                                        if (guildRoles[j]->id == QString::number(rolesIds[k].value) && (highestRole == nullptr || highestRole->position < guildRoles[j]->position))
                                            highestRole = guildRoles[j];
                                    }
                                }
                                if (highestRole) {
                                    qint32 colorInt = highestRole->color;
                                    QColor colorTmp((colorInt & 0x00FF0000) >> 16, (colorInt & 0x0000FF00) >> 8, (colorInt & 0x000000FF));
                                    MemberWidget *memberWidget = new MemberWidget(rm, member->user, (members.guildId == 0 ? QColor() : QColor::fromHsv(colorTmp.hue(), colorTmp.saturation() * (Settings::customColorSaturation ? Settings::saturation : 1.0f), colorTmp.value())), this);
                                    memberWidget->setStatus(member->presence->status);
                                    layout->insertWidget(update->index, memberWidget);
                                }
                                break;
                            }
                        }
                    });
                } else {
                    Api::GuildGroup *group = reinterpret_cast<Api::GuildGroup *>(update->memberOrGroup->content);

                    QColor color;
                    bool found = false;
                    for (int i = 0 ; i < roles.size() ; i++) {
                        if (group->id == roles[i]->id) {
                            color = QColor((roles[i]->color & 0x00FF0000) >> 16, (roles[i]->color & 0x0000FF00) >> 8, roles[i]->color & 0x000000FF);
                            color = QColor::fromHsv(color.hue(), color.saturation() * (Settings::customColorSaturation ? Settings::saturation : 1.0f), color.value());
                            layout->insertWidget(update->index, createTitle(roles[i]->name.toUpper() + " - " + QString::number(group->count)), 0, Qt::AlignLeft);
                            found = true;
                            break;
                        }
                    }
                    if (!found) {
                        layout->insertWidget(update->index, createTitle(group->id.toUpper() + " - " + QString::number(group->count)), 0, Qt::AlignLeft);
                    }
                }
            } else if (members.ops[i] == Api::GuildMemberOp::Delete) {
                Api::GuildMemberDelete *deletes = reinterpret_cast<Api::GuildMemberDelete *>(members.structs[i]);
                QLayoutItem *item = layout->itemAt(deletes->index);
                layout->removeItem(item);
                item->widget()->deleteLater();
            } else if (members.ops[i] == Api::GuildMemberOp::Insert) {
                Api::GuildMemberInsert *insert = reinterpret_cast<Api::GuildMemberInsert *>(members.structs[i]);

                if (insert->memberOrGroup->member) {
                    Api::GuildMember *member = reinterpret_cast<Api::GuildMember *>(insert->memberOrGroup->content);
                    rm->getGuilds([this, insert, members, member](void *guildsPtr){
                        QVector<Api::Guild *> guilds = *reinterpret_cast<QVector<Api::Guild *> *>(guildsPtr);
                        QVector<Api::Snowflake> rolesIds = member->roles;
                        for (int i = 0 ; i < guilds.size() ; i++) {
                            if (guilds[i]->id == members.guildId) {
                                QVector<Api::Role *> guildRoles = guilds[i]->roles;
                                Api::Role *highestRole = nullptr;
                                for (int j = 0 ; j < guildRoles.size() ; j++) {
                                    for (int k = 0 ; k < rolesIds.size() ; k++) {
                                        if (guildRoles[j]->id == QString::number(rolesIds[k].value) && (highestRole == nullptr || highestRole->position < guildRoles[j]->position))
                                            highestRole = guildRoles[j];
                                    }
                                }
                                if (highestRole) {
                                    qint32 colorInt = highestRole->color;
                                    QColor colorTmp((colorInt & 0x00FF0000) >> 16, (colorInt & 0x0000FF00) >> 8, (colorInt & 0x000000FF));
                                    MemberWidget *memberWidget = new MemberWidget(rm, member->user, (members.guildId == 0 ? QColor() : QColor::fromHsv(colorTmp.hue(), colorTmp.saturation() * (Settings::customColorSaturation ? Settings::saturation : 1.0f), colorTmp.value())), this);
                                    memberWidget->setStatus(member->presence->status);
                                    layout->insertWidget(insert->index, memberWidget);
                                }
                                break;
                            }
                        }
                    });
                } else {
                    Api::GuildGroup *group = reinterpret_cast<Api::GuildGroup *>(insert->memberOrGroup->content);

                    QColor color;
                    bool found = false;
                    for (int i = 0 ; i < roles.size() ; i++) {
                        if (group->id == roles[i]->id) {
                            color = QColor((roles[i]->color & 0x00FF0000) >> 16, (roles[i]->color & 0x0000FF00) >> 8, roles[i]->color & 0x000000FF);
                            color = QColor::fromHsv(color.hue(), color.saturation() * (Settings::customColorSaturation ? Settings::saturation : 1.0f), color.value());
                            layout->insertWidget(insert->index, createTitle(roles[i]->name.toUpper() + " - " + QString::number(group->count)), 0, Qt::AlignLeft);
                            found = true;
                            break;
                        }
                    }
                    if (!found) {
                        layout->insertWidget(insert->index, createTitle(group->id.toUpper() + " - " + QString::number(group->count)), 0, Qt::AlignLeft);
                    }
                }
            }
        }
    }
}

void MemberList::showEvent(QShowEvent *)
{
    shown = true;
}

} // namespace Ui