#include "ui/settings/menus/accessibility.h"

#include "ui/common/roundedimage.h"
#include "ui/common/statusicon.h"
#include "ui/settings/components/slider.h"
#include "ui/settings/components/settingsbutton.h"
#include "ui/settings/components/radiobutton.h"
#include "ui/settings/components/switchbutton.h"
#include "ui/settings/settingsmenu.h"
#include "ui/rightcolumn/messagewidget.h"

namespace Ui {

Accessibility::Accessibility(Api::RessourceManager *rm, QWidget *parent)
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

    Label *title = new Label("Accessibility", container);
    title->setFixedSize(QFontMetrics(font).horizontalAdvance("Accessibility"), Settings::scale(20));
    title->setFont(font);
    title->setTextColor(Settings::HeaderPrimary);

    layout->addWidget(title);
    layout->addSpacing(Settings::scale(20));


    Widget *examples = new Widget(container);
    examples->setBackgroundColor(Settings::BackgroundSecondary);
    examples->setBorderColor(Settings::BackgroundTertiary);
    examples->setBorderSize(Settings::scale(1));
    examples->setBorderRadius(Settings::scale(5));
    QVBoxLayout *examplesLayout = new QVBoxLayout(examples);
    examplesLayout->setContentsMargins(Settings::scale(16), Settings::scale(16), Settings::scale(16), Settings::scale(16));
    examplesLayout->setSpacing(Settings::scale(16));

    Widget *buttonAndStatus = new Widget(examples);
    buttonAndStatus->setFixedHeight(Settings::scale(32));
    QHBoxLayout *buttonAndStatusLayout = new QHBoxLayout(buttonAndStatus);
    buttonAndStatusLayout->setContentsMargins(0, 0, 0, 0);
    buttonAndStatusLayout->setSpacing(Settings::scale(8));

    buttonAndStatusLayout->addWidget(new SettingsButton(SettingsButton::Type::Normal, "Example Button", buttonAndStatus));
    buttonAndStatusLayout->addWidget(createAvatar("online"));
    buttonAndStatusLayout->addWidget(createAvatar("dnd"));
    buttonAndStatusLayout->addWidget(createAvatar("idle"));
    buttonAndStatusLayout->addStretch(1);

    Widget *messageContainer = new Widget(examples);
    messageContainer->setBackgroundColor(Settings::BackgroundPrimary);
    messageContainer->setBorderRadius(Settings::scale(4));
    QHBoxLayout *messageLayout = new QHBoxLayout(messageContainer);
    messageLayout->setContentsMargins(Settings::scale(8), Settings::scale(8), Settings::scale(8), Settings::scale(8));
    rm->getClient([rm, messageLayout, messageContainer](Api::CallbackStruct cb){
            Api::Client *client = reinterpret_cast<Api::Client *>(cb.data);
            Api::User author{client->username, client->discriminator, client->avatar, client->banner, client->locale, client->email, client->id, client->accentColor, client->flags, client->purchasedFlags, client->publicFlags, 0, 0, 2, 2};
            messageLayout->addWidget(new MessageWidget(rm, new Api::Message{author, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, QVector<Api::Reaction *>(), QVector<Api::Embed *>(), QVector<Api::User *>(), QVector<Api::Attachment *>(), QVector<Api::ChannelMention *>(), QVector<QString>(), QVector<Api::MessageComponent *>(), QVector<Api::StickerItem *>(), QVector<Api::Sticker *>(), 
                "links will look like this https://discord.com/accessibility", QDateTime::currentDateTime().toString(Qt::DateFormat::ISODate), QString(), QString(), 0, 0, 0, 0, 0, 0, author.publicFlags, Api::MessageType::Default, 0, false, false, false}, true, false, messageContainer));
        });

    examplesLayout->addWidget(buttonAndStatus);
    examplesLayout->addWidget(messageContainer);

    layout->addWidget(examples);
    layout->addSpacing(Settings::scale(20));


