#include "settings/settings.h"

#include <QPalette>
#include <QLabel>

float Settings::ttsRate;
int Settings::fontScaling;
int Settings::messageGroupSpace;
int Settings::lastMessageGroupSpace;
Settings::AnimateStickers Settings::animateStickers;
Settings::RoleColors Settings::roleColors;
bool Settings::compactModeEnabled;
bool Settings::reducedMotion;
bool Settings::playGifs;
bool Settings::animatedEmoji;
bool Settings::showSendMessageButton;
bool Settings::playbackAndTtsCommand;
bool Settings::customColorSaturation;

QMap<Settings::ColorEnum, QColor> Settings::darkColors;
QMap<Settings::ColorEnum, QColor> Settings::lightColors;
QMap<Settings::ColorEnum, QColor> Settings::colors;
Settings::Theme Settings::theme;
Settings::Theme Settings::actualTheme;
float Settings::saturation;
float Settings::scaleFactor;
float Settings::newScaleFactor;

QString Settings::token;

void Settings::initSettings(Api::RessourceManager *rm, QString tokenp)
{
    token = tokenp;
    
    QSettings settings("Fast-Discord", "config");
    settings.beginGroup(token);
    ttsRate               = settings.value("ttsRate", 1.0f).toFloat();
    animateStickers       = (Settings::AnimateStickers)settings.value("animateStickers", (int)AnimateStickers::Always).toInt();
    roleColors            = (Settings::RoleColors)settings.value("roleColors", (int)RoleColors::InName).toInt();
    fontScaling           = settings.value("fontScaling", 16).toInt();
    messageGroupSpace     = settings.value("messageGroupSpace", 16).toInt();
    compactModeEnabled    = settings.value("compactModeEnabled", false).toBool();
    reducedMotion         = settings.value("reducedMotion", false).toBool();
    playGifs              = settings.value("playGifs", true).toBool();
    animatedEmoji         = settings.value("animatedEmoji", true).toBool();
    showSendMessageButton = settings.value("showSendMessageButton", false).toBool();
    playbackAndTtsCommand = settings.value("playbackAndTtsCommand", false).toBool();
    theme                 = (Settings::Theme)settings.value("theme", (int)Theme::Sync).toInt();
    saturation            = settings.value("saturation", 1.0f).toFloat();
    scaleFactor           = settings.value("scaleFactor", 1.0f).toFloat();
    customColorSaturation = settings.value("customColorSaturation", false).toBool();
    settings.endGroup();
    
    newScaleFactor = scaleFactor;
    
    darkColors = {
        {None,                                QColor(0, 0, 0, 0)},
        {HeaderPrimary,                       QColor(255, 255, 255)},
        {HeaderSecondary,                     QColor(185, 187, 190)},
        {TextNormal,                          QColor(220, 221, 222)},
        {TextMuted,                           QColor(163, 166, 170)},
        {InteractiveNormal,                   QColor(185, 187, 190)},
        {InteractiveHover,                    QColor(220, 221, 222)},
        {InteractiveActive,                   QColor(255, 255, 255)},
        {InteractiveMuted,                    QColor(79, 84, 92)},
        {BackgroundPrimary,                   QColor(54, 57, 63)},
        {BackgroundSecondary,                 QColor(47, 49, 54)},
        {BackgroundSecondaryAlt,              QColor(41, 43, 47)},
        {BackgroundTertiary,                  QColor(32, 34, 37)},
        {BackgroundFloating,                  QColor(24, 25, 28)},
        {BackgroundModifierHover,             QColor(79, 84, 92, 101)},
        {BackgroundModifierActive,            QColor(79, 84, 92, 122)},
        {BackgroundModifierSelected,          QColor(79, 84, 92, 153)},
        {BackgroundModifierAccent,            QColor(79, 84, 92, 122)},
        {BackgroundMessageHover,              QColor(4, 4, 5, 17)},
        {ChannelsDefault,                     QColor(150, 152, 157)},
        {ChannelIcon,                         QColor(142, 146, 151)},
        {ChanneltextareaBackground,           QColor(64, 68, 75)},
        {ChanneltextareaPlaceholder,          QColor(114, 118, 125)},
        {DeprecatedCardBg,                    QColor(32, 34, 37, 153)},
        {ButtonSecondaryBackground,           QColor(79, 84, 92)},
        {ButtonSecondaryBackgroundHover,      QColor(104, 109, 115)},
        {ButtonSecondaryBackgroundActive,     QColor(114, 118, 125)},
        {ButtonDangerBackground,              QColor::fromHsl(359, (int)(0.667 * saturation * 255), (int)(0.541 * 255))},
        {ButtonDangerBackgroundHover,         QColor::fromHsl(359, (int)(0.563 * saturation * 255), (int)(0.404 * 255))},
        {ButtonDangerBackgroundActive,        QColor::fromHsl(359, (int)(0.564 * saturation * 255), (int)(0.351 * 255))},
        {ButtonOutlineDangerBackground,       QColor(0, 0, 0, 0)},
        {ButtonOutlineDangerBackgroundHover,  QColor::fromHsl(359, (int)(0.667 * saturation * 255), (int)(0.541 * 255))},
        {ButtonOutlineDangerBackgroundActive, QColor::fromHsl(359, (int)(0.567 * saturation * 255), (int)(0.48 * 255))},
        {ButtonOutlineDangerText,             QColor(255, 255, 255)},
        {ButtonOutlineDangerTextHover,        QColor(255, 255, 255)},
        {ButtonOutlineDangerTextActive,       QColor(255, 255, 255)},
        {ButtonOutlineDangerBorder,           QColor::fromHsl(359, (int)(0.826 * saturation * 255), (int)(0.594 * 255))},
        {ButtonOutlineDangerBorderHover,      QColor::fromHsl(359, (int)(0.667 * saturation * 255), (int)(0.541 * 255))},
        {ButtonOutlineDangerBorderActive,     QColor::fromHsl(359, (int)(0.667 * saturation * 255), (int)(0.541 * 255))},
        {InfoDangerForeground,                QColor::fromHsl(359, (int)(0.826 * saturation * 255), (int)(0.594 * 255))},
        {StatusBackgroundHover,               QColor(57, 61, 67)},
        {StatusBackgroundActive,              QColor(61, 67, 73)},
        {TextPositive,                        QColor::fromHsl(139, (int)(0.516 * saturation * 255), (int)(0.522 * 255))},
        {Link,                                QColor::fromHsl(197, (int)(saturation * 255), (int)(0.478 * 255))},
        {Error,                               QColor::fromHsl(359, (int)(0.82 * saturation * 255), (int)(0.739 * 255))},
        {SettingsButtonTextColor,             QColor(255, 255, 255)},
        {SliderBackground,                    QColor::fromHsl(217, (int)(0.076 * saturation * 255), (int)(0.335 * 255))}
    };
    lightColors = {
        {None,                                QColor(0, 0, 0, 0)},
        {HeaderPrimary,                       QColor(6, 6, 7)},
        {HeaderSecondary,                     QColor(79, 86, 96)},
        {TextNormal,                          QColor(46, 51, 56)},
        {TextMuted,                           QColor(94, 103, 114)},
        {InteractiveNormal,                   QColor(79, 86, 96)},
        {InteractiveHover,                    QColor(46, 51, 56)},
        {InteractiveActive,                   QColor(6, 6, 7)},
        {InteractiveMuted,                    QColor(199, 204, 209)},
        {BackgroundPrimary,                   QColor(255, 255, 255)},
        {BackgroundSecondary,                 QColor(242, 243, 245)},
        {BackgroundSecondaryAlt,              QColor(235, 237, 239)},
        {BackgroundTertiary,                  QColor(227, 229, 232)},
        {BackgroundFloating,                  QColor(255, 255, 255)},
        {BackgroundModifierHover,             QColor(106, 116, 128, 40)},
        {BackgroundModifierActive,            QColor(106, 116, 128, 50)},
        {BackgroundModifierSelected,          QColor(106, 116, 128, 60)},
        {BackgroundModifierAccent,            QColor(106, 116, 128, 60)},
        {BackgroundMessageHover,              QColor(6, 6, 7, 4)},
        {ChannelsDefault,                     QColor(94, 103, 114)},
        {ChannelIcon,                         QColor(106, 116, 128)},
        {ChanneltextareaBackground,           QColor(235, 237, 239)},
        {ChanneltextareaPlaceholder,          QColor(116, 127, 141)},
        {DeprecatedCardBg,                    QColor(248, 249, 249)},
        {ButtonSecondaryBackground,           QColor(106, 116, 128)},
        {ButtonSecondaryBackgroundHover,      QColor(79, 86, 96)},
        {ButtonSecondaryBackgroundActive,     QColor(68, 75, 83)},
        {ButtonDangerBackground,              QColor::fromHsl(359, (int)(0.667 * saturation * 255), (int)(0.541 * 255))},
        {ButtonDangerBackgroundHover,         QColor::fromHsl(359, (int)(0.563 * saturation * 255), (int)(0.404 * 255))},
        {ButtonDangerBackgroundActive,        QColor::fromHsl(359, (int)(0.564 * saturation * 255), (int)(0.351 * 255))},
        {ButtonOutlineDangerBackground,       QColor(0, 0, 0, 0)},
        {ButtonOutlineDangerBackgroundHover,  QColor::fromHsl(359, (int)(0.667 * saturation * 255), (int)(0.541 * 255))},
        {ButtonOutlineDangerBackgroundActive, QColor::fromHsl(359, (int)(0.567 * saturation * 255), (int)(0.48 * 255))},
        {ButtonOutlineDangerText,             QColor(0, 0, 0)},
        {ButtonOutlineDangerTextHover,        QColor(255, 255, 255)},
        {ButtonOutlineDangerTextActive,       QColor(255, 255, 255)},
        {ButtonOutlineDangerBorder,           QColor::fromHsl(359, (int)(0.826 * saturation * 255), (int)(0.594 * 255))},
        {ButtonOutlineDangerBorderHover,      QColor::fromHsl(359, (int)(0.667 * saturation * 255), (int)(0.541 * 255))},
        {ButtonOutlineDangerBorderActive,     QColor::fromHsl(359, (int)(0.667 * saturation * 255), (int)(0.541 * 255))},
        {InfoDangerForeground,                QColor::fromHsl(359, (int)(0.667 * saturation * 255), (int)(0.541 * 255))},
        {StatusBackgroundHover,               QColor(220, 223, 226)},
        {StatusBackgroundActive,              QColor(214, 217, 222)},
        {TextPositive,                        QColor::fromHsl(139, (int)(0.471 * saturation * 255), (int)(0.333 * 255))},
        {Link,                                QColor::fromHsl(212, (int)(saturation * 255), (int)(0.439 * 255))},
        {Error,                               QColor::fromHsl(359, (int)(0.563 * saturation * 255), (int)(0.404 * 255))},
        {SettingsButtonTextColor,             QColor(116, 127, 141)},
        {SliderBackground,                    QColor::fromHsl(210, (int)(0.029 * saturation * 255), (int)(0.867 * 255))}
    };
    colors = darkColors;
    colors[White]              = QColor(255, 255, 255);
    colors[Black40]            = QColor(0, 0, 0, 101);
    colors[Black85]            = QColor(0, 0, 0, 217);
    colors[Black85]            = QColor(0, 0, 0);
    colors[StatusOffline]      = QColor(90, 90, 90);
    colors[StatusOnline]       = QColor::fromHsl(139, (int)(saturation * 255), (int)(0.439 * 255));
    colors[StatusIdle]         = QColor::fromHsl(38, (int)(saturation * 255), (int)(0.541 * 255));
    colors[StatusDND]          = QColor::fromHsl(359, (int)(saturation * 255), (int)(0.594 * 255));
    colors[BrandExperiment]    = QColor::fromHsl(235, (int)(0.856 * saturation * 255), (int)(0.647 * 255));
    colors[BrandExperiment560] = QColor::fromHsl(235, (int)(0.514 * saturation * 255), (int)(0.524 * 255));
    colors[BrandExperiment600] = QColor::fromHsl(235, (int)(0.467 * saturation * 255), (int)(0.441 * 255));
    colors[UnsavedBackground]  = QColor(32, 34, 37, 229);
    colors[SwitchActive]       = QColor::fromHsl(139, (int)(0.473 * saturation * 255), (int)(0.439 * 255));
    colors[SwitchInactive]     = QColor::fromHsl(218, (int)(0.046 * saturation * 255), (int)(0.469 * 255));
    colors[RadioBarGreen]      = QColor::fromHsl(139, (int)(0.473 * saturation * 255), (int)(0.439 * 255));
    colors[RadioBarOrange]     = QColor::fromHsl(37, (int)(0.812 * saturation * 255), (int)(0.439 * 255));
    colors[RadioBarRed]        = QColor::fromHsl(359, (int)(0.826 * saturation * 255), (int)(0.594 * 255));
    colors[FileSizeColor]      = QColor(114, 118, 125);
    colors[SliderBorder]       = QColor::fromHsl(210, (int)(0.029 * saturation * 255), (int)(0.867 * 255));
    theme = Theme::Dark;
    actualTheme = Theme::Dark;

    rm->getClientSettings([](void *settingsPtr){


        Api::ClientSettings *settings = reinterpret_cast<Api::ClientSettings *>(settingsPtr);        


    });
}

