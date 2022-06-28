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

    // Has to be called before using settings
    static void initSettings(Api::RessourceManager *rm);
    
    static void saveSettings();
    
    static void setTheme(Theme theme);
    static void setSaturation(int saturation);
    static void applySaturationToCustom(bool applyToCustom);


    static QMap<ColorEnum, QColor> colors;

    static float ttsRate;
    static int fontScaling;
    static int messageGroupSpace;
    static AnimateStickers animateStickers;
    static bool reducedMotion;
    static bool playGifs;
    static bool animatedEmoji;
    static bool showSendMessageButton;
    static bool playbackAndTtsCommand;

    static VoiceInputMode voiceInputMode;
    static int inputSensitivity;
    static int inputVolume;
    static int outputVolume;
    static bool previewVideo;

private:
    static QMap<ColorEnum, QColor> darkColors;
    static QMap<ColorEnum, QColor> lightColors;
};