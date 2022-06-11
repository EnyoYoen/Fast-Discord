#include "ui/settings/myaccount.h"

#include "ui/settings/settingsbutton.h"
#include "ui/settings/settingsinput.h"
#include "ui/settings/popup.h"
#include "ui/settings/divider.h"
#include "api/objects/client.h"
#include "api/objects/error.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QScrollBar>

namespace Ui {

class Reveal : public QLabel
{
public:
    Reveal(QLabel *labelp, QString revealStrp, QString hiddenStrp)
    {
        reveal = false;
        label = labelp;
        revealStr = revealStrp;
        hiddenStr = hiddenStrp;
        label->setText(hiddenStrp);
        
        font.setPixelSize(14);
        font.setFamily("whitney");
        this->setText("Reveal");
        this->setFont(font);
        this->setStyleSheet("color: #00AFF4");
        this->setCursor(Qt::CursorShape::PointingHandCursor);
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
    QLabel *label;
    bool reveal;
};

MyAccount::MyAccount(Api::RessourceManager *rmp, QWidget *parent)
    : QScrollArea()
{
    rm = rmp;

    QWidget *container = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(container);
    container->setMaximumWidth(752);
    container->setContentsMargins(40, 60, 40, 80);
    container->setStyleSheet("background-color: #36393F");

    QFont font;
    font.setPixelSize(20);
    font.setFamily("whitney");

    QLabel *title = new QLabel("My Account", container);
    title->setFont(font);
    title->setStyleSheet("color: #FFF;");

    QLabel *profile = new QLabel(container);
    profile->setFixedHeight(408);
    profile->setStyleSheet("background-color: #202225;"
                           "border-radius: 8px;");
    rm->getClient([this, profile](void *clientPtr){
        Api::Client client = *reinterpret_cast<Api::Client *>(clientPtr);
    
        QLabel *banner = new QLabel(profile);
        banner->setFixedSize(660, 100);
        
        if (!client.banner.isNull()) {
            rm->getImage([this, banner](void *imageFileName){
                banner->setPixmap(QPixmap(*reinterpret_cast<QString *>(imageFileName)).scaledToWidth(660));
            }, "https://cdn.discordapp.com/banners/" + client.id + "/" + client.banner + ".png?size=600", client.banner);
        } else if (client.bannerColor != 0) {
            banner->setStyleSheet("background-color: #" + QString::number(client.bannerColor, 16));
        } else {
            if (client.avatar.isNull()) {
                banner->setStyleSheet("background-color: #000");
            } else {
                QString channelIconFileName = client.id + (client.avatar.indexOf("a_") == 0 ? ".gif" : ".png");
                rm->getImage([banner](void *imageFileName){
                    QImage img(*reinterpret_cast<QString *>(imageFileName));
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
                    banner->setStyleSheet("background-color: #" + (r < 16 ? "0" + QString::number(r, 16) : QString::number(r, 16)) + (g < 16 ? "0" + QString::number(g, 16) : QString::number(g, 16)) + (b < 16 ? "0" + QString::number(b, 16) : QString::number(b, 16)) + ";");
                }, "https://cdn.discordapp.com/avatars/" + client.id + "/" + client.avatar, channelIconFileName);
            }
        }

        QWidget *nameAndEdit = new QWidget(profile);
        nameAndEdit->setFixedHeight(32);
        nameAndEdit->move(120, 116);
        QHBoxLayout *nameAndEditLayout = new QHBoxLayout(nameAndEdit);
        nameAndEditLayout->setSpacing(0);
        nameAndEditLayout->setContentsMargins(0, 0, 16, 0);

        QFont font;
        font.setPixelSize(20);
        font.setFamily("whitney");

        QLabel *name = new QLabel(client.username, nameAndEdit);
        name->setFont(font);
        name->setStyleSheet("color: #FFF");

        QLabel *discriminator = new QLabel("#" + client.discriminator, nameAndEdit);
        discriminator->setFont(font);
        discriminator->setStyleSheet("color: #B9BBBE");
        
        nameAndEditLayout->addWidget(name);
        nameAndEditLayout->addWidget(discriminator);
        nameAndEditLayout->addStretch(1);
    
        SettingsButton *editUserProfile = new SettingsButton(SettingsButton::Type::Normal, "Edit User Profile", profile);
        editUserProfile->move(526, 116);


        QLabel *avatarBorders = new QLabel(profile);
        avatarBorders->setFixedSize(94, 94);
        avatarBorders->move(16, 76);
        avatarBorders->setStyleSheet("border: 7px solid #202225;"
                                     "border-radius: 47px;");
                              
        RoundedImage *avatar;
        if (client.avatar.isNull()) {
            avatar = new RoundedImage("res/images/png/user-icon-asset0.png", 80, 80, 40, profile);
        } else {
            avatar = new RoundedImage(80, 80, 40, profile);
            QString channelIconFileName = client.id + (client.avatar.indexOf("a_") == 0 ? ".gif" : ".png");
            rm->getImage([avatar](void *imageFileName){
                avatar->setImage(*reinterpret_cast<QString *>(imageFileName));
            }, "https://cdn.discordapp.com/avatars/" + client.id + "/" + client.avatar, channelIconFileName);
        }
        avatar->move(23, 83);
        avatar->setStyleSheet("border-radius: 40px;"
                              "background-color: none;");

        QLabel *statusBorders = new QLabel(profile);
        statusBorders->setFixedSize(26, 26);
        statusBorders->move(75, 137);
        statusBorders->setStyleSheet("border: 5px solid #202225;"
                                     "border-radius: 13px;");

        QLabel *statusIcon = new QLabel(profile);
        statusIcon->setFixedSize(16, 16);
        statusIcon->setStyleSheet("border-radius: 8px;");
        statusIcon->move(80, 142);
        rm->getClientSettings([statusIcon](void *settingsPtr){
            QString status = reinterpret_cast<Api::ClientSettings *>(settingsPtr)->status;
            if (status == "online") statusIcon->setStyleSheet("border-radius: 8px;"
                                                              "background-color: rgb(0, 224, 71);");
            else if (status == "idle") statusIcon->setStyleSheet("border-radius: 8px;"
                                                                 "background-color: rgb(255, 169, 21);");
            else if (status == "dnd") statusIcon->setStyleSheet("border-radius: 8px;"
                                                                "background-color: rgb(255, 48, 51);");
            else statusIcon->setStyleSheet("border-radius: 8px;"
                                           "background-color: rgb(90, 90, 90);");
        });


        QWidget *editSection = new QWidget(profile);
        editSection->move(16, 180);
        editSection->setFixedSize(628, 202);
        editSection->setStyleSheet("background-color: #2F3136");
        QVBoxLayout *editLayout = new QVBoxLayout(editSection);
        editLayout->setContentsMargins(16, 16, 16, 16);
        editLayout->setSpacing(24);


        QWidget *usernameEdit = new QWidget(editSection);
        usernameEdit->setFixedSize(596, 40);

        font.setPixelSize(14);
        QWidget *username = new QWidget(usernameEdit);
        QHBoxLayout *usernameLayout = new QHBoxLayout(usernameEdit);
        name = new QLabel(client.username, username);
        name->setFont(font);
        name->setStyleSheet("color: #FFF");
        discriminator = new QLabel("#" + client.discriminator, username);
        discriminator->setFont(font);
        discriminator->setStyleSheet("color: #B9BBBE");
        usernameLayout->addWidget(name, 0, Qt::AlignBottom);
        usernameLayout->addWidget(discriminator, 0, Qt::AlignBottom);
        usernameLayout->addStretch(1);
        usernameLayout->setSpacing(0);
        usernameLayout->setContentsMargins(0, 0, 0, 0);
        username->move(0, 20);
        
        font.setPixelSize(12);
        QLabel *usernameTitle = new QLabel("USERNAME", usernameEdit);
        usernameTitle->setFixedHeight(16);
        usernameTitle->setFont(font);
        usernameTitle->setStyleSheet("color: #B9BBBE");

        SettingsButton *usernameEditButton = new SettingsButton(SettingsButton::Type::Edit, "Edit", usernameEdit);
        usernameEditButton->move(536, 4);
        QObject::connect(usernameEditButton, &SettingsButton::clicked, [this, client](){
            QFont font;
            font.setPixelSize(12);
            font.setFamily("whitney");

            QWidget *content = new QWidget();

            QWidget *usernameSection = new QWidget(content);
            QVBoxLayout *usernameLayout = new QVBoxLayout(usernameSection);
            usernameLayout->setSpacing(8);
            usernameLayout->setContentsMargins(0, 0, 0, 0);
            QLabel *username = new QLabel("USERNAME", usernameSection);
            username->setFont(font);
            username->setStyleSheet("color: #B9BBBE");
            
            QWidget *inputs = new QWidget(usernameSection);
            inputs->setStyleSheet("border-radius: 3px;"
                                  "background-color: #202225;");
            QHBoxLayout *inputsLayout = new QHBoxLayout(inputs);
            inputsLayout->setSpacing(0);
            inputsLayout->setContentsMargins(0, 0, 4, 0);
            SettingsInput *nameInput = new SettingsInput(QString(), client.username, false, false, inputs);
            
            QWidget *separator = new QWidget(inputs);
            separator->setFixedWidth(32);
            QHBoxLayout *separatorLayout = new QHBoxLayout(separator);
            separatorLayout->setSpacing(0);
            separatorLayout->setContentsMargins(0, 0, 0, 0);
            QWidget *line = new QWidget(separator);
            line->setFixedSize(1, 30);
            line->setStyleSheet("background-color: #A3A6AA");
            font.setPixelSize(16);
            QLabel *prefix = new QLabel("#");
            prefix->setFont(font);
            prefix->setStyleSheet("color: #A3A6AA");
            separatorLayout->addWidget(line);
            separatorLayout->addSpacing(20);
            separatorLayout->addWidget(prefix);

            inputsLayout->addWidget(nameInput);
            inputsLayout->addWidget(separator);

            SettingsInput *discriminator;
            if (client.purchasedFlags) {
                discriminator = new SettingsInput(QString(), client.discriminator, false, false, inputs);
                discriminator->setFixedWidth(70);
                discriminator->setContentsMargins(0, 10, 10, 10);
                inputsLayout->addWidget(discriminator);
            } else {
                discriminator = new SettingsInput(QString(), client.discriminator, true, false, inputs);
                discriminator->setFixedWidth(70);
                SettingsButton *infoButton = new SettingsButton(SettingsButton::Type::Normal, "?", inputs);
                QObject::connect(infoButton, &SettingsButton::clicked, [this](){
                    QWidget *parentWidget = this;
                    while (parentWidget->parent()) parentWidget = (QWidget *)parentWidget->parent();
                    PopUp *popUp = new PopUp(new QWidget(), 440, 100, QString(), QString(), true, false, "Get Nitro if you want to customize your tag", QString(), QString(), false, false, parentWidget->size(), parentWidget);
                    QObject::connect(popUp, &PopUp::cancelled, [popUp](){popUp->deleteLater();});
                });

                inputsLayout->addWidget(discriminator);
                inputsLayout->addWidget(infoButton);
            }

            usernameLayout->addWidget(username);
            usernameLayout->addWidget(inputs);

            QWidget *passwordSection = new QWidget(content);
            QVBoxLayout *passwordLayout = new QVBoxLayout(passwordSection);
            passwordLayout->setSpacing(8);
            passwordLayout->setContentsMargins(0, 0, 0, 0);
            font.setPixelSize(12);
            QLabel *password = new QLabel("CURRENT PASSWORD", passwordSection);
            password->setFont(font);
            password->setStyleSheet("color: #B9BBBE");
            SettingsInput *passwordInput = new SettingsInput(QString(), QString(), false, true, passwordSection);
            passwordLayout->addWidget(password);
            passwordLayout->addWidget(passwordInput);

            QVBoxLayout *layout = new QVBoxLayout(content);
            layout->setSpacing(16);
            layout->setContentsMargins(0, 0, 0, 0);
            layout->addWidget(usernameSection);
            layout->addWidget(passwordSection);
            content->setContentsMargins(16, 0, 16, 0);

            
            QWidget *parentWidget = this;
            while (parentWidget->parent()) parentWidget = (QWidget *)parentWidget->parent();
            PopUp *popUp = new PopUp(content, 440, 336, QString(), "Change your username", true, true, "<div style=\"text-align: center\">Enter a new username and your existing password.<div style=\"text-align: center\">", "Cancel", "Done", true, true, parentWidget->size(), parentWidget);
            QObject::connect(popUp, &PopUp::cancelled, [popUp](){popUp->deleteLater();});
            QObject::connect(popUp, &PopUp::done, [this, popUp, nameInput, discriminator, passwordInput, username, password](){
                username->setText("USERNAME");
                username->setStyleSheet("color: #B9BBBE");
                password->setText("CURRENT PASSWORD");
                password->setStyleSheet("color: #B9BBBE");
                rm->requester->changeUsername([this, popUp, username, password](void *errorsPtr){
                    if (errorsPtr == nullptr) {
                        popUp->deleteLater();
                    } else {
                        QVector<Api::Error *> errors = *reinterpret_cast<QVector<Api::Error *> *>(errorsPtr);
                        for (unsigned int i = 0 ; i < errors.size() ; i++) {
                            if (errors[i]->intCode == 0 || errors[i]->intCode == 1) {
                                username->setText("USERNAME - " + errors[i]->message);
                                username->setStyleSheet("color: #F38688");
                            } else {
                                password->setText("CURRENT PASSWORD - " + errors[i]->message);
                                password->setStyleSheet("color: #F38688");
                            }
                        }
                    }
                }, nameInput->input->text(), discriminator->input->text(), passwordInput->input->text());
            });
        });


        QWidget *emailEdit = new QWidget(editSection);
        emailEdit->setFixedSize(596, 42);

        font.setPixelSize(12);
        QLabel *emailTitle = new QLabel("EMAIL", emailEdit);
        emailTitle->setFont(font);
        emailTitle->setStyleSheet("color: #B9BBBE");

        font.setPixelSize(14);
        QWidget *emailAndReveal = new QWidget(emailEdit);
        QHBoxLayout *emailAndRevealLayout = new QHBoxLayout(emailAndReveal);
        QLabel *email = new QLabel(emailAndReveal);
        email->setFont(font);
        email->setStyleSheet("color: #FFF");
        Reveal *reveal = new Reveal(email, client.email, "**********" + client.email.mid(client.email.indexOf('@')));
        emailAndRevealLayout->addWidget(email);
        emailAndRevealLayout->addWidget(reveal, 0, Qt::AlignTop);
        emailAndRevealLayout->addStretch(1);
        emailAndRevealLayout->setSpacing(2);
        emailAndRevealLayout->setContentsMargins(0, 0, 0, 0);
        emailAndReveal->move(0, 20);

        SettingsButton *emailEditButton = new SettingsButton(SettingsButton::Type::Edit, "Edit", emailEdit);
        emailEditButton->move(536, 5);
        QObject::connect(emailEditButton, &SettingsButton::clicked, [this, client](){
            QWidget *parentWidget = this;
            while (parentWidget->parent()) parentWidget = (QWidget *)parentWidget->parent();
            PopUp *popUp = new PopUp(new QWidget(), 440, 306, QString(), "Verify email address", true, true, "<div style=\"text-align: center\">We'll need to verify your old email address, <b>" + client.email + "</b> in order to change it. <a style=\"color: #00AFF4; text-decoration: none;\" href=\"https://support.discord.com/hc/en-us/requests/new\">Lost access to your email? Go to https://support.discord.com/hc/en-us/requests/new.</a></div>", "Cancel", "Send Verification Code", true, false, parentWidget->size(), parentWidget);
            QObject::connect(popUp, &PopUp::cancelled, [popUp](){popUp->deleteLater();});
            QObject::connect(popUp, &PopUp::done, [this, popUp, parentWidget](){
                rm->requester->sendVerificationEmail();
                popUp->deleteLater();

                QFont font;
                font.setPixelSize(12);
                font.setFamily("whitney");
                QWidget *content = new QWidget();
                content->setContentsMargins(16, 0, 16, 0);
                QVBoxLayout *layout = new QVBoxLayout(content);
                layout->setSpacing(8);
                layout->setContentsMargins(0, 0, 0, 0);
                QLabel *code = new QLabel("VERIFICATION CODE", content);
                code->setFont(font);
                code->setStyleSheet("color: #B9BBBE");
                SettingsInput *codeInput = new SettingsInput(QString(), QString(), false, false, content);
                SettingsButton *resend = new SettingsButton(SettingsButton::Type::NoBackground, "Didn't receive a code or it expired? Resend it.", content);
                layout->addWidget(code);
                layout->addWidget(codeInput);
                layout->addWidget(resend);

                PopUp *codePopUp = new PopUp(content, 440, 320, QString(), "Enter code", true, true, "Check your email: we sent you a verification code. Enter it here to verify you're really you.", "", "Next", true, true, parentWidget->size(), parentWidget);
                QObject::connect(codePopUp, &PopUp::cancelled, [codePopUp](){codePopUp->deleteLater();});
                QObject::connect(codePopUp, &PopUp::done, [this, parentWidget, codePopUp, codeInput, code](){
                    code->setText("VERIFICATION CODE");
                    code->setStyleSheet("color: #B9BBBE");
                    rm->requester->sendVerifyCode([this, parentWidget, codePopUp, codeInput, code](void *errorPtr){
                        if (errorPtr == nullptr) {
                            codePopUp->deleteLater();

                            QFont font;
                            font.setPixelSize(12);
                            font.setFamily("whitney");

                            QWidget *content = new QWidget();

                            QWidget *emailSection = new QWidget(content);
                            QVBoxLayout *emailLayout = new QVBoxLayout(emailSection);
                            emailLayout->setSpacing(8);
                            emailLayout->setContentsMargins(0, 0, 0, 0);
                            QLabel *email = new QLabel("EMAIL", emailSection);
                            email->setFont(font);
                            email->setStyleSheet("color: #B9BBBE");
                            SettingsInput *emailInput = new SettingsInput(QString(), QString(), false, false, emailSection);
                            emailLayout->addWidget(email);
                            emailLayout->addWidget(emailInput);

                            QWidget *passwordSection = new QWidget(content);
                            QVBoxLayout *passwordLayout = new QVBoxLayout(passwordSection);
                            passwordLayout->setSpacing(8);
                            passwordLayout->setContentsMargins(0, 0, 0, 0);
                            font.setPixelSize(12);
                            QLabel *password = new QLabel("CURRENT PASSWORD", passwordSection);
                            password->setFont(font);
                            password->setStyleSheet("color: #B9BBBE");
                            SettingsInput *passwordInput = new SettingsInput(QString(), QString(), false, true, passwordSection);
                            passwordLayout->addWidget(password);
                            passwordLayout->addWidget(passwordInput);

                            QVBoxLayout *layout = new QVBoxLayout(content);
                            layout->setSpacing(16);
                            layout->setContentsMargins(0, 0, 0, 0);
                            layout->addWidget(emailSection);
                            layout->addWidget(passwordSection);
                            content->setContentsMargins(16, 0, 16, 0);

                            PopUp *emailPopUp = new PopUp(content, 440, 348, QString(), "Enter an email address", true, true, "Enter a new email address and your existing password.", "Back", "Done", true, true, parentWidget->size(), parentWidget);
                            QObject::connect(emailPopUp, &PopUp::cancelled, [emailPopUp](){emailPopUp->deleteLater();});
                            QObject::connect(emailPopUp, &PopUp::done, [this, emailPopUp, email, password, emailInput, passwordInput](){
                                email->setText("EMAIL");
                                email->setStyleSheet("color: #B9BBBE");
                                password->setText("CURRENT PASSWORD");
                                password->setStyleSheet("color: #B9BBBE");
                                rm->requester->changeEmail([this, emailPopUp, email, password](void *errorsPtr){
                                    if (errorsPtr == nullptr) {
                                        emailPopUp->deleteLater();
                                    } else {
                                        QVector<Api::Error *> errors = *reinterpret_cast<QVector<Api::Error *> *>(errorsPtr);
                                        for (unsigned int i = 0 ; i < errors.size() ; i++) {
                                            if (errors[i]->intCode == 0) {
                                                email->setText("EMAIL - " + errors[i]->message);
                                                email->setStyleSheet("color: #F38688");
                                            } else {
                                                password->setText("CURRENT PASSWORD - " + errors[i]->message);
                                                password->setStyleSheet("color: #F38688");
                                            }
                                        }
                                    }
                                }, emailInput->input->text(), passwordInput->input->text());
                            });
                        } else {
                            code->setText("USERNAME - " + reinterpret_cast<Api::Error *>(errorPtr)->message);
                            code->setStyleSheet("color: #F38688");
                        }
                    }, codeInput->input->text());
                });
            });
        });


        QWidget *phoneEdit = new QWidget(editSection);
        phoneEdit->setFixedSize(596, 40);

        font.setPixelSize(12);
        QLabel *phoneTitle = new QLabel("PHONE NUMBER", phoneEdit);
        phoneTitle->setFont(font);
        phoneTitle->setStyleSheet("color: #B9BBBE");

        if (client.phone.isNull()) {
            font.setPixelSize(14);
            QLabel *phone = new QLabel("You haven't added a phone number yet.", phoneEdit);
            phone->setFont(font);
            phone->setStyleSheet("color: #FFF");
            phone->move(0, 20);

            SettingsButton *phoneEditButton = new SettingsButton(SettingsButton::Type::Edit, "Add", phoneEdit);
            phoneEditButton->move(536, 4);
            QObject::connect(phoneEditButton, &SettingsButton::clicked, [this, client](){
                QWidget *margins = new QWidget();
                QHBoxLayout *marginsLayout = new QHBoxLayout(margins);
                
                QLabel *content = new QLabel(margins);
                content->setFixedHeight(46);
                content->setContentsMargins(4, 4, 4, 4);
                content->setStyleSheet("background-color: #202225;"
                                       "border: 1px solid #000");

                marginsLayout->addWidget(content);
                marginsLayout->setContentsMargins(16, 0, 16, 16);
                
                SettingsInput *phone = new SettingsInput(QString(), "Not yet implemented", true, false, content);
                SettingsButton *send = new SettingsButton(SettingsButton::Type::Normal, "Send", nullptr);
                phone->setStyleSheet("border: none");

                QHBoxLayout *layout = new QHBoxLayout(content);
                layout->setSpacing(0);
                layout->setContentsMargins(0, 0, 0, 0);
                layout->addWidget(phone);
                layout->addWidget(send);

                QWidget *parentWidget = this;
                while (parentWidget->parent()) parentWidget = (QWidget *)parentWidget->parent();
                PopUp *popUp = new PopUp(margins, 472, 300, QString(), "Enter a Phone Number", true, false, "<div style=\"text-align: center\">You will receive a text message with a verification code.<br><br>Your phone number can be used to verify <b>one Discord account</b> at a time and is only used for verification and login.</div>", "", "", false, false, parentWidget->size(), parentWidget);
                QObject::connect(popUp, &PopUp::cancelled, [popUp](){popUp->deleteLater();});
                QObject::connect(send, &SettingsButton::clicked, [popUp, parentWidget](){
                    popUp->deleteLater();
                    // TODO
                });
            });
        } else {
            font.setPixelSize(14);
            QWidget *phoneAndReveal = new QWidget(phoneEdit);
            QHBoxLayout *phoneAndRevealLayout = new QHBoxLayout(phoneAndReveal);
            QLabel *phone = new QLabel(phoneAndReveal);
            phone->setFont(font);
            phone->setStyleSheet("color: #FFF");
            reveal = new Reveal(phone, client.phone, "********" + client.phone.right(4));
            phoneAndRevealLayout->addWidget(phone);
            phoneAndRevealLayout->addWidget(reveal, 0, Qt::AlignTop);
            phoneAndRevealLayout->addStretch(1);
            phoneAndRevealLayout->setSpacing(2);
            phoneAndRevealLayout->setContentsMargins(0, 0, 0, 0);
            phoneAndReveal->move(0, 20);

            SettingsButton *phoneRemoveButton = new SettingsButton(SettingsButton::Type::NoBackground, "Remove", phoneEdit);
            phoneRemoveButton->move(449, 4);
            QObject::connect(phoneRemoveButton, &SettingsButton::clicked, [this, client](){
                QWidget *content = new QWidget();
                QVBoxLayout *layout = new QVBoxLayout(content);
                layout->setSpacing(0);
                layout->setContentsMargins(16, 0, 16, 0);
                QLabel *password = new QLabel("PASSWORD", content);
                QFont font;
                font.setPixelSize(12);
                font.setFamily("whitney");
                password->setFont(font);
                password->setStyleSheet("color: #B9BBBE");
                SettingsInput *input = new SettingsInput(QString(), QString(), false, true, content);
                layout->addWidget(password);
                layout->addWidget(input);

                QWidget *parentWidget = this;
                while (parentWidget->parent()) parentWidget = (QWidget *)parentWidget->parent();
                PopUp *popUp = new PopUp(content, 440, 272, QString(), "Remove phone number", false, false, (client.mfaEnabled ? "<div style=\"background-color: #FAA81A\">Removing your phone number will disable SMS backup for two-factor authentication.</div>" : QString()), "Cancel", "Remove", true, true, parentWidget->size(), parentWidget);
                QObject::connect(popUp, &PopUp::cancelled, [popUp](){popUp->deleteLater();});
                QObject::connect(popUp, &PopUp::done, [this, popUp, password, input](){
                    password->setText("PASSWORD");
                    password->setStyleSheet("color: #B9BBBE");
                    rm->requester->removePhone([popUp, password](void *errorPtr){
                        if (errorPtr == nullptr) {
                            popUp->deleteLater();
                        } else {
                            password->setText("CURRENT PASSWORD - " + reinterpret_cast<Api::Error *>(errorPtr)->message);
                            password->setStyleSheet("color: #F38688");
                        }
                    }, input->input->text());
                    
                });
            });

            SettingsButton *phoneEditButton = new SettingsButton(SettingsButton::Type::Edit, "Add", phoneEdit);
            phoneEditButton->move(536, 4);
            QObject::connect(phoneEditButton, &SettingsButton::clicked, [this, client](){
                QWidget *margins = new QWidget();
                QHBoxLayout *marginsLayout = new QHBoxLayout(margins);
                
                QLabel *content = new QLabel(margins);
                content->setFixedHeight(46);
                content->setContentsMargins(4, 4, 4, 4);
                content->setStyleSheet("background-color: #202225;"
                                       "border: 1px solid #000");

                marginsLayout->addWidget(content);
                marginsLayout->setContentsMargins(16, 0, 16, 16);
                
                SettingsInput *phone = new SettingsInput(QString(), "Not yet implemented", true, false, content);
                SettingsButton *send = new SettingsButton(SettingsButton::Type::Normal, "Send", nullptr);
                phone->setStyleSheet("border: none");

                QHBoxLayout *layout = new QHBoxLayout(content);
                layout->setSpacing(0);
                layout->setContentsMargins(0, 0, 0, 0);
                layout->addWidget(phone);
                layout->addWidget(send);

                QWidget *parentWidget = this;
                while (parentWidget->parent()) parentWidget = (QWidget *)parentWidget->parent();
                PopUp *popUp = new PopUp(margins, 472, 300, QString(), "Enter a Phone Number", true, false, "<div style=\"text-align: center\">You will receive a text message with a verification code.<br><br>Your phone number can be used to verify <b>one Discord account</b> at a time and is only used for verification and login.</div>", "", "", false, false, parentWidget->size(), parentWidget);
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

    QLabel *sectionTitle = new QLabel("Password and Authentication");
    sectionTitle->setFont(font);
    sectionTitle->setStyleSheet("color: #FFF;");

    SettingsButton *changePassword = new SettingsButton(SettingsButton::Type::Normal, "Change Password", container);
        QObject::connect(changePassword, &SettingsButton::clicked, [this](){
            QFont font;
            font.setPixelSize(12);
            font.setFamily("whitney");

            QWidget *content = new QWidget();

            QWidget *currentSection = new QWidget(content);
            QVBoxLayout *currentLayout = new QVBoxLayout(currentSection);
            currentLayout->setSpacing(8);
            currentLayout->setContentsMargins(0, 0, 0, 0);
            QLabel *current = new QLabel("CURRENT PASSWORD", currentSection);
            current->setFont(font);
            current->setStyleSheet("color: #B9BBBE");
            SettingsInput *currentInput = new SettingsInput(QString(), QString(), false, true, currentSection);
            currentLayout->addWidget(current);
            currentLayout->addWidget(currentInput);

            QWidget *newSection = new QWidget(content);
            QVBoxLayout *newLayout = new QVBoxLayout(newSection);
            newLayout->setSpacing(8);
            newLayout->setContentsMargins(0, 0, 0, 0);
            font.setPixelSize(12);
            QLabel *newPassword = new QLabel("NEW PASSWORD", newSection);
            newPassword->setFont(font);
            newPassword->setStyleSheet("color: #B9BBBE");
            SettingsInput *newInput = new SettingsInput(QString(), QString(), false, true, newSection);
            newLayout->addWidget(newPassword);
            newLayout->addWidget(newInput);

            QWidget *confirmSection = new QWidget(content);
            QVBoxLayout *confirmLayout = new QVBoxLayout(confirmSection);
            confirmLayout->setSpacing(8);
            confirmLayout->setContentsMargins(0, 0, 0, 0);
            font.setPixelSize(12);
            QLabel *confirm = new QLabel("CONFIRM NEW PASSWORD", confirmSection);
            confirm->setFont(font);
            confirm->setStyleSheet("color: #B9BBBE");
            SettingsInput *confirmInput = new SettingsInput(QString(), QString(), false, true, confirmSection);
            confirmLayout->addWidget(confirm);
            confirmLayout->addWidget(confirmInput);

            QVBoxLayout *layout = new QVBoxLayout(content);
            layout->setSpacing(16);
            layout->setContentsMargins(0, 0, 0, 0);
            layout->addWidget(currentSection);
            layout->addWidget(newSection);
            layout->addWidget(confirmSection);
            content->setContentsMargins(16, 0, 16, 0);

            QWidget *parentWidget = this;
            while (parentWidget->parent()) parentWidget = (QWidget *)parentWidget->parent();
            PopUp *popUp = new PopUp(content, 440, 416, QString(), "Update your password", true, true, "Enter your current password and a new password.", "Cancel", "Done", true, true, parentWidget->size(), parentWidget);
            QObject::connect(popUp, &PopUp::cancelled, [popUp](){popUp->deleteLater();});
            QObject::connect(popUp, &PopUp::done, [this, popUp, current, currentInput, newPassword, newInput, confirm, confirmInput](){
                current->setText("CURRENT PASSWORD");
                current->setStyleSheet("color: #B9BBBE");
                newPassword->setText("NEW PASSWORD");
                newPassword->setStyleSheet("color: #B9BBBE");
                confirm->setText("CONFIRM NEW  PASSWORD");
                confirm->setStyleSheet("color: #B9BBBE");
                if (newInput->input->text() != confirmInput->input->text()) {
                    confirm->setText("CONFIRM NEW PASSWORD - Passwords do not match!");
                    confirm->setStyleSheet("color: #F38688");
                } else {
                    rm->requester->changePassword([popUp, current, newPassword](void *errorsPtr){
                        if (errorsPtr == nullptr) {
                            popUp->deleteLater();
                        } else {
                            QVector<Api::Error *> errors = *reinterpret_cast<QVector<Api::Error *> *>(errorsPtr);
                            for (unsigned int i = 0 ; i < errors.size() ; i++) {
                                if (errors[i]->intCode == 0) {
                                    current->setText("CURRENT PASSWORD - " + errors[i]->message);
                                    current->setStyleSheet("color: #F38688");
                                } else {
                                    newPassword->setText("NEW PASSWORD - " + errors[i]->message);
                                    newPassword->setStyleSheet("color: #F38688");
                                }
                            }
                        }
                    }, currentInput->input->text(), newInput->input->text());
                }
            });
        });

    
    /* TODO
    QWidget *tfaSection = new QWidget(container);
    QHBoxLayout *tfaLayout = new QHBoxLayout(tfaSection);
    
    QWidget *tfaP = new QWidget(tfaSection);
    QVBoxLayout *tfaPLayout = new QVBoxLayout(tfaP);
    font.setPixelSize(13);
    QLabel *tfaText = new QLabel("Protect your Discord Account with an extra layer of security. "
                                 "Once configured, you'll be required to enter both yout password "
                                 "and an authentication code from your mobile phone in order to sign in.", tfaP);
    tfaText->setWordWrap(true);
    tfaText->setFont(font);
    tfaText->setStyleSheet("color: #B9BBBE;");  
    QLabel *tfaTitle = new QLabel("TWO-FACTOR AUTHENTICATION", tfaP);
    font.setPixelSize(12);
    font.setBold(true);
    tfaTitle->setFont(font);
    tfaTitle->setStyleSheet("color: #B9BBBE;");
    SettingsButton *tfaButton = new SettingsButton(SettingsButton::Type::Normal, "Enable Two-Factor Auth", tfaP);
    tfaPLayout->addWidget(tfaTitle);
    tfaPLayout->addSpacing(16);
    tfaPLayout->addWidget(tfaText);
    tfaPLayout->addSpacing(20);
    tfaPLayout->addWidget(tfaButton);
    tfaPLayout->setContentsMargins(0, 0, 0, 0);
    tfaPLayout->setSpacing(0);

    QLabel *tfaIcon = new QLabel(tfaSection);
    tfaIcon->setPixmap(QPixmap("res/images/svg/lock-icon.svg").scaled(143, 56));

    tfaLayout->addWidget(tfaP);
    tfaLayout->addWidget(tfaIcon);
    tfaLayout->setContentsMargins(0, 0, 0, 0);
    tfaLayout->setSpacing(10);*/

    font.setPixelSize(12);
    font.setBold(true);


    QLabel *accountRemoval = new QLabel("ACCOUNT REMOVAL", container);
    accountRemoval->setFont(font);
    accountRemoval->setStyleSheet("color: #B9BBBE;");

    font.setBold(false);
    font.setPixelSize(13);
    QLabel *accountRemovalDesc = new QLabel("Disabling your account means you can recover "
                                            "it at any time after taking this action");
    accountRemovalDesc->setWordWrap(true);
    accountRemovalDesc->setStyleSheet("color: #B9BBBE;");
    accountRemovalDesc->setFont(font);

    QWidget *accountButtons = new QWidget(container);
    QHBoxLayout *accountButtonsLayout = new QHBoxLayout(accountButtons);
    SettingsButton *disableAccount = new SettingsButton(SettingsButton::Type::Important, "Disable Account", accountButtons);
    SettingsButton *deleteAccount = new SettingsButton(SettingsButton::Type::Critical, "Delete Account", accountButtons);
    QObject::connect(disableAccount, &SettingsButton::clicked, [this](){
        rm->getClient([this](void *clientPtr){
            Api::Client client = *reinterpret_cast<Api::Client *>(clientPtr);
            rm->getGuilds([this, client](void *guildsPtr){
                QVector<Api::Guild *> guilds = *reinterpret_cast<QVector<Api::Guild *> *>(guildsPtr);
                bool owner = false;
                for (unsigned int i = 0 ; i < guilds.size() ; i++) {
                    if (guilds[i]->ownerId == client.id) {
                        owner = true;
                        break;
                    }
                }
                QWidget *parentWidget = this;
                while (parentWidget->parent()) parentWidget = (QWidget *)parentWidget->parent();
                if (owner) {
                    PopUp *popUp = new PopUp(new QWidget(), 440, 220, QString(), "You Own Servers", false, false, "In order to delete or disable your account you must first transfer ownership of all servers that you own.", "", "Okay", true, true, parentWidget->size(), parentWidget);
                    QObject::connect(popUp, &PopUp::cancelled, [popUp](){popUp->deleteLater();});
                    QObject::connect(popUp, &PopUp::done, [popUp](){popUp->deleteLater();});
                } else {
                    QWidget *content = new QWidget();
                    content->setContentsMargins(16, 0, 16, 0);
                    QVBoxLayout *layout = new QVBoxLayout(content);
                    layout->setSpacing(8);
                    layout->setContentsMargins(0, 0, 0, 0);
                    QLabel *password = new QLabel("PASSWORD", content);
                    QFont font;
                    font.setPixelSize(12);
                    font.setFamily("whitney");
                    password->setFont(font);
                    password->setStyleSheet("color: #B9BBBE");
                    SettingsInput *input = new SettingsInput(QString(), QString(), false, true, content);
                    layout->addWidget(password);
                    layout->addWidget(input);

                    PopUp *popUp = new PopUp(content, 440, 312, QString(), "Delete Account", false, false, "<div style=\"background-color: #FAA81A\">Are you sure that you want to disable your account? This will immediatly log you out of your account and make your account inaccessible to anyone.", "Cancel", "Disable Account", true, true, parentWidget->size(), parentWidget);
                    QObject::connect(popUp, &PopUp::cancelled, [this, popUp](){popUp->deleteLater();});
                    QObject::connect(popUp, &PopUp::done, [this, popUp, parentWidget, password, input](){
                        password->setText("PASSWORD");
                        password->setStyleSheet("color: #B9BBBE");
                        rm->requester->disableAccount([popUp, parentWidget, password](void *errorPtr){
                            if (errorPtr == nullptr) {
                                popUp->deleteLater();

                                PopUp *closePopUp = new PopUp(new QWidget(), 440, 200, QString(), QString(), true, false, "Your account has been disabled successfully, Fast-Discord will now exit.", QString(), QString(), false, false, parentWidget->size(), parentWidget);
                                QObject::connect(closePopUp, &PopUp::cancelled, [](){exit(0);});
                            } else {
                                password->setText("PASSWORD - " + reinterpret_cast<Api::Error *>(errorPtr)->message);
                                password->setStyleSheet("color: #F38688");
                            }
                        }, input->input->text());
                    });
                }
            });
        });
        
    });
    QObject::connect(deleteAccount, &SettingsButton::clicked, [this](){
        rm->getClient([this](void *clientPtr){
            Api::Client client = *reinterpret_cast<Api::Client *>(clientPtr);
            rm->getGuilds([this, client](void *guildsPtr){
                QVector<Api::Guild *> guilds = *reinterpret_cast<QVector<Api::Guild *> *>(guildsPtr);
                bool owner = false;
                for (unsigned int i = 0 ; i < guilds.size() ; i++) {
                    if (guilds[i]->ownerId == client.id) {
                        owner = true;
                        break;
                    }
                }
                QWidget *parentWidget = this;
                while (parentWidget->parent()) parentWidget = (QWidget *)parentWidget->parent();
                if (owner) {
                    PopUp *popUp = new PopUp(new QWidget(), 440, 220, QString(), "You Own Servers", false, false, "In order to delete or disable your account you must first transfer ownership of all servers that you own.", "", "Okay", true, true, parentWidget->size(), parentWidget);
                    QObject::connect(popUp, &PopUp::cancelled, [popUp](){popUp->deleteLater();});
                    QObject::connect(popUp, &PopUp::done, [popUp](){popUp->deleteLater();});
                } else {
                    QWidget *content = new QWidget();
                    content->setContentsMargins(16, 0, 16, 0);
                    QVBoxLayout *layout = new QVBoxLayout(content);
                    layout->setSpacing(8);
                    layout->setContentsMargins(0, 0, 0, 0);
                    QLabel *password = new QLabel("PASSWORD", content);
                    QFont font;
                    font.setPixelSize(12);
                    font.setFamily("whitney");
                    password->setFont(font);
                    password->setStyleSheet("color: #B9BBBE");
                    SettingsInput *input = new SettingsInput(QString(), QString(), false, true, content);
                    layout->addWidget(password);
                    layout->addWidget(input);

                    PopUp *popUp = new PopUp(content, 440, 312, QString(), "Disable Account", false, false, "<div style=\"background-color: #FAA81A\">Are you sure that you want to delete your account? This will immediatly log you out of your account and you will not be able to log in again.", "Cancel", "Delete Account", true, true, parentWidget->size(), parentWidget);
                    QObject::connect(popUp, &PopUp::cancelled, [popUp](){popUp->deleteLater();});
                    QObject::connect(popUp, &PopUp::done, [this, popUp, parentWidget, password, input](){
                        password->setText("CURRENT PASSWORD");
                        password->setStyleSheet("color: #B9BBBE");
                        rm->requester->deleteAccount([popUp, parentWidget, password](void *errorPtr){
                            if (errorPtr == nullptr) {
                                popUp->deleteLater();

                                PopUp *closePopUp = new PopUp(new QWidget(), 440, 200, QString(), QString(), true, false, "Your account has been deleted successfully, Fast-Discord will now exit.", QString(), QString(), false, false, parentWidget->size(), parentWidget);
                                QObject::connect(closePopUp, &PopUp::cancelled, [](){exit(0);});
                            } else {
                                password->setText("PASSWORD - " + reinterpret_cast<Api::Error *>(errorPtr)->message);
                                password->setStyleSheet("color: #F38688");
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
    accountButtonsLayout->setSpacing(16);
    accountButtonsLayout->setContentsMargins(0, 0, 0, 0);

    layout->addWidget(title);
    layout->addSpacing(20);
    layout->addWidget(profile);
    layout->addWidget(new Divider());
    layout->addWidget(sectionTitle);
    layout->addSpacing(20);
    layout->addWidget(changePassword);
    /*layout->addSpacing(28);
    layout->addWidget(tfaSection);*/
    layout->addWidget(new Divider());
    layout->addWidget(accountRemoval);
    layout->addSpacing(8);
    layout->addWidget(accountRemovalDesc);
    layout->addSpacing(16);
    layout->addWidget(accountButtons);

    this->setWidget(container);
    this->setStyleSheet("QScrollBar::handle {border: none; border-radius: 2px; background-color: #202225;}"
                        "QScrollBar {border: none; background-color: #36393F; border-radius: 8px; width: 3px;}"
                        "QScrollBar::add-line, QScrollBar::sub-line {border:none; background: none; height: 0;}");
}

} // namespace Ui