void Settings::saveSettings()
{
    QSettings settings("Fast-Discord", "config");
    settings.beginGroup(token);
    settings.setValue("ttsRate"              , ttsRate);
    settings.setValue("animateStickers"      , (int)animateStickers);
    settings.setValue("roleColors"           , (int)roleColors);
    settings.setValue("fontScaling"          , fontScaling);
    settings.setValue("messageGroupSpace"    , messageGroupSpace);
    settings.setValue("compactModeEnabled"   , compactModeEnabled);
    settings.setValue("reducedMotion"        , reducedMotion);
    settings.setValue("playGifs"             , playGifs);
    settings.setValue("animatedEmoji"        , animatedEmoji);
    settings.setValue("showSendMessageButton", showSendMessageButton);
    settings.setValue("playbackAndTtsCommand", playbackAndTtsCommand);
    settings.setValue("theme"                , (int)theme);
    settings.setValue("saturation"           , saturation);
    settings.setValue("scaleFactor"          , newScaleFactor);
    settings.setValue("customColorSaturation", customColorSaturation);
    settings.endGroup();
}

void Settings::changeToken(QString oldToken, QString newToken)
{
    QSettings settings("Fast-Discord", "config");

    settings.beginGroup("Accounts");
    QStringList keys = settings.allKeys();
    for (int i = 0 ; i < keys.size() ; i++) {
        if (settings.value(keys[i]) == oldToken) {
            settings.setValue(keys[i], newToken);
            break;
        }
    }
    settings.endGroup();
    
    settings.beginGroup(newToken);
    QSettings oldSettings("Fast-Discord", "config");
    oldSettings.beginGroup(oldToken);
    keys = oldSettings.allKeys();
    for (int i = 0 ; i < keys.size() ; i++) {
        settings.setValue(keys[i], oldSettings.value(keys[i]));
    }
    oldSettings.endGroup();
    settings.endGroup();
    settings.remove(oldToken);
}