    font.setPixelSize(Settings::scale(12));
    font.setBold(true);
    Label *saturationTitle = new Label("SATURATION", container);
    saturationTitle->setMinimumSize(Settings::scale(104), Settings::scale(16));
    saturationTitle->setFont(font);
    saturationTitle->setTextColor(Settings::HeaderSecondary);

    font.setBold(false);
    font.setPixelSize(Settings::scale(14));
    Label *saturationDesc = new Label("Reduce the saturation of colors within the application, for those with color sensitivities. This does not affect the\nsaturation of images, videos, role colors or other user-provided content by default.", container);
    saturationDesc->setMinimumHeight(Settings::scale(40));
    saturationDesc->setFont(font);
    saturationDesc->setTextColor(Settings::HeaderSecondary);

    Slider *saturationSlider = new Slider(QVector<int>{0, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100}, "%", 100, Settings::saturation * 100, container);
    QObject::connect(saturationSlider, &Slider::valueChanged, [this](int value){
        Settings::setSaturation(value);
        Settings::saveSettings();

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
    });

    Widget *customColorContainer = new Widget(container);
    customColorContainer->setFixedHeight(Settings::scale(24));
    QHBoxLayout *customColorLayout = new QHBoxLayout(customColorContainer);
    customColorLayout->setSpacing(0);
    customColorLayout->setContentsMargins(0, 0, 0, 0);

    font.setPixelSize(Settings::scale(16));
    Label *customColorTitle = new Label("Apply to custom color choices", container);
    customColorTitle->setTextColor(Settings::HeaderPrimary);
    customColorTitle->setFont(font);
    customColorTitle->setFixedSize(QFontMetrics(font).horizontalAdvance("Apply to custom color choices"), Settings::scale(24));

    SwitchButton *customColorSwitch = new SwitchButton(Settings::customColorSaturation);
    QObject::connect(customColorSwitch, &SwitchButton::clicked, [](bool state){
        Settings::customColorSaturation = state;
        Settings::saveSettings();
    });

    customColorLayout->addWidget(customColorTitle);
    customColorLayout->addStretch(1);
    customColorLayout->addWidget(customColorSwitch);

    font.setPixelSize(Settings::scale(14));
    Label *customColorDesc = new Label("Turn on to apply this adjustment to custom color choices, like role colors", container);
    customColorDesc->setTextColor(Settings::HeaderSecondary);
    customColorDesc->setFont(font);
    customColorDesc->setFixedSize(QFontMetrics(font).horizontalAdvance("Turn on to apply this adjustment to custom color choices, like role colors"), Settings::scale(24));
    
    layout->addWidget(saturationTitle);
    layout->addSpacing(Settings::scale(8));
    layout->addWidget(saturationDesc);
    layout->addSpacing(Settings::scale(20));
    layout->addWidget(saturationSlider);
    layout->addSpacing(Settings::scale(8));
    layout->addWidget(customColorContainer);
    layout->addSpacing(Settings::scale(8));
    layout->addWidget(customColorDesc);
    layout->addSpacing(Settings::scale(20));
    layout->addWidget(createSeparator());
    layout->addSpacing(Settings::scale(20));
    

    font.setPixelSize(Settings::scale(12));
    font.setBold(true);
    Label *roleTitle = new Label("ROLE COLORS", container);
    roleTitle->setMinimumSize(Settings::scale(104), Settings::scale(16));
    roleTitle->setFont(font);
    roleTitle->setTextColor(Settings::HeaderSecondary);

    font.setBold(false);
    font.setPixelSize(Settings::scale(14));
    Label *roleDesc = new Label("Choose a display for role colors.", container);
    roleDesc->setMinimumHeight(Settings::scale(20));
    roleDesc->setFont(font);
    roleDesc->setTextColor(Settings::HeaderSecondary);

