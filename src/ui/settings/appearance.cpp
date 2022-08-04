#include "ui/settings/appearance.h"

#include "ui/settings/slider.h"
#include "ui/settings/radiobutton.h"
#include "ui/settings/switchbutton.h"
#include "ui/settings/settingsmenu.h"
#include "ui/settings/popup.h"
#include "ui/rightcolumn/messagearea.h"

#include <QScrollBar>
#include <QStackedLayout>

namespace Ui {

class MessageAreaBorder : public Widget
{
public:
    MessageAreaBorder(MessageArea *messageAreap, QWidget *parent) : messageArea(messageAreap), Widget(parent) {};

private:
    void showEvent(QShowEvent *event) {
        QScrollBar *vert = messageArea->verticalScrollBar();
        vert->setValue((vert->maximum() - vert->minimum()) / 2);
    };
    void paintEvent(QPaintEvent *event) {
        QScrollBar *vert = messageArea->verticalScrollBar();
        vert->setValue((vert->maximum() - vert->minimum()) / 2);
        Widget::paintEvent(event);
    };

    MessageArea *messageArea;
};

Appearance::Appearance(Api::RessourceManager *rm, QWidget *parent)
    : QScrollArea(parent)
{
    Widget *container = new Widget(this);
    container->setMaximumWidth(Settings::scale(740));
    container->setContentsMargins(Settings::scale(40), Settings::scale(60), Settings::scale(40), Settings::scale(80));
    container->setBackgroundColor(Settings::BackgroundPrimary);
    QVBoxLayout *layout = new QVBoxLayout(container);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(Settings::scale(8));


    QFont font;
    font.setPixelSize(Settings::scale(20));
    font.setFamily("whitney");

    Label *title = new Label("Appearance", container);
    title->setFixedSize(QFontMetrics(font).horizontalAdvance("Appearance"), Settings::scale(20));
    title->setFont(font);
    title->setTextColor(Settings::HeaderPrimary);

    layout->addWidget(title);
    layout->addSpacing(Settings::scale(20));


    Widget *messageAreaContainer = new Widget(container);

    MessageArea *messageArea = new MessageArea(rm, messageAreaContainer);

    MessageAreaBorder *messageAreaBorder = new MessageAreaBorder(messageArea, messageAreaContainer);
    messageAreaBorder->setBorderSize(Settings::scale(1));
    messageAreaBorder->setBorderColor(Settings::BackgroundTertiary);
    messageAreaBorder->setBorderRadius(Settings::scale(5));
    messageAreaBorder->setFixedHeight(Settings::scale(180));
    
    QStackedLayout *stackLayout = new QStackedLayout(messageAreaContainer);
    stackLayout->setStackingMode(QStackedLayout::StackingMode::StackAll);
    stackLayout->setSpacing(0);
    stackLayout->setContentsMargins(0, 0, 0, 0);
    stackLayout->addWidget(messageAreaBorder);
    stackLayout->addWidget(messageArea);

    messageArea->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
    rm->getClient([messageArea](void *clientPtr){
        Api::Client *client = reinterpret_cast<Api::Client *>(clientPtr);
        Api::User author{client->username, client->discriminator, client->avatar, client->banner, client->locale, client->email, client->id, client->accentColor, client->flags, client->purchasedFlags, client->publicFlags, 0, 0, 2, 2};
        messageArea->setMessages(QVector<Api::Message *>{
            new Api::Message{author, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, QVector<Api::Reaction *>(), QVector<Api::Embed *>(), QVector<Api::User *>(), QVector<Api::Attachment *>(), QVector<Api::ChannelMention *>(), QVector<QString>(), QVector<Api::MessageComponent *>(), QVector<Api::StickerItem *>(), QVector<Api::Sticker *>(), 
            "Oh here it is!", QDateTime::currentDateTime().toString(Qt::DateFormat::ISODate), QString(), QString(), 0, 0, 0, 0, 0, 0, author.publicFlags, Api::MessageType::Default, 0, false, false, false},
            new Api::Message{author, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, QVector<Api::Reaction *>(), QVector<Api::Embed *>(), QVector<Api::User *>(), QVector<Api::Attachment *>(), QVector<Api::ChannelMention *>(), QVector<QString>(), QVector<Api::MessageComponent *>(), QVector<Api::StickerItem *>(), QVector<Api::Sticker *>(), 
            "Compact mode would be turned on", QDateTime::currentDateTime().addSecs(-600).toString(Qt::DateFormat::ISODate), QString(), QString(), 0, 0, 0, 0, 0, 0, author.publicFlags, Api::MessageType::Default, 0, false, false, false},
            new Api::Message{author, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, QVector<Api::Reaction *>(), QVector<Api::Embed *>(), QVector<Api::User *>(), QVector<Api::Attachment *>(), QVector<Api::ChannelMention *>(), QVector<QString>(), QVector<Api::MessageComponent *>(), QVector<Api::StickerItem *>(), QVector<Api::Sticker *>(), 
            "Waiting for the day when", QDateTime::currentDateTime().addSecs(-600).toString(Qt::DateFormat::ISODate), QString(), QString(), 0, 0, 0, 0, 0, 0, author.publicFlags, Api::MessageType::Default, 0, false, false, false},
            new Api::Message{author, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, QVector<Api::Reaction *>(), QVector<Api::Embed *>(), QVector<Api::User *>(), QVector<Api::Attachment *>(), QVector<Api::ChannelMention *>(), QVector<QString>(), QVector<Api::MessageComponent *>(), QVector<Api::StickerItem *>(), QVector<Api::Sticker *>(), 
            (Settings::actualTheme == Settings::Theme::Dark ? "Fluttering in the moonlight 🌝" : "Fluttering in the sunlight 🌞"), QDateTime::currentDateTime().addSecs(-1200).toString(Qt::DateFormat::ISODate), QString(), QString(), 0, 0, 0, 0, 0, 0, author.publicFlags, Api::MessageType::Default, 0, false, false, false},
            new Api::Message{author, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, QVector<Api::Reaction *>(), QVector<Api::Embed *>(), QVector<Api::User *>(), QVector<Api::Attachment *>(), QVector<Api::ChannelMention *>(), QVector<QString>(), QVector<Api::MessageComponent *>(), QVector<Api::StickerItem *>(), QVector<Api::Sticker *>(), 
            "Look at me I'm a beautiful butterfly", QDateTime::currentDateTime().addSecs(-1200).toString(Qt::DateFormat::ISODate), QString(), QString(), 0, 0, 0, 0, 0, 0, author.publicFlags, Api::MessageType::Default, 0, false, false, false},
        });
    });

    layout->addWidget(messageAreaContainer);
    layout->addSpacing(Settings::scale(48));


    font.setPixelSize(Settings::scale(12));
    Label *themeTitle = new Label("THEME", container);
    themeTitle->setTextColor(Settings::HeaderSecondary);
    themeTitle->setFont(font);
    themeTitle->setFixedSize(QFontMetrics(font).horizontalAdvance("THEME"), 16);

    RadioGroup *themeRadio = new RadioGroup(QVector<RadioParameters>{
        RadioParameters{"Dark", QString(), Settings::None},
        RadioParameters{"Light", QString(), Settings::None},
        RadioParameters{"Sync with computer", QString(), Settings::None},
    }, Settings::theme == Settings::Theme::Dark ? 0 : Settings::theme == Settings::Theme::Light ? 1 : 2, container);
    QObject::connect(themeRadio, &RadioGroup::clicked, [this, rm, messageArea](int index){
        if (index != 2) {
            QString actualTheme;
            actualTheme = (index == 0 ? "dark" : "light");
            rm->requester->setSettings([](void *){}, "{\"theme\":\"" + actualTheme + "\"}");
        }

        if (index == 0) Settings::setTheme(Settings::Theme::Dark);
        else if (index == 1) Settings::setTheme(Settings::Theme::Light);
        else Settings::setTheme(Settings::Theme::Sync);
        
        Settings::saveSettings();

        messageArea->updateTheme();

        QWidget *parentWidget = this;
        while (parentWidget->parent()) parentWidget = (Widget *)parentWidget->parent();
        parentWidget->update();

        reinterpret_cast<SettingsMenu *>(this->parent())->updateTheme();
        
        this->setStyleSheet("* {border: none; background-color: " + Settings::colors[Settings::BackgroundPrimary].name() + "}"
                            "QScrollBar::handle:vertical {border: none; border-radius: " + QString::number(Settings::scale(2)) + "px; background-color: " + Settings::colors[Settings::BackgroundTertiary].name() + ";}"
                            "QScrollBar:vertical {border: none; background-color: " + Settings::colors[Settings::BackgroundSecondary].name() + "; border-radius: " + QString::number(Settings::scale(8)) + "px; width: " + QString::number(Settings::scale(3)) + "px;}"
                            "QScrollBar::add-line, QScrollBar::sub-line {border:none; background: none; height: 0;}"
                            "QScrollBar:left-arrow:vertical, QScrollBar::right-arrow:vertical {background: none;}"
                            "QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {background: none;}");

        rm->getClient([messageArea](void *clientPtr){
            Api::Client *client = reinterpret_cast<Api::Client *>(clientPtr);
            Api::User author{client->username, client->discriminator, client->avatar, client->banner, client->locale, client->email, client->id, client->accentColor, client->flags, client->purchasedFlags, client->publicFlags, 0, 0, 2, 2};
            messageArea->clear();
            messageArea->setMessages(QVector<Api::Message *>{
                new Api::Message{author, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, QVector<Api::Reaction *>(), QVector<Api::Embed *>(), QVector<Api::User *>(), QVector<Api::Attachment *>(), QVector<Api::ChannelMention *>(), QVector<QString>(), QVector<Api::MessageComponent *>(), QVector<Api::StickerItem *>(), QVector<Api::Sticker *>(), 
                "Oh here it is!", QDateTime::currentDateTime().toString(Qt::DateFormat::ISODate), QString(), QString(), 0, 0, 0, 0, 0, 0, author.publicFlags, Api::MessageType::Default, 0, false, false, false},
                new Api::Message{author, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, QVector<Api::Reaction *>(), QVector<Api::Embed *>(), QVector<Api::User *>(), QVector<Api::Attachment *>(), QVector<Api::ChannelMention *>(), QVector<QString>(), QVector<Api::MessageComponent *>(), QVector<Api::StickerItem *>(), QVector<Api::Sticker *>(), 
                "Compact mode would be turned on", QDateTime::currentDateTime().addSecs(-600).toString(Qt::DateFormat::ISODate), QString(), QString(), 0, 0, 0, 0, 0, 0, author.publicFlags, Api::MessageType::Default, 0, false, false, false},
                new Api::Message{author, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, QVector<Api::Reaction *>(), QVector<Api::Embed *>(), QVector<Api::User *>(), QVector<Api::Attachment *>(), QVector<Api::ChannelMention *>(), QVector<QString>(), QVector<Api::MessageComponent *>(), QVector<Api::StickerItem *>(), QVector<Api::Sticker *>(), 
                "Waiting for the day when", QDateTime::currentDateTime().addSecs(-600).toString(Qt::DateFormat::ISODate), QString(), QString(), 0, 0, 0, 0, 0, 0, author.publicFlags, Api::MessageType::Default, 0, false, false, false},
                new Api::Message{author, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, QVector<Api::Reaction *>(), QVector<Api::Embed *>(), QVector<Api::User *>(), QVector<Api::Attachment *>(), QVector<Api::ChannelMention *>(), QVector<QString>(), QVector<Api::MessageComponent *>(), QVector<Api::StickerItem *>(), QVector<Api::Sticker *>(), 
                (Settings::actualTheme == Settings::Theme::Dark ? "Fluttering in the moonlight 🌝" : "Fluttering in the sunlight 🌞"), QDateTime::currentDateTime().addSecs(-1200).toString(Qt::DateFormat::ISODate), QString(), QString(), 0, 0, 0, 0, 0, 0, author.publicFlags, Api::MessageType::Default, 0, false, false, false},
                new Api::Message{author, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, QVector<Api::Reaction *>(), QVector<Api::Embed *>(), QVector<Api::User *>(), QVector<Api::Attachment *>(), QVector<Api::ChannelMention *>(), QVector<QString>(), QVector<Api::MessageComponent *>(), QVector<Api::StickerItem *>(), QVector<Api::Sticker *>(), 
                "Look at me I'm a beautiful butterfly", QDateTime::currentDateTime().addSecs(-1200).toString(Qt::DateFormat::ISODate), QString(), QString(), 0, 0, 0, 0, 0, 0, author.publicFlags, Api::MessageType::Default, 0, false, false, false},
            });
        });
    });

    Widget *sidebarContainer = new Widget(container);
    sidebarContainer->setFixedHeight(Settings::scale(24));
    QHBoxLayout *sidebarLayout = new QHBoxLayout(sidebarContainer);
    sidebarLayout->setSpacing(0);
    sidebarLayout->setContentsMargins(0, 0, 0, 0);

    font.setPixelSize(Settings::scale(16));
    Label *sidebarTitle = new Label("Dark Sidebar", container);
    sidebarTitle->setTextColor(Settings::HeaderPrimary);
    sidebarTitle->setFont(font);
    sidebarTitle->setFixedSize(QFontMetrics(font).horizontalAdvance("Dark Sidebar"), Settings::scale(24));

    SwitchButton *sidebarSwitch = new SwitchButton(false);

    sidebarLayout->addWidget(sidebarTitle);
    sidebarLayout->addStretch(1);
    sidebarLayout->addWidget(sidebarSwitch);

    Widget *separator1 = new Widget(container);
    separator1->setBackgroundColor(Settings::BackgroundModifierAccent);
    separator1->setFixedHeight(1);
    
    layout->addWidget(themeTitle);
    layout->addSpacing(Settings::scale(8));
    layout->addWidget(themeRadio);
    layout->addSpacing(Settings::scale(20));
    layout->addWidget(sidebarContainer);
    layout->addSpacing(Settings::scale(20));
    layout->addWidget(separator1);
    layout->addSpacing(Settings::scale(20));


    font.setPixelSize(Settings::scale(12));
    Label *displayTitle = new Label("MESSAGE DISPLAY", container);
    displayTitle->setTextColor(Settings::HeaderSecondary);
    displayTitle->setFont(font);
    displayTitle->setFixedSize(QFontMetrics(font).horizontalAdvance("MESSAGE DISPLAY"), Settings::scale(16));

    RadioGroup *displayRadio = new RadioGroup(QVector<RadioParameters>{
        RadioParameters{"Cozy: Modern, beautiful, and easy on your eyes.", QString(), Settings::None},
        RadioParameters{"Compact: Fit more messages on screen at one time. #IRC", QString(), Settings::None},
    }, 0, container);
    QObject::connect(displayRadio, &RadioGroup::clicked, [this, rm, messageArea](int index){
        Settings::compactModeEnabled = (index == 1);
        Settings::saveSettings();

        if (index == 0)
            rm->requester->setSettingsProto([](void *){}, "MjsiCgoIT05fQ0xJQ0tKAggBUgIIAVoCCAFiAggBagIIAXICCAF6AIIBAggBigEAmgECCAKiAQCqAQIIAQ==");
        else
            rm->requester->setSettingsProto([](void *){}, "Mj0iCgoIT05fQ0xJQ0tKAggBUgIIAVoCCAFiAggBagIIAXICCAF6AIIBAggBigECCAGaAQIIAqIBAKoBAggB");

        rm->getClient([messageArea](void *clientPtr){
            Api::Client *client = reinterpret_cast<Api::Client *>(clientPtr);
            Api::User author{client->username, client->discriminator, client->avatar, client->banner, client->locale, client->email, client->id, client->accentColor, client->flags, client->purchasedFlags, client->publicFlags, 0, 0, 2, 2};
            messageArea->clear();
            messageArea->setMessages(QVector<Api::Message *>{
                new Api::Message{author, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, QVector<Api::Reaction *>(), QVector<Api::Embed *>(), QVector<Api::User *>(), QVector<Api::Attachment *>(), QVector<Api::ChannelMention *>(), QVector<QString>(), QVector<Api::MessageComponent *>(), QVector<Api::StickerItem *>(), QVector<Api::Sticker *>(), 
                "Oh here it is!", QDateTime::currentDateTime().toString(Qt::DateFormat::ISODate), QString(), QString(), 0, 0, 0, 0, 0, 0, author.publicFlags, Api::MessageType::Default, 0, false, false, false},
                new Api::Message{author, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, QVector<Api::Reaction *>(), QVector<Api::Embed *>(), QVector<Api::User *>(), QVector<Api::Attachment *>(), QVector<Api::ChannelMention *>(), QVector<QString>(), QVector<Api::MessageComponent *>(), QVector<Api::StickerItem *>(), QVector<Api::Sticker *>(), 
                "Compact mode would be turned on", QDateTime::currentDateTime().addSecs(-600).toString(Qt::DateFormat::ISODate), QString(), QString(), 0, 0, 0, 0, 0, 0, author.publicFlags, Api::MessageType::Default, 0, false, false, false},
                new Api::Message{author, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, QVector<Api::Reaction *>(), QVector<Api::Embed *>(), QVector<Api::User *>(), QVector<Api::Attachment *>(), QVector<Api::ChannelMention *>(), QVector<QString>(), QVector<Api::MessageComponent *>(), QVector<Api::StickerItem *>(), QVector<Api::Sticker *>(), 
                "Waiting for the day when", QDateTime::currentDateTime().addSecs(-600).toString(Qt::DateFormat::ISODate), QString(), QString(), 0, 0, 0, 0, 0, 0, author.publicFlags, Api::MessageType::Default, 0, false, false, false},
                new Api::Message{author, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, QVector<Api::Reaction *>(), QVector<Api::Embed *>(), QVector<Api::User *>(), QVector<Api::Attachment *>(), QVector<Api::ChannelMention *>(), QVector<QString>(), QVector<Api::MessageComponent *>(), QVector<Api::StickerItem *>(), QVector<Api::Sticker *>(), 
                (Settings::actualTheme == Settings::Theme::Dark ? "Fluttering in the moonlight 🌝" : "Fluttering in the sunlight 🌞"), QDateTime::currentDateTime().addSecs(-1200).toString(Qt::DateFormat::ISODate), QString(), QString(), 0, 0, 0, 0, 0, 0, author.publicFlags, Api::MessageType::Default, 0, false, false, false},
                new Api::Message{author, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, QVector<Api::Reaction *>(), QVector<Api::Embed *>(), QVector<Api::User *>(), QVector<Api::Attachment *>(), QVector<Api::ChannelMention *>(), QVector<QString>(), QVector<Api::MessageComponent *>(), QVector<Api::StickerItem *>(), QVector<Api::Sticker *>(), 
                "Look at me I'm a beautiful butterfly", QDateTime::currentDateTime().addSecs(-1200).toString(Qt::DateFormat::ISODate), QString(), QString(), 0, 0, 0, 0, 0, 0, author.publicFlags, Api::MessageType::Default, 0, false, false, false},
            });

            QScrollBar *vert = messageArea->verticalScrollBar();
            vert->setValue((vert->maximum() - vert->minimum()) / 2);
        });
    });

    Widget *separator2 = new Widget(container);
    separator2->setBackgroundColor(Settings::BackgroundModifierAccent);
    separator2->setFixedHeight(1);
    
    layout->addWidget(displayTitle);
    layout->addSpacing(Settings::scale(8));
    layout->addWidget(displayRadio);
    layout->addSpacing(Settings::scale(20));
    layout->addWidget(separator2);
    layout->addSpacing(Settings::scale(8));

    
    Label *scalingTitle = new Label("CHAT FONT SCALING", container);
    scalingTitle->setTextColor(Settings::HeaderSecondary);
    scalingTitle->setFont(font);
    scalingTitle->setFixedSize(QFontMetrics(font).horizontalAdvance("CHAT FONT SCALING"), 16);
    
    Slider *fontScaling = new Slider(QVector<int>{12, 14, 15, 16, 18, 20, 24}, "px", 16, Settings::fontScaling, container);
    layout->addWidget(fontScaling);
    QObject::connect(fontScaling, &Slider::valueChanged, [messageArea](int value){
        Settings::fontScaling = value;
        Settings::saveSettings();
        messageArea->updateMessagesFont();
        QScrollBar *vert = messageArea->verticalScrollBar();
        vert->setValue((vert->maximum() - vert->minimum()) / 2);
    });

    layout->addWidget(scalingTitle);
    layout->addSpacing(Settings::scale(20));
    layout->addWidget(fontScaling);
    layout->addSpacing(Settings::scale(32));


    Label *spacingTitle = new Label("SPACE BETWEEN MESSAGE GROUPS", container);
    spacingTitle->setTextColor(Settings::HeaderSecondary);
    spacingTitle->setFont(font);
    spacingTitle->setFixedSize(QFontMetrics(font).horizontalAdvance("SPACE BETWEEN MESSAGE GROUPS"), 16);
    
    Slider *spacing = new Slider(QVector<int>{0, 4, 8, 16, 24}, "px", 16, Settings::messageGroupSpace, container);
    layout->addWidget(spacing);
    QObject::connect(spacing, &Slider::valueChanged, [messageArea](int value){
        Settings::lastMessageGroupSpace = Settings::messageGroupSpace;
        Settings::messageGroupSpace = value;
        messageArea->updateMessagesSpacing();
        QScrollBar *vert = messageArea->verticalScrollBar();
        vert->setValue((vert->maximum() - vert->minimum()) / 2);
    });

    layout->addWidget(spacingTitle);
    layout->addSpacing(Settings::scale(20));
    layout->addWidget(spacing);
    layout->addSpacing(Settings::scale(32));


    Label *zoomTitle = new Label("ZOOM LEVEL", container);
    zoomTitle->setTextColor(Settings::HeaderSecondary);
    zoomTitle->setFont(font);
    zoomTitle->setFixedSize(QFontMetrics(font).horizontalAdvance("SPACE BETWEEN MESSAGE GROUPS"), 16);
    
    Slider *zoom = new Slider(QVector<int>{50, 67, 75, 80, 90, 100, 110, 125, 150, 175, 200}, "", 100, 100 * Settings::scaleFactor, container);
    layout->addWidget(zoom);
    QObject::connect(zoom, &Slider::valueChanged, [this, container](int value){
        QWidget *parentWidget = this;
        while (parentWidget->parent()) parentWidget = (Widget *)parentWidget->parent();

        PopUp *popUp = new PopUp(new Widget(), Settings::scale(400), Settings::scale(200), QString(), "Restart Fast-Discord", true, false, "You must restart Fast-Discord for the scale change to be effective.", QString(), QString(), false, false, parentWidget->size(), parentWidget);
        QObject::connect(popUp, &PopUp::cancelled, [popUp](){popUp->deleteLater();});
        
        Settings::newScaleFactor = value / 100.0f;
        Settings::saveSettings();
    });
    
    Widget *separator3 = new Widget(container);
    separator3->setBackgroundColor(Settings::BackgroundModifierAccent);
    separator3->setFixedHeight(1);

    layout->addWidget(zoomTitle);
    layout->addSpacing(Settings::scale(20));
    layout->addWidget(zoom);
    layout->addSpacing(Settings::scale(32));
    layout->addWidget(separator3);

    layout->addStretch(1);

    this->setWidgetResizable(true);
    this->setWidget(container);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setStyleSheet("* {border: none; background-color: " + Settings::colors[Settings::BackgroundPrimary].name() + "}"
                        "QScrollBar::handle:vertical {border: none; border-radius: " + QString::number(Settings::scale(2)) + "px; background-color: " + Settings::colors[Settings::BackgroundTertiary].name() + ";}"
                        "QScrollBar:vertical {border: none; background-color: " + Settings::colors[Settings::BackgroundSecondary].name() + "; border-radius: " + QString::number(Settings::scale(8)) + "px; width: " + QString::number(Settings::scale(3)) + "px;}"
                        "QScrollBar::add-line, QScrollBar::sub-line {border:none; background: none; height: 0;}"
                        "QScrollBar:left-arrow:vertical, QScrollBar::right-arrow:vertical {background: none;}"
                        "QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {background: none;}");
}

}