void Settings::setTheme(Settings::Theme themep)
{
    theme = themep;

    if (theme == Theme::Sync) {
        QLabel label("Am I in the dark?");
        int textColorValue = label.palette().color(QPalette::WindowText).value();
        int bgColorValue = label.palette().color(QPalette::Window).value();
        
        if (textColorValue < bgColorValue) 
            actualTheme = Theme::Dark;
        else
            actualTheme = Theme::Light;
    } else {
        actualTheme = theme;
    }

    if (actualTheme == Theme::Dark)
        colors = darkColors;
    else
        colors = lightColors;

    colors[White]              = QColor(255, 255, 255);
    colors[Black40]            = QColor(0, 0, 0, 101);
    colors[Black85]            = QColor(0, 0, 0, 217);
    colors[Black85]            = QColor(0, 0, 0);
    colors[StatusOffline]      = QColor(90, 90, 90);
    colors[StatusOnline]       = QColor::fromHsl(139, (int)(saturation * 255), (int)(0.439 * 255));
    colors[StatusIdle]         = QColor::fromHsl(38, (int)(saturation * 255), (int)(0.541 * 255));
    colors[StatusDND]          = QColor::fromHsl(359, (int)(saturation * 255), (int)(0.594 * 255));
    colors[BrandExperiment]    = QColor::fromHsl(235, (int)(0.856 * saturation * 255), (int)(0.647 * 255));
    colors[BrandExperiment560] = QColor::fromHsl(235, (int)(0.514 * saturation * 255), (int)(0.524 * 255));
    colors[BrandExperiment600] = QColor::fromHsl(235, (int)(0.467 * saturation * 255), (int)(0.441 * 255));
    colors[UnsavedBackground]  = QColor(32, 34, 37, 229);
    colors[SwitchActive]       = QColor::fromHsl(139, (int)(0.473 * saturation * 255), (int)(0.439 * 255));
    colors[SwitchInactive]     = QColor::fromHsl(218, (int)(0.046 * saturation * 255), (int)(0.469 * 255));
    colors[RadioBarGreen]      = QColor::fromHsl(139, (int)(0.473 * saturation * 255), (int)(0.439 * 255));
    colors[RadioBarOrange]     = QColor::fromHsl(37, (int)(0.812 * saturation * 255), (int)(0.439 * 255));
    colors[RadioBarRed]        = QColor::fromHsl(359, (int)(0.826 * saturation * 255), (int)(0.594 * 255));
    colors[FileSizeColor]      = QColor(114, 118, 125);
    colors[SliderBorder]       = QColor::fromHsl(210, (int)(0.029 * saturation * 255), (int)(0.867 * 255));
}