    RadioGroup *roleRadio = new RadioGroup(QVector<RadioParameters>{
        RadioParameters{"Show role colors in names", QString(), Settings::None},
        RadioParameters{"Show role colors next to names", QString(), Settings::None},
        RadioParameters{"Don't show role colors", QString(), Settings::None},
    }, (Settings::roleColors == Settings::RoleColors::InName ? 0 : Settings::roleColors == Settings::RoleColors::NextToName ? 1 : 2), container);
    QObject::connect(roleRadio, &RadioGroup::clicked, [](int index){
        if (index == 0)
            Settings::roleColors = Settings::RoleColors::InName;
        else if (index == 1)
            Settings::roleColors = Settings::RoleColors::NextToName;
        else
            Settings::roleColors = Settings::RoleColors::NotShown;
        Settings::saveSettings();
    });
    
    layout->addWidget(roleTitle);
    layout->addSpacing(Settings::scale(8));
    layout->addWidget(roleDesc);
    layout->addSpacing(Settings::scale(8));
    layout->addWidget(roleRadio);
    layout->addSpacing(Settings::scale(20));
    layout->addWidget(createSeparator());
    layout->addSpacing(Settings::scale(20));

    
    font.setPixelSize(Settings::scale(12));
    font.setBold(true);
    Label *motionTitle = new Label("REDUCED MOTION", container);
    motionTitle->setTextColor(Settings::HeaderSecondary);
    motionTitle->setFont(font);
    motionTitle->setFixedSize(QFontMetrics(font).horizontalAdvance("REDUCED MOTION"), 16);
    
    font.setBold(false);
    font.setPixelSize(Settings::scale(14));
    Label *motionDesc = new Label("Reduce the amount and intensity of animations, hover effects, and other moving effects across Discord.\nNeed help? Check our Help Center at https://support.discord.com/hc/en-us/articles/360040613412 for more info!", container);
    motionDesc->setMinimumHeight(Settings::scale(40));
    motionDesc->setFont(font);
    motionDesc->setTextColor(Settings::HeaderSecondary);
    
    Widget *enableContainer = new Widget(container);
    enableContainer->setFixedHeight(Settings::scale(24));
    QHBoxLayout *enableLayout = new QHBoxLayout(enableContainer);
    enableLayout->setSpacing(0);
    enableLayout->setContentsMargins(0, 0, 0, 0);

    font.setPixelSize(Settings::scale(16));
    Label *enableTitle = new Label("Enable Reduced Motion", container);
    enableTitle->setTextColor(Settings::HeaderPrimary);
    enableTitle->setFont(font);
    enableTitle->setFixedSize(QFontMetrics(font).horizontalAdvance("Enable Reduced Motion"), Settings::scale(24));

    SwitchButton *enableSwitch = new SwitchButton(Settings::reducedMotion);

    enableLayout->addWidget(enableTitle);
    enableLayout->addStretch(1);
    enableLayout->addWidget(enableSwitch);
    
    Widget *gifContainer = new Widget(container);
    gifContainer->setFixedHeight(Settings::scale(24));
    QHBoxLayout *gifLayout = new QHBoxLayout(gifContainer);
    gifLayout->setSpacing(0);
    gifLayout->setContentsMargins(0, 0, 0, 0);

    font.setPixelSize(Settings::scale(16));
    Label *gifTitle = new Label("Automatically play GIFs when Discord is focused.", container);
    gifTitle->setTextColor(Settings::HeaderPrimary);
    gifTitle->setFont(font);
    gifTitle->setFixedSize(QFontMetrics(font).horizontalAdvance("Automatically play GIFs when Discord is focused."), Settings::scale(24));

    SwitchButton *gifSwitch = new SwitchButton(Settings::playGifs);
    QObject::connect(gifSwitch, &SwitchButton::clicked, [rm](bool state){
        /*if (state)
            rm->requester->setSettingsProto([](void *){}, "Mj0iCgoIT05fQ0xJQ0tKAggBUgIIAVoCCAFiAggBagIIAXIAegIIAoIBAggBigECCAGaAQIIAqIBAKoBAggB");
        else
            rm->requester->setSettingsProto([](void *){}, "MjsiCgoIT05fQ0xJQ0tKAggBUgIIAVoAYgIIAWoCCAFyAHoCCAKCAQIIAYoBAggBmgECCAKiAQCqAQIIAQ==");*/

        Settings::playGifs = state;
        Settings::saveSettings();
    });

