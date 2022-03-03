#include "ui/guildfolder.h"

#include "ui/guildpill.h"

#include <QHBoxLayout>
#include <QGridLayout>

namespace Ui {

GuildFolder::GuildFolder(Api::RessourceManager *rm, const std::vector<Api::Guild *>& guilds, QWidget *parent)
    : QLabel(parent)
{
    QHBoxLayout *closedLayout = new QHBoxLayout(this);
    closedLayout->setSpacing(6);
    closedLayout->setContentsMargins(0, 0, 0, 0);

    closedContent = new QLabel(this);
    QGridLayout *contentLayout = new QGridLayout(closedContent);
    closedContent->setFixedSize(48, 48);
    closedContent->setStyleSheet("background-color: rgba(100, 100, 100, 0.3);"
                                 "border-radius: 16px;");

    pill = new GuildPill(this);
    closedLayout->addWidget(pill);
    closedLayout->addWidget(closedContent);


    openedContent = new QWidget(this);
    openedContent->hide();
    openedContent->setFixedHeight(guilds.size() * (48 + 6) + 48 - 6);
    openedContent->setStyleSheet("background-color: rgba(60, 60, 60, 0.3);"
                                 "border-radius: 16px;");

    closeButton = new QLabel(openedContent);
    closeButton->setFixedSize(72, 48);
    closeButton->setStyleSheet("background-image: url(res/images/svg/folder-icon.svg);"
                               "background-repeat: no-repeat;"
                               "background-position: center;"
                               "background-color: none;");

    QVBoxLayout *openedLayout = new QVBoxLayout(openedContent);
    openedLayout->setSpacing(6);
    openedLayout->setContentsMargins(0, 0, 0, 10);
    openedLayout->addWidget(closeButton);

    for (unsigned int i = 0 ; i < guilds.size() ; i++) {
        Api::Guild actualGuild = *guilds[i];

        GuildWidget *guildWidget = new GuildWidget(rm, *guilds[i], openedContent);
        QObject::connect(guildWidget, SIGNAL(leftClicked(const std::string&)), this, SLOT(propagateGuildClic(const std::string&)));
        openedLayout->addWidget(guildWidget);
        guildWidgets.push_back(guildWidget);

        if (i < 4) {
            contentLayout->addWidget(new GuildIcon(rm, *actualGuild.id, *actualGuild.name, actualGuild.icon, true, closedContent), (i < 2 ? 0 : 1), i%2);
        }
    }

    this->setFixedSize(72, 48);
    this->setContentsMargins(0, 0, 12, 0);
}

void GuildFolder::mouseReleaseEvent(QMouseEvent *event)
{
    if (!opened) {
        this->setFixedHeight(guildWidgets.size() * (48 + 6) + 48 - 6);
        this->setContentsMargins(0, 0, 0, 0);

        closedContent->hide();
        pill->hide();
        openedContent->show();

        opened = !opened;
    } else if (closeButton->contentsRect().contains(event->pos())) {
        this->setFixedSize(72, 48);
        this->setContentsMargins(0, 0, 12, 0);

        openedContent->hide();
        closedContent->show();
        pill->show();

        opened = !opened;
    }
}

void GuildFolder::unclicked()
{
    for (unsigned int i = 0 ; i < guildWidgets.size() ; i++)
        guildWidgets[i]->unclicked();
}

void GuildFolder::unclickedExcept(const std::string& id)
{
    for (size_t i = 0 ; i < guildWidgets.size() ; i++) {
        if (guildWidgets[i]->id != id) {
            guildWidgets[i]->unclicked();
        }
    }
}

void GuildFolder::propagateGuildClic(const std::string& id)
{
    emit guildClicked(id);
}

} // namespace Ui