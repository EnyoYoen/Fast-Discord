#include "ui/rightcolumn/reaction.h"

#include <QFont>
#include <QHBoxLayout>

namespace Ui {

Reaction::Reaction(Api::RessourceManager *rmp, const Api::Reaction& reactionp, const Api::Snowflake& channelIdp, const Api::Snowflake& messageIdp, QWidget *parent)
    : Widget(parent)
{
    reaction = reactionp;
    channelId = channelIdp;
    messageId = messageIdp;
    rm = rmp;
    count = reaction.count;
    active = reaction.me;

    this->setFixedHeight(Settings::scale(20));
    this->setContentsMargins(Settings::scale(2), Settings::scale(6), Settings::scale(2), Settings::scale(6));
    this->setBorderRadius(Settings::scale(8));
    if (active) {
        this->setBackgroundColor(Settings::BrandExperiment);
        this->setBorderSize(Settings::scale(1));
        this->setBorderColor(Settings::ReactionBorder);
    } else {
        this->setBackgroundColor(Settings::BackgroundTertiary);
    }
    QHBoxLayout *reactionsLayout = new QHBoxLayout(this);
    reactionsLayout->setContentsMargins(0, 0, 0, 0);
    reactionsLayout->setSpacing(Settings::scale(6));

    Label *emoji = new Label(this);
    emoji->setFixedSize(Settings::scale(16), Settings::scale(16));
    if (reaction.emoji.id.value) {
        rm->getImage([emoji](Api::CallbackStruct cb){
            emoji->setImage(*reinterpret_cast<QString *>(cb.data));
        }, "https://cdn.discordapp.com/emojis/" + reaction.emoji.id, QString::number(reaction.emoji.id.value));
    } else {
        emoji->setText(reaction.emoji.name);
    }

    QFont font;
    font.setPixelSize(Settings::scale(Settings::fontScaling - 2));
    font.setFamily("whitney");
    countLabel = new Label(QString::number(reaction.count), this);
    countLabel->setFixedSize(QFontMetrics(font).horizontalAdvance(QString::number(reaction.count)), Settings::scale(Settings::fontScaling - 2));
    countLabel->setTextColor(Settings::InteractiveNormal);

    reactionsLayout->addWidget(emoji, 0, Qt::AlignVCenter);
    reactionsLayout->addWidget(countLabel, 0, Qt::AlignVCenter);
}

void Reaction::mouseReleaseEvent(QMouseEvent *event)
{
    if (active)
        rm->requester->deleteReaction([](Api::CallbackStruct cb){}, channelId, messageId, reaction.emoji.id, reaction.emoji.name);
    else
        rm->requester->addReaction([](Api::CallbackStruct cb){}, channelId, messageId, reaction.emoji.id, reaction.emoji.name);
}

void Reaction::enterEvent(QEvent *)
{
    if (!active) {
        this->setBorderSize(Settings::scale(1));
        this->setBorderColor(Settings::ReactionBorder);
    }
}

void Reaction::leaveEvent(QEvent *)
{
    if (!active)
        this->setBorderSize(0);
}

void Reaction::addReaction(bool me)
{
    if (me) {
        this->setBackgroundColor(Settings::BrandExperiment);
        this->setBorderSize(Settings::scale(1));
        this->setBorderColor(Settings::ReactionBorder);

        active = true;
    }
    countLabel->setText(QString::number(++count));
}

void Reaction::removeReaction(bool me, bool all)
{
    if (all || count == 1) {
        this->deleteLater();
        return;
    }
        
    if (me) {
        this->setBackgroundColor(Settings::BackgroundTertiary);
        this->setBorderSize(Settings::scale(0));

        active = false;
    }
    countLabel->setText(QString::number(--count));
}

} // namespace Ui