void Settings::setSaturation(int saturationp)
{
    saturation = saturationp / 100.0f;

    darkColors = {
        {None,                                QColor(0, 0, 0, 0)},
        {HeaderPrimary,                       QColor(255, 255, 255)},
        {HeaderSecondary,                     QColor(185, 187, 190)},
        {TextNormal,                          QColor(220, 221, 222)},
        {TextMuted,                           QColor(163, 166, 170)},
        {InteractiveNormal,                   QColor(185, 187, 190)},
        {InteractiveHover,                    QColor(220, 221, 222)},
        {InteractiveActive,                   QColor(255, 255, 255)},
        {InteractiveMuted,                    QColor(79, 84, 92)},
        {BackgroundPrimary,                   QColor(54, 57, 63)},
        {BackgroundSecondary,                 QColor(47, 49, 54)},
        {BackgroundSecondaryAlt,              QColor(41, 43, 47)},
        {BackgroundTertiary,                  QColor(32, 34, 37)},
        {BackgroundFloating,                  QColor(24, 25, 28)},
        {BackgroundModifierHover,             QColor(79, 84, 92, 101)},
        {BackgroundModifierActive,            QColor(79, 84, 92, 122)},
        {BackgroundModifierSelected,          QColor(79, 84, 92, 153)},
        {BackgroundModifierAccent,            QColor(79, 84, 92, 122)},
        {BackgroundMessageHover,              QColor(4, 4, 5, 17)},
        {ChannelsDefault,                     QColor(150, 152, 157)},
        {ChannelIcon,                         QColor(142, 146, 151)},
        {ChanneltextareaBackground,           QColor(64, 68, 75)},
        {ChanneltextareaPlaceholder,          QColor(114, 118, 125)},
        {DeprecatedCardBg,                    QColor(32, 34, 37, 153)},
        {ButtonSecondaryBackground,           QColor(79, 84, 92)},
        {ButtonSecondaryBackgroundHover,      QColor(104, 109, 115)},
        {ButtonSecondaryBackgroundActive,     QColor(114, 118, 125)},
        {ButtonDangerBackground,              QColor::fromHsl(359, (int)(0.667 * saturation * 255), (int)(0.541 * 255))},
        {ButtonDangerBackgroundHover,         QColor::fromHsl(359, (int)(0.563 * saturation * 255), (int)(0.404 * 255))},
        {ButtonDangerBackgroundActive,        QColor::fromHsl(359, (int)(0.564 * saturation * 255), (int)(0.351 * 255))},
        {ButtonOutlineDangerBackground,       QColor(0, 0, 0, 0)},
        {ButtonOutlineDangerBackgroundHover,  QColor::fromHsl(359, (int)(0.667 * saturation * 255), (int)(0.541 * 255))},
        {ButtonOutlineDangerBackgroundActive, QColor::fromHsl(359, (int)(0.567 * saturation * 255), (int)(0.48 * 255))},
        {ButtonOutlineDangerText,             QColor(255, 255, 255)},
        {ButtonOutlineDangerTextHover,        QColor(255, 255, 255)},
        {ButtonOutlineDangerTextActive,       QColor(255, 255, 255)},
        {ButtonOutlineDangerBorder,           QColor::fromHsl(359, (int)(0.826 * saturation * 255), (int)(0.594 * 255))},
        {ButtonOutlineDangerBorderHover,      QColor::fromHsl(359, (int)(0.667 * saturation * 255), (int)(0.541 * 255))},
        {ButtonOutlineDangerBorderActive,     QColor::fromHsl(359, (int)(0.667 * saturation * 255), (int)(0.541 * 255))},
        {InfoDangerForeground,                QColor::fromHsl(359, (int)(0.826 * saturation * 255), (int)(0.594 * 255))},
        {StatusBackgroundHover,               QColor(57, 61, 67)},
        {StatusBackgroundActive,              QColor(61, 67, 73)},
        {TextPositive,                        QColor::fromHsl(139, (int)(0.516 * saturation * 255), (int)(0.522 * 255))},
        {Link,                                QColor::fromHsl(197, (int)(saturation * 255), (int)(0.478 * 255))},
        {Error,                               QColor::fromHsl(359, (int)(0.82 * saturation * 255), (int)(0.739 * 255))},
        {SettingsButtonTextColor,             QColor(255, 255, 255)},
        {SliderBackground,                    QColor::fromHsl(217, (int)(0.076 * saturation * 255), (int)(0.335 * 255))}
    };
    lightColors = {
        {None,                                QColor(0, 0, 0, 0)},
        {HeaderPrimary,                       QColor(6, 6, 7)},
        {HeaderSecondary,                     QColor(79, 86, 96)},
        {TextNormal,                          QColor(46, 51, 56)},
        {TextMuted,                           QColor(94, 103, 114)},
        {InteractiveNormal,                   QColor(79, 86, 96)},
        {InteractiveHover,                    QColor(46, 51, 56)},
        {InteractiveActive,                   QColor(6, 6, 7)},
        {InteractiveMuted,                    QColor(199, 204, 209)},
        {BackgroundPrimary,                   QColor(255, 255, 255)},
        {BackgroundSecondary,                 QColor(242, 243, 245)},
        {BackgroundSecondaryAlt,              QColor(235, 237, 239)},
        {BackgroundTertiary,                  QColor(227, 229, 232)},
        {BackgroundFloating,                  QColor(255, 255, 255)},
        {BackgroundModifierHover,             QColor(106, 116, 128, 40)},
        {BackgroundModifierActive,            QColor(106, 116, 128, 50)},
        {BackgroundModifierSelected,          QColor(106, 116, 128, 60)},
        {BackgroundModifierAccent,            QColor(106, 116, 128, 60)},
        {BackgroundMessageHover,              QColor(6, 6, 7, 4)},
        {ChannelsDefault,                     QColor(94, 103, 114)},
        {ChannelIcon,                         QColor(106, 116, 128)},
        {ChanneltextareaBackground,           QColor(235, 237, 239)},
        {ChanneltextareaPlaceholder,          QColor(116, 127, 141)},
        {DeprecatedCardBg,                    QColor(248, 249, 249)},
        {ButtonSecondaryBackground,           QColor(106, 116, 128)},
        {ButtonSecondaryBackgroundHover,      QColor(79, 86, 96)},
        {ButtonSecondaryBackgroundActive,     QColor(68, 75, 83)},
        {ButtonDangerBackground,              QColor::fromHsl(359, (int)(0.667 * saturation * 255), (int)(0.541 * 255))},
        {ButtonDangerBackgroundHover,         QColor::fromHsl(359, (int)(0.563 * saturation * 255), (int)(0.404 * 255))},
        {ButtonDangerBackgroundActive,        QColor::fromHsl(359, (int)(0.564 * saturation * 255), (int)(0.351 * 255))},
        {ButtonOutlineDangerBackground,       QColor(0, 0, 0, 0)},
        {ButtonOutlineDangerBackgroundHover,  QColor::fromHsl(359, (int)(0.667 * saturation * 255), (int)(0.541 * 255))},
        {ButtonOutlineDangerBackgroundActive, QColor::fromHsl(359, (int)(0.567 * saturation * 255), (int)(0.48 * 255))},
        {ButtonOutlineDangerText,             QColor(0, 0, 0)},
        {ButtonOutlineDangerTextHover,        QColor(255, 255, 255)},
        {ButtonOutlineDangerTextActive,       QColor(255, 255, 255)},
        {ButtonOutlineDangerBorder,           QColor::fromHsl(359, (int)(0.826 * saturation * 255), (int)(0.594 * 255))},
        {ButtonOutlineDangerBorderHover,      QColor::fromHsl(359, (int)(0.667 * saturation * 255), (int)(0.541 * 255))},
        {ButtonOutlineDangerBorderActive,     QColor::fromHsl(359, (int)(0.667 * saturation * 255), (int)(0.541 * 255))},
        {InfoDangerForeground,                QColor::fromHsl(359, (int)(0.667 * saturation * 255), (int)(0.541 * 255))},
        {StatusBackgroundHover,               QColor(220, 223, 226)},
        {StatusBackgroundActive,              QColor(214, 217, 222)},
        {TextPositive,                        QColor::fromHsl(139, (int)(0.471 * saturation * 255), (int)(0.333 * 255))},
        {Link,                                QColor::fromHsl(212, (int)(saturation * 255), (int)(0.439 * 255))},
        {Error,                               QColor::fromHsl(359, (int)(0.563 * saturation * 255), (int)(0.404 * 255))},
        {SettingsButtonTextColor,             QColor(116, 127, 141)},
        {SliderBackground,                    QColor::fromHsl(210, (int)(0.029 * saturation * 255), (int)(0.867 * 255))}
    };

    if (actualTheme == Theme::Dark)
        colors = darkColors;
    else
        colors = lightColors;

    colors[White]              = QColor(255, 255, 255);
    colors[Black40]            = QColor(0, 0, 0, 101);
    colors[Black85]            = QColor(0, 0, 0, 217);
    colors[Black85]            = QColor(0, 0, 0);
    colors[StatusOffline]      = QColor(90, 90, 90);
    colors[StatusOnline]       = QColor::fromHsl(139, (int)(saturation * 255), (int)(0.439 * 255));
    colors[StatusIdle]         = QColor::fromHsl(38, (int)(saturation * 255), (int)(0.541 * 255));
    colors[StatusDND]          = QColor::fromHsl(359, (int)(saturation * 255), (int)(0.594 * 255));
    colors[BrandExperiment]    = QColor::fromHsl(235, (int)(0.856 * saturation * 255), (int)(0.647 * 255));
    colors[BrandExperiment560] = QColor::fromHsl(235, (int)(0.514 * saturation * 255), (int)(0.524 * 255));
    colors[BrandExperiment600] = QColor::fromHsl(235, (int)(0.467 * saturation * 255), (int)(0.441 * 255));
    colors[UnsavedBackground]  = QColor(32, 34, 37, 229);
    colors[SwitchActive]       = QColor::fromHsl(139, (int)(0.473 * saturation * 255), (int)(0.439 * 255));
    colors[SwitchInactive]     = QColor::fromHsl(218, (int)(0.046 * saturation * 255), (int)(0.469 * 255));
    colors[RadioBarGreen]      = QColor::fromHsl(139, (int)(0.473 * saturation * 255), (int)(0.439 * 255));
    colors[RadioBarOrange]     = QColor::fromHsl(37, (int)(0.812 * saturation * 255), (int)(0.439 * 255));
    colors[RadioBarRed]        = QColor::fromHsl(359, (int)(0.826 * saturation * 255), (int)(0.594 * 255));
    colors[FileSizeColor]      = QColor(114, 118, 125);
    colors[SliderBorder]       = QColor::fromHsl(210, (int)(0.029 * saturation * 255), (int)(0.867 * 255));
}