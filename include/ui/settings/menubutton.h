#pragma once

#include <QLabel>
#include <QMouseEvent>

namespace Ui {

class MenuButton : public QLabel
{
    Q_OBJECT
public:
    enum class ButtonType : quint8 {
        MyAccount,
        UserProfile,
        PrivacySafety,
        AuthorizedApps,
        Connections,
        Nitro,
        ServerBoost,
        Subscriptions,
        GiftInventory,
        Billing,
        Appearance,
        Accessibility,
        VoiceVideo,
        TextImages,
        Notifications,
        Keybinds,
        Language,
        StreamerMode,
        Advanced,
        ActivityStatus,
        WhatsNew,
        HypeSquad,
        LogOut
    };

    MenuButton(MenuButton::ButtonType type, QWidget *parent);
    void unclicked();

    ButtonType type;

signals:
    void clicked(ButtonType type);

private:
    void mouseReleaseEvent(QMouseEvent *) override;
    void mousePressEvent(QMouseEvent *) override;
    void enterEvent(QEvent *) override;
    void leaveEvent(QEvent *) override;

    bool pressed;
};

} // namespace Ui