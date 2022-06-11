#include "ui/settings/userprofile.h"

#include "ui/settings/divider.h"
#include "ui/settings/settingsbutton.h"
#include "ui/settings/popup.h"
#include "ui/settings/selectimagebutton.h"

#include <QTextEdit>
#include <QColorDialog>
#include <QFileDialog>
#include <QStandardPaths>

namespace Ui {

UserProfile::UserProfile(Api::RessourceManager *rmp, QWidget *parent)
    : QScrollArea()
{
    rm = rmp;
    modified = false;

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    QWidget *container = new QWidget(this);
    layout = new QVBoxLayout(container);
    layout->setContentsMargins(0, 0, 0, 0);
    container->setMaximumWidth(752);
    container->setContentsMargins(40, 60, 40, 80);
    container->setStyleSheet("background-color: #36393F");

    QFont font;
    font.setPixelSize(20);
    font.setFamily("whitney");

    QLabel *title = new QLabel("User Profile", container);
    title->setFont(font);
    title->setStyleSheet("color: #FFF;");

    layout->addWidget(title);
    layout->addWidget(new Divider());

    QWidget *profileEdit = new QWidget(container);
    QHBoxLayout *profileLayout = new QHBoxLayout(profileEdit);
    profileLayout->setSpacing(20);
    profileLayout->setContentsMargins(0, 0, 0, 0);

    font.setPixelSize(12);
    QWidget *customization = new QWidget(profileEdit);
    QVBoxLayout *customizationLayout = new QVBoxLayout(customization);
    customizationLayout->setSpacing(0);
    customizationLayout->setContentsMargins(0, 0, 0, 0);


    QWidget *avatarSection = new QWidget(customization);
    QVBoxLayout *avatarLayout = new QVBoxLayout(avatarSection);
    avatarLayout->setSpacing(8);
    avatarLayout->setContentsMargins(0, 0, 0, 0);

    QLabel *avatarTitle = new QLabel("AVATAR", avatarSection);
    avatarTitle->setFont(font);
    avatarTitle->setStyleSheet("color: #B9BBBE");
    
    QWidget *avatarButtons = new QWidget(avatarSection);
    QHBoxLayout *avatarButtonsLayout = new QHBoxLayout(avatarButtons);
    avatarButtonsLayout->setSpacing(4);
    avatarButtonsLayout->setContentsMargins(0, 0, 0, 0);
    SettingsButton *changeAvatar = new SettingsButton(SettingsButton::Type::Normal, "Change Avatar", avatarButtons);
    SettingsButton *removeAvatar = new SettingsButton(SettingsButton::Type::NoBackground, "Remove Avatar", avatarButtons);
    avatarButtonsLayout->addWidget(changeAvatar);
    avatarButtonsLayout->addWidget(removeAvatar);
    avatarButtonsLayout->addStretch(1);

    avatarLayout->addWidget(avatarTitle);
    avatarLayout->addWidget(avatarButtons);


    QWidget *colorSection = new QWidget(customization);
    QVBoxLayout *colorLayout = new QVBoxLayout(colorSection);
    colorLayout->setSpacing(16);
    colorLayout->setContentsMargins(0, 0, 0, 0);

    QLabel *colorTitle = new QLabel("PROFILE COLOR", colorSection);
    colorTitle->setFont(font);
    colorTitle->setStyleSheet("color: #B9BBBE");

    QWidget *colorButtons = new QWidget(colorSection);
    QHBoxLayout *colorButtonsLayout = new QHBoxLayout(colorButtons);
    colorButtonsLayout->setSpacing(16);
    colorButtonsLayout->setContentsMargins(0, 0, 0, 0);

    font.setPixelSize(11);
    QWidget *defaultColorContainer = new QWidget(colorButtons);
    defaultColorContainer->setFixedSize(69, 70);
    QVBoxLayout *defaultColorLayout = new QVBoxLayout(defaultColorContainer);
    defaultColorLayout->setSpacing(4);
    defaultColorLayout->setContentsMargins(0, 0, 0, 0);
    QLabel *defaultColorTitle = new QLabel("Default", defaultColorContainer);
    defaultColorTitle->setFont(font);
    defaultColorTitle->setStyleSheet("color: #B9BBBE");
    defaultColor = new ColorLabel(defaultColorContainer);
    defaultColorLayout->addWidget(defaultColor);
    defaultColorLayout->addWidget(defaultColorTitle, 1, Qt::AlignHCenter);

    QWidget *customColorContainer = new QWidget(colorButtons);
    customColorContainer->setFixedSize(69, 70);
    QVBoxLayout *customColorLayout = new QVBoxLayout(customColorContainer);
    customColorLayout->setSpacing(4);
    customColorLayout->setContentsMargins(0, 0, 0, 0);
    QLabel *customColorTitle = new QLabel("Custom", customColorContainer);
    customColorTitle->setFont(font);
    customColorTitle->setStyleSheet("color: #B9BBBE");
    customColor = new ColorLabel(customColorContainer);
    QLabel *colorIcon = new QLabel(customColor);
    colorIcon->setPixmap(QPixmap("res/images/svg/grab-color-icon.svg").scaled(14, 14));
    colorIcon->move(50, 0);
    customColorLayout->addWidget(customColor);
    customColorLayout->addWidget(customColorTitle, 0, Qt::AlignHCenter);
    
    rm->getClient([this](void *clientPtr){
        Api::Client *client = reinterpret_cast<Api::Client *>(clientPtr);

        bannerColor = "#" + QString::number(client->bannerColor, 16);
        avatarFile = client->avatar;
        bannerFile = client->banner;
        bio = client->bio;

        if (client->bannerColor != 0) {
            customColor->setClicked();
            customColor->setColor("#" + QString::number(client->bannerColor, 16));
        } else {
            defaultColor->setClicked();
            customColor->setColor("#000");
        }
        
        if (client->avatar.isNull()) {
            defaultColor->setColor("#000");
        } else {
            QString channelIconFileName = client->id + (client->avatar.indexOf("a_") == 0 ? ".gif" : ".png");
            rm->getImage([this](void *imageFileName){
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
                defaultBannerColor = QColor(r, g, b);
                defaultColor->setColor(defaultBannerColor.name());
            }, "https://cdn.discordapp.com/avatars/" + client->id + "/" + client->avatar, channelIconFileName);
        }
    });

    colorButtonsLayout->addWidget(defaultColorContainer);
    colorButtonsLayout->addWidget(customColorContainer);
    colorButtonsLayout->addStretch(1);

    colorLayout->addWidget(colorTitle);
    colorLayout->addWidget(colorButtons);
    colorLayout->addStretch(1);


    QWidget *bannerSection = new QWidget(customization);
    QVBoxLayout *bannerLayout = new QVBoxLayout(bannerSection);
    bannerLayout->setSpacing(8);
    bannerLayout->setContentsMargins(0, 0, 0, 0);

    font.setPixelSize(12);
    QWidget *bannerTitle = new QWidget(bannerSection);
    QHBoxLayout *bannerTitleLayout = new QHBoxLayout(bannerTitle);
    bannerTitleLayout->setSpacing(0);
    bannerTitleLayout->setContentsMargins(0, 0, 0, 0);
    QLabel *bannerTitleText = new QLabel("PROFILE BANNER", bannerTitle);
    bannerTitleText->setFont(font);
    bannerTitleText->setStyleSheet("color: #B9BBBE");
    QLabel *bannerTitleIcon = new QLabel(bannerTitle);
    bannerTitleIcon->setPixmap(QPixmap("res/images/svg/nitro-icon.svg").scaled(16, 16));
    bannerTitleLayout->addWidget(bannerTitleText);
    bannerTitleLayout->addWidget(bannerTitleIcon);
    bannerTitleLayout->addStretch(1);

    QLabel *bannerDescription = new QLabel("We recommend an image of at least 600x240. You can\nupload a PNG, JPG, or an animated GIF under 10 MB.", bannerSection);
    bannerDescription->setFont(font);
    bannerDescription->setStyleSheet("color: #B9BBBE");

    QWidget *bannerButtons = new QWidget(bannerSection);
    QHBoxLayout *bannerButtonsLayout = new QHBoxLayout(bannerButtons);
    bannerButtonsLayout->setSpacing(4);
    bannerButtonsLayout->setContentsMargins(0, 0, 0, 0);
    rm->getClient([bannerButtons, bannerButtonsLayout, this](void *clientPtr){
        Api::Client *client = reinterpret_cast<Api::Client *>(clientPtr);
        if (client->purchasedFlags) {
            SettingsButton *changeBanner = new SettingsButton(SettingsButton::Type::Normal, "Change Banner", bannerButtons);
            bannerButtonsLayout->addWidget(changeBanner);

            if (!client->banner.isNull()) {
                SettingsButton *removeBanner = new SettingsButton(SettingsButton::Type::NoBackground, "Remove Banner", bannerButtons);
                bannerButtonsLayout->addWidget(removeBanner);
                QObject::connect(removeBanner, &SettingsButton::clicked, [this](){
                    profile->setBanner(QString(), "#000");
                    bannerFile.clear();
                    profileChanged();
                });
            }
            QObject::connect(changeBanner, &SettingsButton::clicked, [this](){
                QWidget *parentWidget = this;
                while (parentWidget->parent()) parentWidget = (QWidget *)parentWidget->parent();

                QFont font;
                font.setPixelSize(12);
                font.setFamily("whitney");
                QWidget *container = new QWidget();
                container->setFixedSize(200, 212);
                QHBoxLayout *mainLayout = new QHBoxLayout(container);
                mainLayout->setContentsMargins(16, 0, 16, 16);
                QLabel *content = new QLabel(container);
                content->setFixedSize(160, 196);
                content->setStyleSheet("background-color: #292B2F");
                QVBoxLayout *layout = new QVBoxLayout(content);
                layout->setSpacing(32);
                layout->setContentsMargins(16, 16, 16, 16);
                SelectImageButton *button = new SelectImageButton(content);
                QLabel *desc = new QLabel("Upload Image", content);
                desc->setFont(font);
                layout->addWidget(button);
                layout->addWidget(desc, 0, Qt::AlignHCenter);
                mainLayout->addWidget(content, 0, Qt::AlignCenter);

                PopUp *popUp = new PopUp(container, 240, 280, QString(), "Select Image", false, true, QString(), QString(), QString(), false, false, parentWidget->size(), parentWidget);
                QObject::connect(popUp, &PopUp::cancelled, [popUp](){popUp->deleteLater();});
                QObject::connect(button, &SelectImageButton::clicked, [this, parentWidget, popUp](){
                    QString defaultFolder = QStandardPaths::writableLocation(QStandardPaths::PicturesLocation) + "/";
                    if (defaultFolder.isEmpty()) {
                        defaultFolder = ".";
                    }
                    QString imagePath = QFileDialog::getOpenFileName(nullptr, "Select Image", defaultFolder);
                    popUp->deleteLater();
                    if (!imagePath.isNull()) {
                        profile->setBanner(imagePath, QString());
                        bannerFile = imagePath;
                        profileChanged();
                    }
                });
            });
            
            bannerButtonsLayout->addStretch(1);
        } else {
            SettingsButton *getNitroButton = new SettingsButton(SettingsButton::Type::Normal, "Get Nitro", bannerButtons);
            QObject::connect(getNitroButton, &SettingsButton::clicked, [this](){
                QWidget *parentWidget = this;
                while (parentWidget->parent()) parentWidget = (QWidget *)parentWidget->parent();
                PopUp *popUp = new PopUp(new QWidget(), 440, 120, QString(), "Get Nitro", true, false, "<div style=\"text-align: center\">You can get nitro at <a style=\"color: #00AFF4; text-decoration: none;\" href=\"https://discord.com/nitro\">https://discord.com/nitro</a></div>", QString(), QString(), false, false, parentWidget->size(), parentWidget);
                QObject::connect(popUp, &PopUp::cancelled, [popUp](){popUp->deleteLater();});
            });
            bannerButtonsLayout->addWidget(getNitroButton);
            bannerButtonsLayout->addStretch(1);
        }
    });

    bannerLayout->addWidget(bannerTitle);
    bannerLayout->addWidget(bannerDescription);
    bannerLayout->addSpacing(4);
    bannerLayout->addWidget(bannerButtons);


    QWidget *aboutSection = new QWidget(customization);
    QVBoxLayout *aboutLayout = new QVBoxLayout(aboutSection);
    aboutLayout->setSpacing(8);
    aboutLayout->setContentsMargins(0, 0, 0, 0);

    QLabel *aboutTitle = new QLabel("ABOUT ME", aboutSection);
    aboutTitle->setFont(font);
    aboutTitle->setStyleSheet("color: #B9BBBE");

    QLabel *aboutDescription = new QLabel("You can use markdown and links if you'd like.", aboutSection);
    aboutDescription->setFont(font);
    aboutDescription->setStyleSheet("color: #B9BBBE");

    font.setPixelSize(14);
    QLabel *aboutEdit = new QLabel(aboutSection);
    aboutEdit->setFixedSize(340, 136);
    aboutEdit->setStyleSheet("background-color: #202225;"
                             "border-radius: 3px");
    QHBoxLayout *aboutEditLayout = new QHBoxLayout(aboutEdit);
    aboutEditLayout->setSpacing(10);
    aboutEditLayout->setContentsMargins(16, 11, 16, 11);
    aboutTextEdit = new QTextEdit(aboutEdit);
    aboutTextEdit->setFont(font);
    aboutTextEdit->setStyleSheet("color: #DCDDDE");
    QLabel *charCount = new QLabel(aboutEdit);
    rm->getClient([charCount, this](void *clientPtr){
        aboutTextEdit->setText(reinterpret_cast<Api::Client *>(clientPtr)->bio);
        charCount->setText(QString::number(190 - reinterpret_cast<Api::Client *>(clientPtr)->bio.length()));
    });
    charCount->setFont(font);
    charCount->setStyleSheet("color: #DCDDDE");
    QObject::connect(aboutTextEdit, &QTextEdit::textChanged, [charCount, this](){
        QString text = aboutTextEdit->toPlainText();
        if (text.length() > 190) {
            QTextCursor endCursor = aboutTextEdit->textCursor();
            int position = endCursor.position();
            aboutTextEdit->setPlainText(text.mid(0, 190));
            endCursor.setPosition((position == 191 ? position-1 : position));
            aboutTextEdit->setTextCursor(endCursor);
            bio = text.mid(0, 190);
            profileChanged();
        } else {
            charCount->setText(QString::number(190 - text.length()));
            profile->setBio(text);
            bio = text;
            profileChanged();
        }
    });
    aboutEditLayout->addWidget(aboutTextEdit);
    aboutEditLayout->addWidget(charCount, 0, Qt::AlignBottom);

    aboutLayout->addWidget(aboutTitle);
    aboutLayout->addWidget(aboutDescription);
    aboutLayout->addWidget(aboutEdit);


    QWidget *line1 = new QWidget(this);
    line1->setFixedSize(660, 1);
    line1->setStyleSheet("background-color: rgba(79, 84, 92, 0.48);");
    QWidget *line2 = new QWidget(this);
    line2->setFixedSize(660, 1);
    line2->setStyleSheet("background-color: rgba(79, 84, 92, 0.48);");

    customizationLayout->addWidget(avatarSection);
    customizationLayout->addSpacing(24);
    customizationLayout->addWidget(line1);
    customizationLayout->addSpacing(24);
    customizationLayout->addWidget(colorSection);
    customizationLayout->addSpacing(24);
    customizationLayout->addWidget(bannerSection);
    customizationLayout->addSpacing(24);
    customizationLayout->addWidget(line2);
    customizationLayout->addSpacing(24);
    customizationLayout->addWidget(aboutSection);
    customizationLayout->addStretch(1);


    QWidget *preview = new QWidget(profileEdit);
    QVBoxLayout *previewLayout = new QVBoxLayout(preview);
    previewLayout->setSpacing(8);
    previewLayout->setContentsMargins(0, 0, 0, 0);

    font.setPixelSize(12);
    QLabel *previewTitle = new QLabel("PREVIEW", preview);
    previewTitle->setFont(font);
    previewTitle->setStyleSheet("color: #B9BBBE");
    previewLayout->addWidget(previewTitle);

    rm->getClient([rmp, preview, previewLayout, this](void *clientPtr){
        profile = new Profile(rmp, *reinterpret_cast<Api::Client *>(clientPtr), preview);
        QObject::connect(profile, &Profile::bannerChanged, [this](QString banner){
            bannerFile = banner;
            profileChanged();
        });
        QObject::connect(profile, &Profile::avatarChanged, [this](QString avatar){
            avatarFile = avatar;
            profileChanged();
        });
        previewLayout->addWidget(profile);
    });

    profileLayout->addWidget(customization);
    profileLayout->addWidget(preview);

    layout->addSpacing(20);
    layout->addWidget(profileEdit);

    mainLayout->addWidget(container, 10);

    this->setWidget(container);
    this->setStyleSheet("QScrollBar::handle {border: none; border-radius: 2px; background-color: #202225;}"
                        "QScrollBar {border: none; background-color: #36393F; border-radius: 8px; width: 3px;}"
                        "QScrollBar::add-line, QScrollBar::sub-line {border:none; background: none; height: 0;}");


    QObject::connect(defaultColor, &ColorLabel::clicked, [this](){
        customColor->setUnclicked();
        bannerColor = defaultColor->color;
        profile->setBanner(QString(), defaultColor->color);
        profileChanged();
    });
    QObject::connect(customColor, &ColorLabel::clicked, [this](){
        defaultColor->setUnclicked();
        if (customColor->pressed) {
            QString color = QColorDialog::getColor().name();
            bannerColor = color;
            customColor->setColor(color);
            profile->setBanner(QString(), color);
            profileChanged();
        } else {
            bannerColor = customColor->color;
            profile->setBanner(QString(), customColor->color);
            profileChanged();
        }
    });
    QObject::connect(removeAvatar, &SettingsButton::clicked, [this](){
        defaultColor->setColor("#000");
        profile->setAvatar(QString(), "#000");
        avatarFile.clear();
        profileChanged();
    });
    QObject::connect(changeAvatar, &SettingsButton::clicked, [this](){
        QWidget *parentWidget = this;
        while (parentWidget->parent()) parentWidget = (QWidget *)parentWidget->parent();

        QFont font;
        font.setPixelSize(12);
        font.setFamily("whitney");
        QWidget *container = new QWidget();
        container->setFixedSize(200, 212);
        QHBoxLayout *mainLayout = new QHBoxLayout(container);
        mainLayout->setContentsMargins(16, 0, 16, 16);
        QLabel *content = new QLabel(container);
        content->setFixedSize(160, 196);
        content->setStyleSheet("background-color: #292B2F");
        QVBoxLayout *layout = new QVBoxLayout(content);
        layout->setSpacing(32);
        layout->setContentsMargins(16, 16, 16, 16);
        SelectImageButton *button = new SelectImageButton(content);
        QLabel *desc = new QLabel("Upload Image", content);
        desc->setFont(font);
        layout->addWidget(button);
        layout->addWidget(desc, 0, Qt::AlignHCenter);
        mainLayout->addWidget(content, 0, Qt::AlignCenter);

        PopUp *popUp = new PopUp(container, 240, 280, QString(), "Select Image", false, true, QString(), QString(), QString(), false, false, parentWidget->size(), parentWidget);
        QObject::connect(popUp, &PopUp::cancelled, [popUp](){popUp->deleteLater();});
        QObject::connect(button, &SelectImageButton::clicked, [this, parentWidget, popUp](){
            QString defaultFolder = QStandardPaths::writableLocation(QStandardPaths::PicturesLocation) + "/";
            if (defaultFolder.isEmpty()) {
                defaultFolder = ".";
            }
            QString imagePath = QFileDialog::getOpenFileName(nullptr, "Select Image", defaultFolder);
            popUp->deleteLater();
            if (!imagePath.isNull()) {
                avatarFile = imagePath;
                profile->setAvatar(imagePath, QString());
                profileChanged();
            }
        });
    });
}

void UserProfile::profileChanged()
{
    if (!modified) {
        modified = true;

        QFont font;
        font.setPixelSize(13);
        font.setFamily("whitney");

        QLabel *unsaved = new QLabel(this);
        unsaved->setFixedHeight(32);
        unsaved->setMaximumWidth(700);
        unsaved->setStyleSheet("background-color: rgba(32, 34, 37, 0.9);"
                               "border-radius: 5px");
        QHBoxLayout *layout = new QHBoxLayout(unsaved);
        layout->setSpacing(10);
        layout->setContentsMargins(16, 10, 10, 10);
        QLabel *careful = new QLabel("Careful - you have unsaved changes!", unsaved);
        careful->setFont(font);
        careful->setStyleSheet("color: #fff");
        SettingsButton *reset = new SettingsButton(SettingsButton::Type::NoBackground, "Reset", unsaved);
        SettingsButton *save = new SettingsButton(SettingsButton::Type::Normal, "Save Changes", unsaved);
        layout->addWidget(careful);
        layout->addWidget(reset);
        layout->addWidget(save);

        this->layout->addWidget(unsaved);

        QObject::connect(reset, &SettingsButton::clicked, [this, unsaved](){
            rm->getClient([this, unsaved](void *clientPtr){
                Api::Client client = *reinterpret_cast<Api::Client *>(clientPtr);

                if (client.bannerColor != 0) {
                    customColor->setClicked();
                    customColor->setColor("#" + QString::number(client.bannerColor, 16));
                } else {
                    defaultColor->setClicked();
                    customColor->setColor("#000");
                }
                
                if (client.avatar.isNull()) {
                    defaultColor->setColor("#000");
                } else {
                    defaultColor->setColor(defaultBannerColor.name());
                }

                aboutTextEdit->setPlainText(client.bio);

                profile->setAvatar(client.banner, (client.banner != 0 ? QString::number(client.bannerColor, 16) : defaultBannerColor.name()));
                profile->setAvatar((client.avatar.isNull() ? "res/images/png/user-icon-asset0.png" : client.avatar + (client.avatar.indexOf("a_") == 0 ? ".gif" : ".png")), QString());

                this->modified = false;
                unsaved->deleteLater();
            });
        });

        QObject::connect(save, &SettingsButton::clicked, [this, unsaved](){
            rm->getClient([this, unsaved](void *clientPtr){
                Api::Client client = *reinterpret_cast<Api::Client *>(clientPtr);

                bool modified = false;
                QString data = "{";

                if ("#" + QString::number(client.bannerColor, 16) != bannerColor) {
                    data += "\"accent_color\":" + bannerColor.mid(1, 6).toLong(nullptr, 16);
                    modified = true;
                }

                if (client.bio != bio) {
                    data += (modified ? "," : "") + QString("\"bio\":\"") + bio + "\"";
                    modified = true;
                }

                if (client.banner != bannerFile) {
                    QFile image(avatarFile);
                    image.open(QIODevice::OpenModeFlag::ReadOnly);
                    QMimeDatabase db;
                    data += (modified ? "," : "") + QString("\"banner\":\"") + "data:" + db.mimeTypeForFile(avatarFile).name() + ";base64,"
                        + image.readAll().toBase64() + "\"";
                    modified = true;
                }

                if (client.avatar != avatarFile) {
                    QFile image(avatarFile);
                    image.open(QIODevice::OpenModeFlag::ReadOnly);
                    QMimeDatabase db;
                    data += (modified ? "," : "") + QString("\"avatar\":\"") + "data:" + db.mimeTypeForFile(avatarFile).name() + ";base64,"
                        + image.readAll().toBase64() + "\"";
                }

                data += "}";

                rm->requester->changeClient([this, unsaved](void *errorsPtr){
                    if (errorsPtr == nullptr) {
                        this->modified = false;
                        unsaved->deleteLater();
                    } else {
                        QVector<Api::Error *> errors = *reinterpret_cast<QVector<Api::Error *> *>(errorsPtr);

                        QWidget *container = new QWidget();
                        QHBoxLayout *layout = new QHBoxLayout(container);
                        layout->setContentsMargins(32, 32, 32, 32);
                        layout->setSpacing(10);
                        
                        QFont font;
                        font.setPixelSize(14);
                        font.setBold(true);
                        font.setFamily("whitney");

                        for (unsigned int i = 0 ; i < errors.size() ; i++) {
                            QLabel *label = new QLabel(errors[i]->message, container);
                            label->setFont(font);
                            label->setStyleSheet("color: #B9BBBE");
                            layout->addWidget(label);
                        }
                        layout->addStretch(1);

                        QWidget *parentWidget = this;
                        while (parentWidget->parent()) parentWidget = (QWidget *)parentWidget->parent();

                        PopUp *popUp = new PopUp(container, 400, 300, QString(), "Errors", false, true, QString(), QString(), QString(), false, false, parentWidget->size(), parentWidget);
                        QObject::connect(popUp, &PopUp::cancelled, [popUp](){popUp->deleteLater();});
                    }
                }, data);
            });
        });
    }
}

} // namespace Ui