    gifLayout->addWidget(gifTitle);
    gifLayout->addStretch(1);
    gifLayout->addWidget(gifSwitch);
    
    Widget *emojiContainer = new Widget(container);
    emojiContainer->setFixedHeight(Settings::scale(24));
    QHBoxLayout *emojiLayout = new QHBoxLayout(emojiContainer);
    emojiLayout->setSpacing(0);
    emojiLayout->setContentsMargins(0, 0, 0, 0);

    font.setPixelSize(Settings::scale(16));
    Label *emojiTitle = new Label("Play animated emoji", container);
    emojiTitle->setTextColor(Settings::HeaderPrimary);
    emojiTitle->setFont(font);
    emojiTitle->setFixedSize(QFontMetrics(font).horizontalAdvance("Play animated emoji"), Settings::scale(24));

    SwitchButton *emojiSwitch = new SwitchButton(Settings::animatedEmoji);
    QObject::connect(emojiSwitch, &SwitchButton::clicked, [rm](bool state){
        /*if (state)
            rm->requester->setSettingsProto([](void *){}, "Mj0iCgoIT05fQ0xJQ0tKAggBUgIIAVoAYgIIAWoCCAFyAggBegIIAoIBAggBigECCAGaAQIIAqIBAKoBAggB");
        else
            rm->requester->setSettingsProto([](void *){}, "MjsiCgoIT05fQ0xJQ0tKAggBUgIIAVoAYgIIAWoCCAFyAHoCCAKCAQIIAYoBAggBmgECCAKiAQCqAQIIAQ==");*/

        Settings::animatedEmoji = state;
        Settings::saveSettings();
    });

    emojiLayout->addWidget(emojiTitle);
    emojiLayout->addStretch(1);
    emojiLayout->addWidget(emojiSwitch);

    QObject::connect(enableSwitch, &SwitchButton::clicked, [gifSwitch, emojiSwitch](bool state){
        if (state) {
            gifSwitch->setState(false);
            emojiSwitch->setState(false);
        } else {
            gifSwitch->setState(Settings::playGifs);
            emojiSwitch->setState(Settings::animatedEmoji);
        }

        Settings::reducedMotion = state;
        Settings::saveSettings();
    });
    
    layout->addWidget(motionTitle);
    layout->addSpacing(Settings::scale(8));
    layout->addWidget(motionDesc);
    layout->addSpacing(Settings::scale(20));
    layout->addWidget(enableContainer);
    layout->addSpacing(Settings::scale(20));
    layout->addWidget(createSeparator());
    layout->addSpacing(Settings::scale(20));
    layout->addWidget(gifContainer);
    layout->addSpacing(Settings::scale(20));
    layout->addWidget(createSeparator());
    layout->addSpacing(Settings::scale(20));
    layout->addWidget(emojiContainer);
    layout->addSpacing(Settings::scale(20));
    layout->addWidget(createSeparator());
    layout->addSpacing(Settings::scale(20));
    

    font.setPixelSize(Settings::scale(12));
    font.setBold(true);
    Label *stickersTitle = new Label("STICKERS", container);
    stickersTitle->setMinimumSize(Settings::scale(104), Settings::scale(16));
    stickersTitle->setFont(font);
    stickersTitle->setTextColor(Settings::HeaderSecondary);

    font.setBold(false);
    font.setPixelSize(Settings::scale(14));
    Label *stickersDesc = new Label("This controls when stickers animate.", container);
    stickersDesc->setMinimumHeight(Settings::scale(20));
    stickersDesc->setFont(font);
    stickersDesc->setTextColor(Settings::HeaderSecondary);

