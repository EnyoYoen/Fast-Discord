#pragma once

#include "api/ressourcemanager.h"

#include <QMap>
#include <QString>
#include <QColor>
#include <QtGlobal>

class Settings {

public:
    enum ColorEnum {
        None,
        White,
        HeaderPrimary,
        HeaderSecondary,
        TextNormal,
        TextMuted,
        InteractiveNormal,
        InteractiveHover,
        InteractiveActive,
        InteractiveMuted,
        BackgroundPrimary,
        BackgroundSecondary,
        BackgroundSecondaryAlt,
        BackgroundTertiary,
        BackgroundFloating,
        BackgroundModifierSelected,
        BackgroundModifierActive,
        BackgroundModifierHover,
        BackgroundModifierAccent,
        BackgroundMessageHover,
        ChannelsDefault,
        ChannelIcon,
        ChanneltextareaBackground,
        ChanneltextareaPlaceholder,
        DeprecatedCardBg,
        ButtonSecondaryBackground,
        ButtonSecondaryBackgroundHover,
        ButtonSecondaryBackgroundActive,
        ButtonDangerBackground,
        ButtonDangerBackgroundHover,
        ButtonDangerBackgroundActive,
        ButtonOutlineDangerBackground,
        ButtonOutlineDangerBackgroundHover,
        ButtonOutlineDangerBackgroundActive,
        ButtonOutlineDangerText,
        ButtonOutlineDangerTextHover,
        ButtonOutlineDangerTextActive,
        ButtonOutlineDangerBorder,
        ButtonOutlineDangerBorderHover,
        ButtonOutlineDangerBorderActive,
        InfoDangerForeground,
        BrandExperiment,
        BrandExperiment560,
        BrandExperiment600,
        StatusOffline,
        StatusOnline,
        StatusIdle,
        StatusDND,
        StatusBackgroundHover,
        StatusBackgroundActive,
        TextPositive,
        UnsavedBackground,
        Black40,
        Black85,
        Black,
        Link,
        Error,
        SwitchActive,
        SwitchInactive,
        RadioBarGreen,
        RadioBarOrange,
        RadioBarRed,
        SettingsButtonTextColor,
        FileSizeColor,
        SliderBackground,
        SliderBorder,
    };
    enum class MessageDisplay : quint8 {
        Cozy,
        Compact
    };
    enum class Theme : quint8 {
        Dark,
        Light,
        Sync
    };
    enum class AnimateStickers : quint8 {
        Always,
        AnimateOnInteraction,
        Never
    };
    enum class VoiceInputMode : quint8 {
        VoiceActivity,
        PushToTalk
    };
    enum class RoleColors : quint8 {
        InName,
        NextToName,
        NotShown
    };

    // Has to be called before using settings
    static void initSettings(Api::RessourceManager *rm, QString token);
    static void saveSettings();
    static void changeToken(QString oldToken, QString newToken);
    
    static void setTheme(Theme theme);
    static void setSaturation(int saturation);


    static QMap<ColorEnum, QColor> colors;
    static Theme theme;
    static Theme actualTheme; // Only Dark or Light
    static float saturation;
    static float scaleFactor;
    static float newScaleFactor;

    static float ttsRate;
    static int fontScaling;
    static int messageGroupSpace;
    static int lastMessageGroupSpace;
    static AnimateStickers animateStickers;
    static RoleColors roleColors;
    static bool reducedMotion;
    static bool playGifs;
    static bool animatedEmoji;
    static bool showSendMessageButton;
    static bool playbackAndTtsCommand;
    static bool compactModeEnabled;
    static bool customColorSaturation;

    static VoiceInputMode voiceInputMode;
    static int inputSensitivity;
    static int inputVolume;
    static int outputVolume;
    static bool previewVideo;

    
    static inline int scale(int value)
    {
        if (value == 1 && scaleFactor < 1.0f) return 1;
        return value * scaleFactor;
    }

private:
    static QMap<ColorEnum, QColor> darkColors;
    static QMap<ColorEnum, QColor> lightColors;
    static QString token;
};