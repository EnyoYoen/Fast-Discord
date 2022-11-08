#include "ui/settings/menus/myaccount.h"

#include "ui/common/basicwidgets.h"
#include "ui/common/roundedimage.h"
#include "ui/settings/components/settingsbutton.h"
#include "ui/settings/components/settingsinput.h"
#include "ui/settings/components/popup.h"
#include "ui/settings/components/divider.h"
#include "api/objects/client.h"
#include "api/objects/error.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QScrollBar>

namespace Ui {

class Reveal : public Label
{
public:
    Reveal(Label *labelp, QString revealStrp, QString hiddenStrp)
        : Label(nullptr)
    {
        reveal = false;
        label = labelp;
        revealStr = revealStrp;
        hiddenStr = hiddenStrp;
        label->setText(hiddenStrp);
        
        font.setPixelSize(Settings::scale(14));
        font.setFamily("whitney");
        this->setText("Reveal");
        this->setFont(font);
        this->setTextColor(Settings::Link);
        this->setBackgroundColor(Settings::BackgroundSecondaryAlt);
        this->setCursor(Qt::CursorShape::PointingHandCursor);
        this->setFixedSize(QFontMetrics(font).horizontalAdvance("Reveal"), Settings::scale(16));
    }

private:
    void mouseReleaseEvent(QMouseEvent *) override
    {
        if (reveal) {
            this->setText("Reveal");
            label->setText(hiddenStr);
            reveal = false;
        } else {
            this->setText("Hide");
            label->setText(revealStr);
            reveal = true;
        }
    }
    void enterEvent(QEvent *) override
    {
        font.setUnderline(true);
        this->setFont(font);
    }
    void leaveEvent(QEvent *) override
    {
        font.setUnderline(false);
        this->setFont(font);
    }