    RadioGroup *stickersRadio = new RadioGroup(QVector<RadioParameters>{
        RadioParameters{"Always animate", QString(), Settings::None},
        RadioParameters{"Animate on interaction", "On the desktop client, stickers will animate\non hover or focus. On mobile clients, stickers will animate on long-press.", Settings::None},
        RadioParameters{"Never animate", QString(), Settings::None},
    }, (Settings::animateStickers == Settings::AnimateStickers::Always ? 0 : Settings::animateStickers == Settings::AnimateStickers::AnimateOnInteraction ? 1 : 2), container);
    QObject::connect(stickersRadio, &RadioGroup::clicked, [rm](int index){
        if (index == 0) {
            //rm->requester->setSettingsProto([](void *){}, "MjkiCgoIT05fQ0xJQ0tKAggBUgIIAVoAYgIIAWoCCAFyAHoAggECCAGKAQIIAZoBAggCogEAqgECCAE=");
            Settings::animateStickers = Settings::AnimateStickers::Always;
        } else if (index == 1) {
            //rm->requester->setSettingsProto([](void *){}, "MjkiCgoIT05fQ0xJQ0tKAggBUgIIAVoAYgIIAWoCCAFyAHoCCAGCAQCKAQIIAZoBAggCogEAqgECCAE=");
            Settings::animateStickers = Settings::AnimateStickers::AnimateOnInteraction;
        } else {
            //rm->requester->setSettingsProto([](void *){}, "MjkiCgoIT05fQ0xJQ0tKAggBUgIIAVoAYgIIAWoCCAFyAHoCCAKCAQCKAQIIAZoBAggCogEAqgECCAE=");
            Settings::animateStickers = Settings::AnimateStickers::Never;
        }
        Settings::saveSettings();
    });
    
    layout->addWidget(stickersTitle);
    layout->addSpacing(Settings::scale(8));
    layout->addWidget(stickersDesc);
    layout->addSpacing(Settings::scale(8));
    layout->addWidget(stickersRadio);
    layout->addSpacing(Settings::scale(40));

    
    font.setPixelSize(Settings::scale(12));
    font.setBold(true);
    Label *chatTitle = new Label("CHAT INPUT", container);
    chatTitle->setTextColor(Settings::HeaderSecondary);
    chatTitle->setFont(font);
    chatTitle->setFixedSize(QFontMetrics(font).horizontalAdvance("CHAT INPUT"), 16);
    
    Widget *chatContainer = new Widget(container);
    chatContainer->setFixedHeight(Settings::scale(24));
    QHBoxLayout *chatLayout = new QHBoxLayout(chatContainer);
    chatLayout->setSpacing(0);
    chatLayout->setContentsMargins(0, 0, 0, 0);

    font.setBold(false);
    font.setPixelSize(Settings::scale(16));
    Label *chatcTitle = new Label("Show Send Message Button", container);
    chatcTitle->setTextColor(Settings::HeaderPrimary);
    chatcTitle->setFont(font);
    chatcTitle->setFixedSize(QFontMetrics(font).horizontalAdvance("Show Send Message Button"), Settings::scale(24));

    SwitchButton *chatSwitch = new SwitchButton(Settings::showSendMessageButton);
    QObject::connect(chatSwitch, &SwitchButton::clicked, [](bool state){
        Settings::showSendMessageButton = state;
        Settings::saveSettings();
    });

    chatLayout->addWidget(chatcTitle);
    chatLayout->addStretch(1);
    chatLayout->addWidget(chatSwitch);
    
    layout->addWidget(chatTitle);
    layout->addSpacing(Settings::scale(20));
    layout->addWidget(chatContainer);
    layout->addSpacing(Settings::scale(20));
    layout->addWidget(createSeparator());
    layout->addSpacing(Settings::scale(20));

    
    font.setPixelSize(Settings::scale(12));
    font.setBold(true);
    Label *ttsTitle = new Label("TEXT-TO-SPEECH", container);
    ttsTitle->setTextColor(Settings::HeaderSecondary);
    ttsTitle->setFont(font);
    ttsTitle->setFixedSize(QFontMetrics(font).horizontalAdvance("TEXT-TO-SPEECH"), 16);
    
    Widget *ttsContainer = new Widget(container);
    ttsContainer->setFixedHeight(Settings::scale(24));
    QHBoxLayout *ttsLayout = new QHBoxLayout(ttsContainer);
    ttsLayout->setSpacing(0);
    ttsLayout->setContentsMargins(0, 0, 0, 0);

    font.setBold(false);
    font.setPixelSize(Settings::scale(16));
    Label *ttscTitle = new Label("Allow playback and usage of /tts command", container);
    ttscTitle->setTextColor(Settings::HeaderPrimary);
    ttscTitle->setFont(font);
    ttscTitle->setFixedSize(QFontMetrics(font).horizontalAdvance("Allow playback and usage of /tts command"), Settings::scale(24));

    SwitchButton *ttsSwitch = new SwitchButton(Settings::playbackAndTtsCommand);
    QObject::connect(ttsSwitch, &SwitchButton::clicked, [rm](bool state){
        /*if (state)
            rm->requester->setSettingsProto([](void *){}, "MjsiCgoIT05fQ0xJQ0tKAggBUgIIAVoAYgIIAWoCCAFyAHoCCAKCAQIIAYoBAggBmgECCAKiAQCqAQIIAQ==");
        else
            rm->requester->setSettingsProto([](void *){}, "MjkiCgoIT05fQ0xJQ0tKAggBUgIIAVoAYgIIAWoCCAFyAHoCCAKCAQCKAQIIAZoBAggCogEAqgECCAE=");*/

        Settings::playbackAndTtsCommand = state;
        Settings::saveSettings();
    });

    ttsLayout->addWidget(ttscTitle);
    ttsLayout->addStretch(1);
    ttsLayout->addWidget(ttsSwitch);
    
    layout->addWidget(ttsTitle);
    layout->addSpacing(Settings::scale(20));
    layout->addWidget(ttsContainer);
    layout->addSpacing(Settings::scale(20));
    layout->addWidget(createSeparator());
    layout->addSpacing(Settings::scale(20));


    /*TODO
    
    font.setPixelSize(Settings::scale(12));
    font.setBold(true);
    Label *ttsrTitle = new Label("TEXT-TO-SPEECH RATE", container);
    ttsrTitle->setTextColor(Settings::HeaderSecondary);
    ttsrTitle->setFont(font);
    ttsrTitle->setFixedSize(QFontMetrics(font).horizontalAdvance("TEXT-TO-SPEECH RATE"), 16);

    SettingsButton *preview = new SettingsButton(SettingsButton::Type::Normal, "Preview", container);

    Slider *ttsrSlider = new Slider(QVector<int>{0, 1, 2, 3, 4, 5}, "", 1, 1, container);
    
    layout->addWidget(ttsrTitle);
    layout->addSpacing(Settings::scale(20));
    layout->addWidget(preview);
    layout->addSpacing(Settings::scale(24));
    layout->addWidget(ttsrSlider);
    layout->addSpacing(Settings::scale(20));
    layout->addWidget(createSeparator());
    layout->addSpacing(Settings::scale(20));*/
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

Widget *Accessibility::createAvatar(QString status)
{
    Widget *iconContainer = new Widget(this);
    iconContainer->setFixedSize(32, 32);
    RoundedImage *icon = new RoundedImage(":user-icon-asset0.png", Settings::scale(32), Settings::scale(32), Settings::scale(16), iconContainer);

    Widget *statusBackground = new Widget(iconContainer);
    statusBackground->setFixedSize(Settings::scale(16), Settings::scale(16));
    statusBackground->move(Settings::scale(18), Settings::scale(18));
    statusBackground->setBorderRadius(Settings::scale(8));
    statusBackground->setBackgroundColor(Settings::BackgroundSecondary);

    StatusIcon *statusIcon = new StatusIcon(iconContainer);
    statusIcon->setStatus(status);
    statusIcon->move(Settings::scale(21), Settings::scale(21));

    return iconContainer;
}

Widget *Accessibility::createSeparator()
{
    Widget *separator = new Widget(this);
    separator->setBackgroundColor(Settings::BackgroundModifierAccent);
    separator->setFixedHeight(1);
    return separator;
}

}