    QFont font;
    QString hiddenStr;
    QString revealStr;
    Label *label;
    bool reveal;
};

MyAccount::MyAccount(Api::RessourceManager *rmp, QWidget *parent)
    : QScrollArea()
{
    rm = rmp;

    Widget *container = new Widget(this);
    QVBoxLayout *layout = new QVBoxLayout(container);
    container->setMaximumWidth(Settings::scale(752));
    container->setContentsMargins(Settings::scale(40), Settings::scale(60), Settings::scale(40), Settings::scale(80));
    container->setBackgroundColor(Settings::BackgroundPrimary);

    QFont font;
    font.setPixelSize(Settings::scale(20));
    font.setFamily("whitney");

    Label *title = new Label("My Account", container);
    title->setFixedHeight(Settings::scale(24));
    title->setFont(font);
    title->setTextColor(Settings::HeaderPrimary);

    Widget *profile = new Widget(container);
    profile->setFixedHeight(Settings::scale(408));
    profile->setBorderRadius(Settings::scale(8));
    profile->setBackgroundColor(Settings::BackgroundTertiary);
    rm->getClient([this, profile](Api::CallbackStruct cb){
        Api::Client client = *reinterpret_cast<Api::Client *>(cb.data);
    
        Widget *banner = new Widget(profile);
        banner->setBorderRadius(Settings::scale(8), Settings::scale(8), 0, 0);
        banner->setFixedSize(Settings::scale(660), Settings::scale(100));
        
        if (!client.banner.isNull()) {
            rm->getImage([this, banner](Api::CallbackStruct cb){
                banner->setPixmap(QPixmap(*reinterpret_cast<QString *>(cb.data)).scaledToWidth(660));
            }, "https://cdn.discordapp.com/banners/" + client.id + "/" + client.banner + ".png?size=600", client.banner);
        } else if (client.bannerColor != 0) {
            profile->setBackgroundColor(Settings::BackgroundTertiary);
            banner->setBackgroundColor(QColor((client.bannerColor & 0xFF000000) >> 24, (client.bannerColor & 0x00FF0000) >> 16, (client.bannerColor & 0x0000FF00) >> 8));
        } else {
            if (client.avatar.isNull()) {
                banner->setBackgroundColor(Settings::Black);
            } else {
                QString channelIconFileName = client.id + (client.avatar.indexOf("a_") == 0 ? ".gif" : ".png");
                rm->getImage([banner](Api::CallbackStruct cb){
                    QImage img(*reinterpret_cast<QString *>(cb.data));
                    int count = 0;
                    int r = 0, g = 0, b = 0;
                    for (int i = 0 ; i < img.width() ; i++) {
                        for (int j = 0 ; j < img.height() ; j++) {
                            count++;
                            QColor c = img.pixel(i, j);
                            r += c.red();
                            g += c.green();
                            b += c.blue();
                        }
                    }

                    r /= count;
                    g /= count;
                    b /= count;
                    banner->setBackgroundColor(QColor(r, g, b));
                }, "https://cdn.discordapp.com/avatars/" + client.id + "/" + client.avatar, channelIconFileName);
            }
        }

        Widget *nameAndEdit = new Widget(profile);
        nameAndEdit->setFixedHeight(Settings::scale(32));
        nameAndEdit->move(Settings::scale(120), Settings::scale(116));
        QHBoxLayout *nameAndEditLayout = new QHBoxLayout(nameAndEdit);
        nameAndEditLayout->setSpacing(0);
        nameAndEditLayout->setContentsMargins(0, 0, Settings::scale(16), 0);

        QFont font;
        font.setPixelSize(Settings::scale(20));
        font.setFamily("whitney");

        Label *name = new Label(client.username, nameAndEdit);
        name->setFixedSize(QFontMetrics(font).horizontalAdvance(client.username), Settings::scale(24));
        name->setFont(font);
        name->setTextColor(Settings::HeaderPrimary);

        Label *discriminator = new Label("#" + client.discriminator, nameAndEdit);
        discriminator->setFixedSize(Settings::scale(60), Settings::scale(24));
        discriminator->setFont(font);
        discriminator->setTextColor(Settings::HeaderSecondary);
        
        nameAndEditLayout->addWidget(name);
        nameAndEditLayout->addWidget(discriminator);
        nameAndEditLayout->addStretch(1);
    
        SettingsButton *editUserProfile = new SettingsButton(SettingsButton::Type::Normal, "Edit User Profile", profile);
        editUserProfile->move(Settings::scale(526), Settings::scale(116));


        Widget *avatarBorders = new Widget(profile);
        avatarBorders->setFixedSize(Settings::scale(94), Settings::scale(94));
        avatarBorders->move(Settings::scale(16), Settings::scale(76));
        avatarBorders->setBorderRadius(Settings::scale(47));
        avatarBorders->setBorderSize(Settings::scale(7));
        avatarBorders->setBorderColor(Settings::BackgroundTertiary);
                              
        RoundedImage *avatar;
        if (client.avatar.isNull()) {
            avatar = new RoundedImage(":user-icon-asset0.png", Settings::scale(80), Settings::scale(80), Settings::scale(40), profile);
        } else {
            avatar = new RoundedImage(Settings::scale(80), Settings::scale(80), Settings::scale(40), profile);
            QString channelIconFileName = client.id + (client.avatar.indexOf("a_") == 0 ? ".gif" : ".png");
            rm->getImage([avatar](Api::CallbackStruct cb){
                avatar->setRoundedImage(*reinterpret_cast<QString *>(cb.data));
            }, "https://cdn.discordapp.com/avatars/" + client.id + "/" + client.avatar, channelIconFileName);
        }
        avatar->move(Settings::scale(23), Settings::scale(83));
        avatar->setBorderRadius(Settings::scale(40));
        avatar->setBackgroundColor(Settings::None);

        Widget *statusBorders = new Widget(profile);
        statusBorders->setFixedSize(Settings::scale(26), Settings::scale(26));
        statusBorders->move(Settings::scale(75), Settings::scale(137));
        statusBorders->setBorderRadius(Settings::scale(13));
        statusBorders->setBorderSize(Settings::scale(5));
        statusBorders->setBorderColor(Settings::BackgroundTertiary);

        Widget *statusIcon = new Widget(profile);
        statusIcon->setFixedSize(Settings::scale(16), Settings::scale(16));
        statusIcon->setBorderRadius(Settings::scale(8));
        statusIcon->move(Settings::scale(80), Settings::scale(142));
        rm->getClientSettings([statusIcon](Api::CallbackStruct cb){
            QString status = reinterpret_cast<Api::ClientSettings *>(cb.data)->status;
            if (status == "online") statusIcon->setBackgroundColor(Settings::StatusOnline);
            else if (status == "idle") statusIcon->setBackgroundColor(Settings::StatusIdle);
            else if (status == "dnd") statusIcon->setBackgroundColor(Settings::StatusDND);
            else statusIcon->setBackgroundColor(Settings::StatusOffline);
        });


        Widget *editSection = new Widget(profile);
        editSection->move(Settings::scale(16), Settings::scale(180));
        editSection->setFixedSize(Settings::scale(628), Settings::scale(202));
        editSection->setBackgroundColor(Settings::BackgroundSecondaryAlt);
        editSection->setBorderRadius(Settings::scale(8));
        QVBoxLayout *editLayout = new QVBoxLayout(editSection);
        editLayout->setContentsMargins(Settings::scale(16), Settings::scale(16), Settings::scale(16), Settings::scale(16));
        editLayout->setSpacing(Settings::scale(24));


        Widget *usernameEdit = new Widget(editSection);
        usernameEdit->setFixedSize(Settings::scale(596), Settings::scale(40));

        font.setPixelSize(Settings::scale(14));
        Widget *username = new Widget(usernameEdit);
        QHBoxLayout *usernameLayout = new QHBoxLayout(usernameEdit);
        name = new Label(client.username, username);
        name->setTextColor(Settings::HeaderPrimary);
        name->setFixedSize(QFontMetrics(font).horizontalAdvance(client.username), Settings::scale(20));
        name->setFont(font);
        discriminator = new Label("#" + client.discriminator, username);
        discriminator->setFont(font);
        discriminator->setTextColor(Settings::HeaderSecondary);
        discriminator->setFixedSize(QFontMetrics(font).horizontalAdvance("#" + client.discriminator), Settings::scale(20));
        usernameLayout->addWidget(name, 0, Qt::AlignBottom);
        usernameLayout->addWidget(discriminator, 0, Qt::AlignBottom);
        usernameLayout->addStretch(1);
        usernameLayout->setSpacing(0);
        usernameLayout->setContentsMargins(0, 0, 0, 0);
        username->move(0, Settings::scale(20));
        
        font.setPixelSize(Settings::scale(12));
        Label *usernameTitle = new Label("USERNAME", usernameEdit);
        usernameTitle->setFixedHeight(Settings::scale(16));
        usernameTitle->setFont(font);
        usernameTitle->setTextColor(Settings::HeaderSecondary);

        SettingsButton *usernameEditButton = new SettingsButton(SettingsButton::Type::Edit, "Edit", usernameEdit);
        usernameEditButton->move(Settings::scale(536), Settings::scale(4));
        QObject::connect(usernameEditButton, &SettingsButton::clicked, [this, client](){
            QFont font;
            font.setPixelSize(Settings::scale(12));
            font.setFamily("whitney");

            Widget *content = new Widget(nullptr);

            Widget *usernameSection = new Widget(content);
            usernameSection->setFixedHeight(Settings::scale(68));
            QVBoxLayout *usernameLayout = new QVBoxLayout(usernameSection);
            usernameLayout->setSpacing(Settings::scale(8));
            usernameLayout->setContentsMargins(0, 0, 0, 0);
            Label *username = new Label("USERNAME", usernameSection);
            username->setFixedSize(QFontMetrics(font).horizontalAdvance("USERNAME"), Settings::scale(20));
            username->setFont(font);
            username->setTextColor(Settings::HeaderSecondary);
            
            Widget *inputs = new Widget(usernameSection);
            inputs->setBackgroundColor(Settings::BackgroundTertiary);
            inputs->setFixedHeight(Settings::scale(40));
            inputs->setBorderRadius(Settings::scale(3));
            QHBoxLayout *inputsLayout = new QHBoxLayout(inputs);
            inputsLayout->setSpacing(0);
            inputsLayout->setContentsMargins(0, 0, Settings::scale(4), 0);
            SettingsInput *nameInput = new SettingsInput(QString(), client.username, false, false, inputs);
            
            Widget *separator = new Widget(inputs);
            separator->setFixedWidth(Settings::scale(32));
            QHBoxLayout *separatorLayout = new QHBoxLayout(separator);
            separatorLayout->setSpacing(0);
            separatorLayout->setContentsMargins(0, 0, 0, 0);
            Widget *line = new Widget(separator);
            line->setFixedSize(Settings::scale(1), Settings::scale(30));
            line->setBackgroundColor(Settings::TextMuted);
            font.setPixelSize(Settings::scale(16));
            Label *prefix = new Label("#", separator);
            prefix->setFont(font);
            prefix->setTextColor(Settings::TextMuted);
            separatorLayout->addWidget(line);
            separatorLayout->addSpacing(Settings::scale(20));
            separatorLayout->addWidget(prefix);

            inputsLayout->addWidget(nameInput);
            inputsLayout->addWidget(separator);

            SettingsInput *discriminator;
            if (client.purchasedFlags) {
                discriminator = new SettingsInput(QString(), client.discriminator, false, false, inputs);
                discriminator->setFixedWidth(Settings::scale(70));
                discriminator->setContentsMargins(0, Settings::scale(10), Settings::scale(10), Settings::scale(10));
                inputsLayout->addWidget(discriminator);
            } else {
                discriminator = new SettingsInput(QString(), client.discriminator, true, false, inputs);
                discriminator->setFixedWidth(Settings::scale(70));
                SettingsButton *infoButton = new SettingsButton(SettingsButton::Type::Normal, "?", inputs);
                QObject::connect(infoButton, &SettingsButton::clicked, [this](){
                    QWidget *parentWidget = this;
                    while (parentWidget->parent()) parentWidget = (QWidget *)parentWidget->parent();
                    PopUp *popUp = new PopUp(new Widget(nullptr), Settings::scale(440), Settings::scale(120), QString(), QString(), true, false, "Get Nitro if you want to customize your tag", QString(), QString(), false, false, parentWidget->size(), parentWidget);
                    QObject::connect(popUp, &PopUp::cancelled, [popUp](){popUp->deleteLater();});
                });

                inputsLayout->addWidget(discriminator);
                inputsLayout->addWidget(infoButton);
            }

            usernameLayout->addWidget(username);
            usernameLayout->addWidget(inputs);

            Widget *passwordSection = new Widget(content);
            passwordSection->setFixedHeight(Settings::scale(68));
            QVBoxLayout *passwordLayout = new QVBoxLayout(passwordSection);
            passwordLayout->setSpacing(Settings::scale(8));
            passwordLayout->setContentsMargins(0, 0, 0, 0);
            font.setPixelSize(Settings::scale(12));
            Label *password = new Label("CURRENT PASSWORD", passwordSection);
            password->setFixedSize(QFontMetrics(font).horizontalAdvance("CURRENT PASSWORD"), Settings::scale(20));
            password->setFont(font);
            password->setTextColor(Settings::HeaderSecondary);
            SettingsInput *passwordInput = new SettingsInput(QString(), QString(), false, true, passwordSection);
            passwordLayout->addWidget(password);
            passwordLayout->addWidget(passwordInput);

            QVBoxLayout *layout = new QVBoxLayout(content);
            layout->setSpacing(Settings::scale(16));
            layout->setContentsMargins(0, 0, 0, 0);
            layout->addWidget(usernameSection);
            layout->addWidget(passwordSection);
            content->setContentsMargins(Settings::scale(16), 0, Settings::scale(16), 0);

            
            QWidget *parentWidget = this;
            while (parentWidget->parent()) parentWidget = (QWidget *)parentWidget->parent();
            PopUp *popUp = new PopUp(content, Settings::scale(440), Settings::scale(380), QString(), "Change your username", true, true, "<div style=\"text-align: center\">Enter a new username and your existing password.<div style=\"text-align: center\">", "Cancel", "Done", true, true, parentWidget->size(), parentWidget);
            QObject::connect(popUp, &PopUp::cancelled, [popUp](){popUp->deleteLater();});
            QObject::connect(popUp, &PopUp::done, [this, popUp, nameInput, discriminator, passwordInput, username, password](){
                username->setText("USERNAME");
                username->setFixedSize(QFontMetrics(QFont()).horizontalAdvance("USERNAME"), Settings::scale(16));
                username->setTextColor(Settings::HeaderSecondary);
                password->setText("CURRENT PASSWORD");
                password->setFixedSize(QFontMetrics(QFont()).horizontalAdvance("CURRENT PASSWORD"), Settings::scale(16));
                password->setTextColor(Settings::HeaderSecondary);
                rm->requester->changeUsername([this, popUp, username, password](Api::CallbackStruct cb){
                    if (cb.data == nullptr) {
                        popUp->deleteLater();
                    } else {
                        QVector<Api::Error *> errors = *reinterpret_cast<QVector<Api::Error *> *>(cb.data);
                        for (int i = 0 ; i < errors.size() ; i++) {
                            if (errors[i]->intCode == 0 || errors[i]->intCode == 1) {
                                username->setText("USERNAME - " + errors[i]->message);
                                username->setTextColor(Settings::Error);
                            } else {
                                password->setText("CURRENT PASSWORD - " + errors[i]->message);
                                password->setTextColor(Settings::Error);
                            }
                        }
                    }
                }, nameInput->input->text(), discriminator->input->text(), passwordInput->input->text());
            });
        });


        Widget *emailEdit = new Widget(editSection);
        emailEdit->setFixedSize(Settings::scale(596), Settings::scale(42));

        font.setPixelSize(Settings::scale(12));
        Label *emailTitle = new Label("EMAIL", emailEdit);
        emailTitle->setFixedHeight(Settings::scale(16));
        emailTitle->setFont(font);
        emailTitle->setTextColor(Settings::HeaderSecondary);

        font.setPixelSize(Settings::scale(14));
        Widget *emailAndReveal = new Widget(emailEdit);
        QHBoxLayout *emailAndRevealLayout = new QHBoxLayout(emailAndReveal);
        Label *email = new Label(emailAndReveal);
        email->setFixedSize(QFontMetrics(font).horizontalAdvance((client.email.size() > QString("**********" + client.email.mid(client.email.indexOf('@'))).size() ? client.email : QString("**********" + client.email.mid(client.email.indexOf('@'))))), Settings::scale(20));
        email->setFont(font);
        email->setTextColor(Settings::HeaderPrimary);
        email->setBackgroundColor(Settings::BackgroundSecondaryAlt);
        Reveal *reveal = new Reveal(email, client.email, "**********" + client.email.mid(client.email.indexOf('@')));
        emailAndRevealLayout->addWidget(email);
        emailAndRevealLayout->addWidget(reveal, 0, Qt::AlignTop);
        emailAndRevealLayout->addStretch(1);
        emailAndRevealLayout->setSpacing(Settings::scale(2));
        emailAndRevealLayout->setContentsMargins(0, 0, 0, 0);
        emailAndReveal->move(0, Settings::scale(20));

        SettingsButton *emailEditButton = new SettingsButton(SettingsButton::Type::Edit, "Edit", emailEdit);
        emailEditButton->move(Settings::scale(536), Settings::scale(5));
        QObject::connect(emailEditButton, &SettingsButton::clicked, [this, client](){
            QWidget *parentWidget = this;
            while (parentWidget->parent()) parentWidget = (QWidget *)parentWidget->parent();
            PopUp *popUp = new PopUp(new Widget(nullptr), Settings::scale(440), Settings::scale(306), QString(), "Verify email address", true, true, "<div style=\"text-align: center\">We'll need to verify your old email address, <b>" + client.email + "</b> in order to change it. <a style=\"color: #00AFF4; text-decoration: none;\" href=\"https://support.discord.com/hc/en-us/requests/new\">Lost access to your email? Go to https://support.discord.com/hc/en-us/requests/new.</a></div>", "Cancel", "Send Verification Code", true, false, parentWidget->size(), parentWidget);
            QObject::connect(popUp, &PopUp::cancelled, [popUp](){popUp->deleteLater();});
            QObject::connect(popUp, &PopUp::done, [this, popUp, parentWidget](){
                rm->requester->sendVerificationEmail([](Api::CallbackStruct cb){});
                popUp->deleteLater();

                QFont font;
                font.setPixelSize(Settings::scale(12));
                font.setFamily("whitney");
                Widget *content = new Widget(nullptr);
                content->setContentsMargins(Settings::scale(16), 0, Settings::scale(16), 0);
                QVBoxLayout *layout = new QVBoxLayout(content);
                layout->setSpacing(Settings::scale(8));
                layout->setContentsMargins(0, 0, 0, 0);
                Label *code = new Label("VERIFICATION CODE", content);
                code->setFixedSize(QFontMetrics(font).horizontalAdvance("VERIFICATION CODE"), Settings::scale(20));
                code->setFont(font);
                code->setTextColor(Settings::HeaderSecondary);
                SettingsInput *codeInput = new SettingsInput(QString(), QString(), false, false, content);
                SettingsButton *resend = new SettingsButton(SettingsButton::Type::NoBackground, "Didn't receive a code or it expired? Resend it.", content);
                layout->addWidget(code);
                layout->addWidget(codeInput);
                layout->addWidget(resend);

                PopUp *codePopUp = new PopUp(content, Settings::scale(440), Settings::scale(320), QString(), "Enter code", true, true, "Check your email: we sent you a verification code. Enter it here to verify you're really you.", "", "Next", true, true, parentWidget->size(), parentWidget);
                QObject::connect(codePopUp, &PopUp::cancelled, [codePopUp](){codePopUp->deleteLater();});
                QObject::connect(codePopUp, &PopUp::done, [this, parentWidget, codePopUp, codeInput, code](){
                    code->setText("VERIFICATION CODE");
                    code->setTextColor(Settings::HeaderSecondary);
                    rm->requester->sendVerifyCode([this, parentWidget, codePopUp, codeInput, code](Api::CallbackStruct cb){
                        if (cb.data == nullptr) {
                            codePopUp->deleteLater();

                            QFont font;
                            font.setPixelSize(12);
                            font.setFamily("whitney");

                            Widget *content = new Widget(nullptr);

                            Widget *emailSection = new Widget(content);
                            QVBoxLayout *emailLayout = new QVBoxLayout(emailSection);
                            emailLayout->setSpacing(Settings::scale(8));
                            emailLayout->setContentsMargins(0, 0, 0, 0);
                            Label *email = new Label("EMAIL", emailSection);
                            email->setFixedSize(QFontMetrics(font).horizontalAdvance("EMAIL"), Settings::scale(20));
                            email->setFont(font);
                            email->setTextColor(Settings::HeaderSecondary);
                            SettingsInput *emailInput = new SettingsInput(QString(), QString(), false, false, emailSection);
                            emailLayout->addWidget(email);
                            emailLayout->addWidget(emailInput);

                            Widget *passwordSection = new Widget(content);
                            QVBoxLayout *passwordLayout = new QVBoxLayout(passwordSection);
                            passwordLayout->setSpacing(Settings::scale(8));
                            passwordLayout->setContentsMargins(0, 0, 0, 0);
                            font.setPixelSize(Settings::scale(12));
                            Label *password = new Label("CURRENT PASSWORD", passwordSection);
                            password->setFixedSize(QFontMetrics(font).horizontalAdvance("CURRENT PASSWORD"), Settings::scale(20));
                            password->setFont(font);
                            password->setTextColor(Settings::HeaderSecondary);
                            SettingsInput *passwordInput = new SettingsInput(QString(), QString(), false, true, passwordSection);
                            passwordLayout->addWidget(password);
                            passwordLayout->addWidget(passwordInput);

                            QVBoxLayout *layout = new QVBoxLayout(content);
                            layout->setSpacing(Settings::scale(16));
                            layout->setContentsMargins(0, 0, 0, 0);
                            layout->addWidget(emailSection);
                            layout->addWidget(passwordSection);
                            content->setContentsMargins(Settings::scale(16), 0, Settings::scale(16), 0);

                            PopUp *emailPopUp = new PopUp(content, Settings::scale(440), Settings::scale(348), QString(), "Enter an email address", true, true, "Enter a new email address and your existing password.", "Back", "Done", true, true, parentWidget->size(), parentWidget);
                            QObject::connect(emailPopUp, &PopUp::cancelled, [emailPopUp](){emailPopUp->deleteLater();});
                            QObject::connect(emailPopUp, &PopUp::done, [this, emailPopUp, email, password, emailInput, passwordInput](){
                                email->setText("EMAIL");
                                email->setTextColor(Settings::HeaderSecondary);
                                password->setText("CURRENT PASSWORD");
                                password->setTextColor(Settings::HeaderSecondary);
                                rm->requester->changeEmail([this, emailPopUp, email, password](Api::CallbackStruct cb){
                                    if (cb.data == nullptr) {
                                        emailPopUp->deleteLater();
                                    } else {
                                        QVector<Api::Error *> errors = *reinterpret_cast<QVector<Api::Error *> *>(cb.data);
                                        for (int i = 0 ; i < errors.size() ; i++) {
                                            if (errors[i]->intCode == 0) {
                                                email->setText("EMAIL - " + errors[i]->message);
                                                email->setTextColor(Settings::Error);
                                            } else {
                                                password->setText("CURRENT PASSWORD - " + errors[i]->message);
                                                password->setTextColor(Settings::Error);
                                            }
                                        }
                                    }
                                }, emailInput->input->text(), passwordInput->input->text());
                            });
                        } else {
                            code->setText("USERNAME - " + reinterpret_cast<Api::Error *>(cb.data)->message);
                            code->setTextColor(Settings::Error);
                        }
                    }, codeInput->input->text());
                });
            });
        });


        Widget *phoneEdit = new Widget(editSection);
        phoneEdit->setFixedSize(Settings::scale(596), Settings::scale(40));

        font.setPixelSize(Settings::scale(12));
        Label *phoneTitle = new Label("PHONE NUMBER", phoneEdit);
        phoneTitle->setFont(font);
        phoneTitle->setTextColor(Settings::HeaderSecondary);

        if (client.phone.isNull()) {
            font.setPixelSize(14);
            Label *phone = new Label("You haven't added a phone number yet.", phoneEdit);
            phone->setFixedSize(QFontMetrics(font).horizontalAdvance("You haven't added a phone number yet."), Settings::scale(20));
            phone->setFont(font);
            phone->setTextColor(Settings::HeaderPrimary);
            phone->setBackgroundColor(Settings::BackgroundSecondaryAlt);
            phone->move(0, Settings::scale(20));

            SettingsButton *phoneEditButton = new SettingsButton(SettingsButton::Type::Edit, "Add", phoneEdit);
            phoneEditButton->move(Settings::scale(536), Settings::scale(4));
            QObject::connect(phoneEditButton, &SettingsButton::clicked, [this, client](){
                Widget *margins = new Widget(nullptr);
                QHBoxLayout *marginsLayout = new QHBoxLayout(margins);
                
                Widget *content = new Widget(margins);
                content->setFixedHeight(Settings::scale(46));
                content->setContentsMargins(Settings::scale(4), Settings::scale(4), Settings::scale(4), Settings::scale(4));
                content->setBackgroundColor(Settings::BackgroundTertiary);
                content->setBorderColor(Settings::Black);
                content->setBorderSize(Settings::scale(1));

                marginsLayout->addWidget(content);
                marginsLayout->setContentsMargins(Settings::scale(16), 0, Settings::scale(16), Settings::scale(16));
                
                SettingsInput *phone = new SettingsInput(QString(), "Not yet implemented", true, false, content);
                SettingsButton *send = new SettingsButton(SettingsButton::Type::Normal, "Send", nullptr);

                QHBoxLayout *layout = new QHBoxLayout(content);
                layout->setSpacing(0);
                layout->setContentsMargins(0, 0, 0, 0);
                layout->addWidget(phone);
                layout->addWidget(send);

                QWidget *parentWidget = this;
                while (parentWidget->parent()) parentWidget = (QWidget *)parentWidget->parent();
                PopUp *popUp = new PopUp(margins, Settings::scale(472), Settings::scale(300), QString(), "Enter a Phone Number", true, false, "<div style=\"text-align: center\">You will receive a text message with a verification code.<br><br>Your phone number can be used to verify <b>one Discord account</b> at a time and is only used for verification and login.</div>", "", "", false, false, parentWidget->size(), parentWidget);
                QObject::connect(popUp, &PopUp::cancelled, [popUp](){popUp->deleteLater();});
                QObject::connect(send, &SettingsButton::clicked, [popUp, parentWidget](){
                    popUp->deleteLater();
                    // TODO
                });
            });
        } else {
            font.setPixelSize(Settings::scale(14));
            Widget *phoneAndReveal = new Widget(phoneEdit);
            QHBoxLayout *phoneAndRevealLayout = new QHBoxLayout(phoneAndReveal);
            Label *phone = new Label(phoneAndReveal);
            phone->setFixedSize(QFontMetrics(font).horizontalAdvance((client.email.size() > QString("**********" + client.email.mid(client.email.indexOf('@'))).size() ? client.email : QString("**********" + client.email.mid(client.email.indexOf('@'))))), Settings::scale(20));
            phone->setFont(font);
            phone->setTextColor(Settings::HeaderPrimary);
            phone->setBackgroundColor(Settings::BackgroundSecondaryAlt);
            reveal = new Reveal(phone, client.phone, "********" + client.phone.right(4));
            phoneAndRevealLayout->addWidget(phone);
            phoneAndRevealLayout->addWidget(reveal, 0, Qt::AlignTop);
            phoneAndRevealLayout->addStretch(1);
            phoneAndRevealLayout->setSpacing(Settings::scale(2));
            phoneAndRevealLayout->setContentsMargins(0, 0, 0, 0);
            phoneAndReveal->move(0, Settings::scale(20));

            SettingsButton *phoneRemoveButton = new SettingsButton(SettingsButton::Type::NoBackground, "Remove", phoneEdit);
            phoneRemoveButton->move(Settings::scale(449), Settings::scale(4));
            QObject::connect(phoneRemoveButton, &SettingsButton::clicked, [this, client](){
                Widget *content = new Widget(nullptr);
                QVBoxLayout *layout = new QVBoxLayout(content);
                layout->setSpacing(0);
                layout->setContentsMargins(Settings::scale(16), 0, Settings::scale(16), 0);
                Label *password = new Label("PASSWORD", content);
                QFont font;
                font.setPixelSize(Settings::scale(12));
                font.setFamily("whitney");
                password->setFixedSize(QFontMetrics(font).horizontalAdvance("PASSWORD"), Settings::scale(20));
                password->setFont(font);
                password->setTextColor(Settings::HeaderSecondary);
                SettingsInput *input = new SettingsInput(QString(), QString(), false, true, content);
                layout->addWidget(password);
                layout->addWidget(input);

                QWidget *parentWidget = this;
                while (parentWidget->parent()) parentWidget = (QWidget *)parentWidget->parent();
                PopUp *popUp = new PopUp(content, Settings::scale(440), Settings::scale(272), QString(), "Remove phone number", false, false, (client.mfaEnabled ? "<div style=\"background-color: #FAA81A\">Removing your phone number will disable SMS backup for two-factor authentication.</div>" : QString()), "Cancel", "Remove", true, true, parentWidget->size(), parentWidget);
                QObject::connect(popUp, &PopUp::cancelled, [popUp](){popUp->deleteLater();});
                QObject::connect(popUp, &PopUp::done, [this, popUp, password, input](){
                    password->setText("PASSWORD");
                    password->setTextColor(Settings::HeaderSecondary);
                    rm->requester->removePhone([popUp, password](Api::CallbackStruct cb){
                        if (cb.data == nullptr) {
                            popUp->deleteLater();
                        } else {
                            password->setText("CURRENT PASSWORD - " + reinterpret_cast<Api::Error *>(cb.data)->message);
                            password->setTextColor(Settings::Error);
                        }
                    }, input->input->text());
                    
                });
            });

            SettingsButton *phoneEditButton = new SettingsButton(SettingsButton::Type::Edit, "Add", phoneEdit);
            phoneEditButton->move(Settings::scale(536), Settings::scale(4));
            QObject::connect(phoneEditButton, &SettingsButton::clicked, [this, client](){
                Widget *margins = new Widget(nullptr);
                QHBoxLayout *marginsLayout = new QHBoxLayout(margins);
                
                Widget *content = new Widget(margins);
                content->setFixedHeight(Settings::scale(46));
                content->setContentsMargins(Settings::scale(4), Settings::scale(4), Settings::scale(4), Settings::scale(4));
                content->setBackgroundColor(Settings::BackgroundTertiary);
                content->setBorderColor(Settings::Black);
                content->setBorderSize(Settings::scale(1));

                marginsLayout->addWidget(content);
                marginsLayout->setContentsMargins(Settings::scale(16), 0, Settings::scale(16), Settings::scale(16));
                
                SettingsInput *phone = new SettingsInput(QString(), "Not yet implemented", true, false, content);
                SettingsButton *send = new SettingsButton(SettingsButton::Type::Normal, "Send", nullptr);

                QHBoxLayout *layout = new QHBoxLayout(content);
                layout->setSpacing(0);
                layout->setContentsMargins(0, 0, 0, 0);
                layout->addWidget(phone);
                layout->addWidget(send);

                QWidget *parentWidget = this;
                while (parentWidget->parent()) parentWidget = (QWidget *)parentWidget->parent();
                PopUp *popUp = new PopUp(margins, Settings::scale(472), Settings::scale(300), QString(), "Enter a Phone Number", true, false, "<div style=\"text-align: center\">You will receive a text message with a verification code.<br><br>Your phone number can be used to verify <b>one Discord account</b> at a time and is only used for verification and login.</div>", "", "", false, false, parentWidget->size(), parentWidget);
                QObject::connect(popUp, &PopUp::cancelled, [popUp](){popUp->deleteLater();});
                QObject::connect(send, &SettingsButton::clicked, [popUp, parentWidget](){
                    popUp->deleteLater();
                    // TODO
                });
            });
        }


        editLayout->addWidget(usernameEdit);
        editLayout->addWidget(emailEdit);
        editLayout->addWidget(phoneEdit);
    });

    Label *sectionTitle = new Label("Password and Authentication", nullptr);
    sectionTitle->setFixedSize(QFontMetrics(font).horizontalAdvance("Password and Authentication"), Settings::scale(20));
    sectionTitle->setFont(font);
    sectionTitle->setTextColor(Settings::HeaderPrimary);

    SettingsButton *changePassword = new SettingsButton(SettingsButton::Type::Normal, "Change Password", container);
        QObject::connect(changePassword, &SettingsButton::clicked, [this](){
            QFont font;
            font.setPixelSize(Settings::scale(12));
            font.setFamily("whitney");

            Widget *content = new Widget(nullptr);

            QWidget *currentSection = new QWidget(content);
            QVBoxLayout *currentLayout = new QVBoxLayout(currentSection);
            currentLayout->setSpacing(Settings::scale(8));
            currentLayout->setContentsMargins(0, 0, 0, 0);
            Label *current = new Label("CURRENT PASSWORD", currentSection);
            current->setFixedSize(QFontMetrics(font).horizontalAdvance("CURRENT PASSWORD"), Settings::scale(20));
            current->setFont(font);
            current->setTextColor(Settings::HeaderSecondary);
            SettingsInput *currentInput = new SettingsInput(QString(), QString(), false, true, currentSection);
            currentLayout->addWidget(current);
            currentLayout->addWidget(currentInput);

            QWidget *newSection = new QWidget(content);
            QVBoxLayout *newLayout = new QVBoxLayout(newSection);
            newLayout->setSpacing(Settings::scale(8));
            newLayout->setContentsMargins(0, 0, 0, 0);
            font.setPixelSize(Settings::scale(12));
            Label *newPassword = new Label("NEW PASSWORD", newSection);
            newPassword->setFixedSize(QFontMetrics(font).horizontalAdvance("NEW PASSWORD"), Settings::scale(20));
            newPassword->setFont(font);
            newPassword->setTextColor(Settings::HeaderSecondary);
            SettingsInput *newInput = new SettingsInput(QString(), QString(), false, true, newSection);
            newLayout->addWidget(newPassword);
            newLayout->addWidget(newInput);

            QWidget *confirmSection = new QWidget(content);
            QVBoxLayout *confirmLayout = new QVBoxLayout(confirmSection);
            confirmLayout->setSpacing(Settings::scale(8));
            confirmLayout->setContentsMargins(0, 0, 0, 0);
            font.setPixelSize(Settings::scale(12));
            Label *confirm = new Label("CONFIRM NEW PASSWORD", confirmSection);
            confirm->setFixedSize(QFontMetrics(font).horizontalAdvance("CONFIRM NEW PASSWORD"), Settings::scale(20));
            confirm->setFont(font);
            confirm->setTextColor(Settings::HeaderSecondary);
            SettingsInput *confirmInput = new SettingsInput(QString(), QString(), false, true, confirmSection);
            confirmLayout->addWidget(confirm);
            confirmLayout->addWidget(confirmInput);

            QVBoxLayout *layout = new QVBoxLayout(content);
            layout->setSpacing(Settings::scale(16));
            layout->setContentsMargins(0, 0, 0, 0);
            layout->addWidget(currentSection);
            layout->addWidget(newSection);
            layout->addWidget(confirmSection);
            content->setContentsMargins(Settings::scale(16), 0, Settings::scale(16), 0);

            QWidget *parentWidget = this;
            while (parentWidget->parent()) parentWidget = (QWidget *)parentWidget->parent();
            PopUp *popUp = new PopUp(content, Settings::scale(440), Settings::scale(416), QString(), "Update your password", true, true, "Enter your current password and a new password.", "Cancel", "Done", true, true, parentWidget->size(), parentWidget);
            QObject::connect(popUp, &PopUp::cancelled, [popUp](){popUp->deleteLater();});
            QObject::connect(popUp, &PopUp::done, [this, popUp, current, currentInput, newPassword, newInput, confirm, confirmInput](){
                current->setText("CURRENT PASSWORD");
                current->setTextColor(Settings::HeaderSecondary);
                newPassword->setText("NEW PASSWORD");
                newPassword->setTextColor(Settings::HeaderSecondary);
                confirm->setText("CONFIRM NEW  PASSWORD");
                confirm->setTextColor(Settings::HeaderSecondary);
                if (newInput->input->text() != confirmInput->input->text()) {
                    confirm->setText("CONFIRM NEW PASSWORD - Passwords do not match!");
                    confirm->setTextColor(Settings::Error);
                } else {
                    rm->requester->changePassword([popUp, current, newPassword](Api::CallbackStruct cb){
                        if (cb.data == nullptr) {
                            popUp->deleteLater();
                        } else {
                            QVector<Api::Error *> errors = *reinterpret_cast<QVector<Api::Error *> *>(cb.data);
                            for (int i = 0 ; i < errors.size() ; i++) {
                                if (errors[i]->intCode == 0) {
                                    current->setText("CURRENT PASSWORD - " + errors[i]->message);
                                    current->setTextColor(Settings::Error);
                                } else {
                                    newPassword->setText("NEW PASSWORD - " + errors[i]->message);
                                    newPassword->setTextColor(Settings::Error);
                                }
                            }
                        }
                    }, currentInput->input->text(), newInput->input->text());
                }
            });
        });

    font.setPixelSize(Settings::scale(12));
    font.setBold(true);


    Label *accountRemoval = new Label("ACCOUNT REMOVAL", container);
    accountRemoval->setFixedSize(QFontMetrics(font).horizontalAdvance("ACCOUNT REMOVAL"), Settings::scale(16));
    accountRemoval->setFont(font);
    accountRemoval->setTextColor(Settings::HeaderSecondary);

    font.setBold(false);
    font.setPixelSize(Settings::scale(13));
    Label *accountRemovalDesc = new Label("Disabling your account means you can recover "
                                          "it at any time after taking this action", nullptr);
    accountRemovalDesc->setFixedHeight(Settings::scale(40));
    accountRemovalDesc->setTextColor(Settings::HeaderSecondary);
    accountRemovalDesc->setFont(font);

    Widget *accountButtons = new Widget(container);
    QHBoxLayout *accountButtonsLayout = new QHBoxLayout(accountButtons);
    SettingsButton *disableAccount = new SettingsButton(SettingsButton::Type::Important, "Disable Account", accountButtons);
    SettingsButton *deleteAccount = new SettingsButton(SettingsButton::Type::Critical, "Delete Account", accountButtons);
    QObject::connect(disableAccount, &SettingsButton::clicked, [this](){
        rm->getClient([this](Api::CallbackStruct cb){
            Api::Client client = *reinterpret_cast<Api::Client *>(cb.data);
            rm->getGuilds([this, client](Api::CallbackStruct cb){
                QVector<Api::Guild *> guilds = *reinterpret_cast<QVector<Api::Guild *> *>(cb.data);
                bool owner = false;
                for (int i = 0 ; i < guilds.size() ; i++) {
                    if (guilds[i]->ownerId == client.id) {
                        owner = true;
                        break;
                    }
                }
                QWidget *parentWidget = this;
                while (parentWidget->parent()) parentWidget = (QWidget *)parentWidget->parent();
                if (owner) {
                    PopUp *popUp = new PopUp(new Widget(nullptr), Settings::scale(440), Settings::scale(240), QString(), "You Own Servers", false, false, "In order to delete or disable your account you must first transfer ownership of all servers that you own.", "", "Okay", true, true, parentWidget->size(), parentWidget);
                    QObject::connect(popUp, &PopUp::cancelled, [popUp](){popUp->deleteLater();});
                    QObject::connect(popUp, &PopUp::done, [popUp](){popUp->deleteLater();});
                } else {
                    Widget *content = new Widget(nullptr);
                    content->setContentsMargins(Settings::scale(16), 0, Settings::scale(16), 0);
                    QVBoxLayout *layout = new QVBoxLayout(content);
                    layout->setSpacing(Settings::scale(8));
                    layout->setContentsMargins(0, 0, 0, 0);
                    Label *password = new Label("PASSWORD", content);
                    QFont font;
                    font.setPixelSize(Settings::scale(12));
                    font.setFamily("whitney");
                    password->setFont(font);
                    password->setTextColor(Settings::HeaderSecondary);
                    SettingsInput *input = new SettingsInput(QString(), QString(), false, true, content);
                    layout->addWidget(password);
                    layout->addWidget(input);

                    PopUp *popUp = new PopUp(content, Settings::scale(440), Settings::scale(312), QString(), "Delete Account", false, false, "<div style=\"background-color: #FAA81A\">Are you sure that you want to disable your account? This will immediatly log you out of your account and make your account inaccessible to anyone.", "Cancel", "Disable Account", true, true, parentWidget->size(), parentWidget);
                    QObject::connect(popUp, &PopUp::cancelled, [this, popUp](){popUp->deleteLater();});
                    QObject::connect(popUp, &PopUp::done, [this, popUp, parentWidget, password, input](){
                        password->setText("PASSWORD");
                        password->setTextColor(Settings::HeaderSecondary);
                        rm->requester->disableAccount([this, popUp, parentWidget, password](Api::CallbackStruct cb){
                            if (cb.data == nullptr) {
                                popUp->deleteLater();

                                PopUp *closePopUp = new PopUp(new Widget(nullptr), Settings::scale(440), Settings::scale(200), QString(), QString(), true, false, "Your account has been disabled successfully, Fast-Discord will now exit.", QString(), QString(), false, false, parentWidget->size(), parentWidget);
                                QObject::connect(closePopUp, &PopUp::cancelled, [](){
                                    Settings::saveSettings();
                                    exit(0);
                                });
                            } else {
                                password->setText("PASSWORD - " + reinterpret_cast<Api::Error *>(cb.data)->message);
                                password->setTextColor(Settings::Error);
                            }
                        }, input->input->text());
                    });
                }
            });
        });
        
    });
    QObject::connect(deleteAccount, &SettingsButton::clicked, [this](){
        rm->getClient([this](Api::CallbackStruct cb){
            Api::Client client = *reinterpret_cast<Api::Client *>(cb.data);
            rm->getGuilds([this, client](Api::CallbackStruct cb){
                QVector<Api::Guild *> guilds = *reinterpret_cast<QVector<Api::Guild *> *>(cb.data);
                bool owner = false;
                for (int i = 0 ; i < guilds.size() ; i++) {
                    if (guilds[i]->ownerId == client.id) {
                        owner = true;
                        break;
                    }
                }
                QWidget *parentWidget = this;
                while (parentWidget->parent()) parentWidget = (QWidget *)parentWidget->parent();
                if (owner) {
                    PopUp *popUp = new PopUp(new Widget(nullptr), Settings::scale(440), Settings::scale(240), QString(), "You Own Servers", false, false, "In order to delete or disable your account you must first transfer ownership of all servers that you own.", "", "Okay", true, true, parentWidget->size(), parentWidget);
                    QObject::connect(popUp, &PopUp::cancelled, [popUp](){popUp->deleteLater();});
                    QObject::connect(popUp, &PopUp::done, [popUp](){popUp->deleteLater();});
                } else {
                    Widget *content = new Widget(nullptr);
                    content->setContentsMargins(Settings::scale(16), 0, Settings::scale(16), 0);
                    QVBoxLayout *layout = new QVBoxLayout(content);
                    layout->setSpacing(Settings::scale(8));
                    layout->setContentsMargins(0, 0, 0, 0);
                    Label *password = new Label("PASSWORD", content);
                    QFont font;
                    font.setPixelSize(Settings::scale(12));
                    font.setFamily("whitney");
                    password->setFont(font);
                    password->setTextColor(Settings::HeaderSecondary);
                    SettingsInput *input = new SettingsInput(QString(), QString(), false, true, content);
                    layout->addWidget(password);
                    layout->addWidget(input);

                    PopUp *popUp = new PopUp(content, Settings::scale(440), Settings::scale(312), QString(), "Disable Account", false, false, "<div style=\"background-color: #FAA81A\">Are you sure that you want to delete your account? This will immediatly log you out of your account and you will not be able to log in again.", "Cancel", "Delete Account", true, true, parentWidget->size(), parentWidget);
                    QObject::connect(popUp, &PopUp::cancelled, [popUp](){popUp->deleteLater();});
                    QObject::connect(popUp, &PopUp::done, [this, popUp, parentWidget, password, input](){
                        password->setText("CURRENT PASSWORD");
                        password->setTextColor(Settings::HeaderSecondary);
                        rm->requester->deleteAccount([this, popUp, parentWidget, password](Api::CallbackStruct cb){
                            if (cb.data == nullptr) {
                                popUp->deleteLater();

                                PopUp *closePopUp = new PopUp(new Widget(nullptr), Settings::scale(440), Settings::scale(200), QString(), QString(), true, false, "Your account has been deleted successfully, Fast-Discord will now exit.", QString(), QString(), false, false, parentWidget->size(), parentWidget);
                                QObject::connect(closePopUp, &PopUp::cancelled, [](){
                                    Settings::saveSettings();
                                    exit(0);
                                });
                            } else {
                                password->setText("PASSWORD - " + reinterpret_cast<Api::Error *>(cb.data)->message);
                                password->setTextColor(Settings::Error);
                            }
                        }, input->input->text());
                    });
                }
            });
        });
    });
    accountButtonsLayout->addWidget(disableAccount);
    accountButtonsLayout->addWidget(deleteAccount);
    accountButtonsLayout->addStretch(1);
    accountButtonsLayout->setSpacing(Settings::scale(16));
    accountButtonsLayout->setContentsMargins(0, 0, 0, 0);

    layout->addWidget(title);
    layout->addSpacing(Settings::scale(20));
    layout->addWidget(profile);
    layout->addWidget(new Divider());
    layout->addWidget(sectionTitle);
    layout->addSpacing(Settings::scale(20));
    layout->addWidget(changePassword);
    layout->addWidget(new Divider());
    layout->addWidget(accountRemoval);
    layout->addSpacing(Settings::scale(8));
    layout->addWidget(accountRemovalDesc);
    layout->addSpacing(Settings::scale(16));
    layout->addWidget(accountButtons);

    this->setWidget(container);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setStyleSheet("* {border: none; background-color: " + Settings::colors[Settings::BackgroundPrimary].name() + "}"
                        "QScrollBar::handle:vertical {border: none; border-radius: " + QString::number(Settings::scale(2)) + "px; background-color: " + Settings::colors[Settings::BackgroundTertiary].name() + ";}"
                        "QScrollBar:vertical {border: none; background-color: " + Settings::colors[Settings::BackgroundSecondary].name() + "; border-radius: " + QString::number(Settings::scale(8)) + "px; width: " + QString::number(Settings::scale(3)) + "px;}"
                        "QScrollBar::add-line, QScrollBar::sub-line {border:none; background: none; height: 0;}"
                        "QScrollBar:left-arrow:vertical, QScrollBar::right-arrow:vertical {background: none;}"
                        "QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {background: none;}");
}

} // namespace Ui