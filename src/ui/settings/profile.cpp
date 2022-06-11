#include "ui/settings/profile.h"

#include "ui/settings/divider.h"
#include "ui/settings/popup.h"
#include "ui/settings/selectimagebutton.h"

#include <QVBoxLayout>
#include <QPixmap>
#include <QFileDialog>

namespace Ui {

class ImageHoverWidget : public QLabel
{
    Q_OBJECT
public: 
    ImageHoverWidget(QString text, QWidget *parent)
        : QLabel(parent)
    {
        this->setAttribute(Qt::WA_StyledBackground);
        this->setStyleSheet("background-color: none");

        QVBoxLayout *layout = new QVBoxLayout(parent);
        layout->setContentsMargins(0, 0, 0, 0);
        QVBoxLayout *thisLayout = new QVBoxLayout(this);
        thisLayout->setContentsMargins(0, 0, 0, 0);
        textLabel = new QLabel(text, this);
        textLabel->hide();
        QFont font;
        font.setPixelSize(11);
        font.setFamily("whitney");
        font.setBold(true);
        textLabel->setFont(font);
        textLabel->setStyleSheet("color: white;"
                                 "background-color: none");
        thisLayout->addWidget(textLabel, 0, Qt::AlignCenter);
        layout->addWidget(this);
    }

signals:
    void clicked();

private:
    QLabel *textLabel;
    void mouseReleaseEvent(QMouseEvent *event) override
    {
        if (event->button() == Qt::LeftButton)
            emit clicked();
    }
    void enterEvent(QEvent *) override
    {
        this->setStyleSheet("background-color: rgba(0, 0, 0, 0.4)");
        textLabel->setStyleSheet("background-color: none");
        textLabel->show();
    }
    void leaveEvent(QEvent *) override
    {
        this->setStyleSheet("background-color: none");
        textLabel->setStyleSheet("background-color: none");
        textLabel->hide();
    }
};

Profile::Profile(Api::RessourceManager *rm, Api::Client client, QWidget *parent)
{
    QWidget *container = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(container);

    QFont font;
    font.setPixelSize(20);
    font.setFamily("whitney");
    font.setBold(true);


    banner = new QLabel(container);
    banner->setFixedWidth(300);
    if (!client.banner.isNull()) {
        banner->setFixedHeight(120);
        rm->getImage([this](void *imageFileName){
            banner->setPixmap(QPixmap(*reinterpret_cast<QString *>(imageFileName)).scaledToWidth(300));
        }, "https://cdn.discordapp.com/banners/" + client.id + "/" + client.banner + ".png?size=600", client.banner);
    } else if (client.bannerColor != 0) {
        banner->setFixedHeight(60);
        banner->setStyleSheet("background-color: #" + QString::number(client.bannerColor, 16));
    } else {
        if (client.avatar.isNull()) {
            banner->setStyleSheet("background-color: #000");
        } else {
            QString channelIconFileName = client.id + (client.avatar.indexOf("a_") == 0 ? ".gif" : ".png");
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
                banner->setStyleSheet("background-color: #" + (r < 16 ? "0" + QString::number(r, 16) : QString::number(r, 16)) + (g < 16 ? "0" + QString::number(g, 16) : QString::number(g, 16)) + (b < 16 ? "0" + QString::number(b, 16) : QString::number(b, 16)) + ";");
            }, "https://cdn.discordapp.com/avatars/" + client.id + "/" + client.avatar, channelIconFileName);
        }
        banner->setFixedHeight(60);
    }

    ImageHoverWidget *bannerHover = new ImageHoverWidget((client.purchasedFlags ? "CHANGE BANNER" : "UNLOCK BANNER"), banner);
    if (client.purchasedFlags) {
        QObject::connect(bannerHover, &ImageHoverWidget::clicked, [this](){
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
                    this->setBanner(imagePath, QString());
                emit bannerChanged(imagePath);
                }
            });
        });
    } else {
        QObject::connect(bannerHover, &ImageHoverWidget::clicked, [this](){
            QWidget *parentWidget = this;
            while (parentWidget->parent()) parentWidget = (QWidget *)parentWidget->parent();
            PopUp *popUp = new PopUp(new QWidget(), 440, 120, QString(), "Get Nitro", true, false, "<div style=\"text-align: center\">You can get nitro at <a style=\"color: #00AFF4; text-decoration: none;\" href=\"https://discord.com/nitro\">https://discord.com/nitro</a></div>", QString(), QString(), false, false, parentWidget->size(), parentWidget);
            QObject::connect(popUp, &PopUp::cancelled, [popUp](){popUp->deleteLater();});
        });
    }

    QWidget *content = new QWidget(container);
    content->move(0, client.banner.isNull() ? 124 : 184);
    layout = new QVBoxLayout(content);
    layout->setSpacing(16);
    layout->setContentsMargins(16, 0, 16, 0);

    QWidget *username = new QWidget(content);
    username->setFixedHeight(32);
    QHBoxLayout *usernameLayout = new QHBoxLayout(username);
    usernameLayout->setSpacing(0);
    usernameLayout->setContentsMargins(0, 0, 16, 0);

    QLabel *name = new QLabel(client.username, username);
    name->setFont(font);
    name->setStyleSheet("color: #FFF");

    QLabel *discriminator = new QLabel("#" + client.discriminator, username);
    discriminator->setFont(font);
    discriminator->setStyleSheet("color: #B9BBBE");
    
    usernameLayout->addWidget(name);
    usernameLayout->addWidget(discriminator);
    usernameLayout->addStretch(1);

    layout->addWidget(username);

    if (!client.bio.isNull() && !client.bio.isEmpty()) {
        bioSection = new QWidget(content);
        QVBoxLayout *bioLayout = new QVBoxLayout(bioSection);
        bioLayout->setContentsMargins(0, 0, 0, 0);
        bioLayout->setSpacing(8);
        
        Divider *divider = new Divider();
        divider->setContentsMargins(0, 0, 0, 4);
        divider->setFixedHeight(5);

        font.setPixelSize(12);
        QLabel *bioTitle = new QLabel("ABOUT ME", bioSection);
        bioTitle->setFont(font);
        bioTitle->setStyleSheet("color: #B9BBBE");

        bio = new QLabel(client.bio, bioSection);
        bio->setFont(font);
        bio->setStyleSheet("color: #DCDDDE");

        bioLayout->addWidget(divider);
        bioLayout->addWidget(bioTitle);
        bioLayout->addWidget(bio);

        layout->addWidget(bioSection);
    } else {
        bioSection = new QWidget(content);
        bio = nullptr;
    }

    QWidget *customizingSection = new QWidget(content);
    QVBoxLayout *customizingLayout = new QVBoxLayout(customizingSection);
    customizingLayout->setSpacing(8);
    customizingLayout->setContentsMargins(0, 0, 0, 0);

    font.setPixelSize(12);
    QLabel *customizingTitle = new QLabel("CUSTOMIZING MY PROFILE", customizingSection);
    customizingTitle->setFont(font);
    customizingTitle->setStyleSheet("color: #B9BBBE");

    QWidget *activityContainer = new QWidget(customizingSection);
    QHBoxLayout *activityContainerLayout = new QHBoxLayout(activityContainer);
    activityContainerLayout->setSpacing(16);
    activityContainerLayout->setContentsMargins(0, 0, 0, 0);

    QLabel *icon = new QLabel(activityContainer);
    icon->setFixedSize(64, 64);
    icon->setContentsMargins(8, 8, 8, 8);
    icon->setStyleSheet("background-color: #3C45A5");
    QLabel *image = new QLabel(icon);
    image->setFixedSize(48, 48);
    image->move(8, 8);
    image->setPixmap(QPixmap("res/images/png/pen.png").scaled(48, 48));

    QWidget *activity = new QWidget(activityContainer);
    QVBoxLayout *activityLayout = new QVBoxLayout(activity);
    activityLayout->setSpacing(0);
    activityLayout->setContentsMargins(0, 0, 0, 0);

    QLabel *activityTitle = new QLabel("User Profile");
    activityTitle->setFont(font);
    activityTitle->setStyleSheet("color: #DCDDDE");

    font.setBold(false);
    timer = new QTimer();
    timer->setInterval(1000);
    timer->setSingleShot(false);
    QLabel *timeElapsed = new QLabel("00:00 elapsed", activity);
    timeElapsed->setFont(font);
    timeElapsed->setStyleSheet("color: #DCDDDE");
    counter = 0;
    QObject::connect(timer, &QTimer::timeout, [timeElapsed, this](){
        counter++;
        timeElapsed->setText((counter/60 < 10 ? "0" : "") + QString::number(counter/60) + ":" + (counter%60 < 10 ? "0" : "") + QString::number(counter%60) + " elapsed");
    });
    timer->start();

    activityLayout->addStretch(1);
    activityLayout->addWidget(activityTitle);
    activityLayout->addWidget(timeElapsed);
    activityLayout->addStretch(1);

    activityContainerLayout->addWidget(icon);
    activityContainerLayout->addWidget(activity, 0, Qt::AlignVCenter);

    customizingLayout->addWidget(customizingTitle);
    customizingLayout->addWidget(activityContainer);

    layout->addWidget(customizingSection);

    mainLayout->setSpacing(64);
    mainLayout->setContentsMargins(0, 0, 0, 16);
    mainLayout->addWidget(banner);
    mainLayout->addWidget(content);


    avatarBorders = new QLabel(container);
    avatarBorders->setFixedSize(94, 94);
    avatarBorders->move(16, client.banner.isNull() ? 16 : 76);
    avatarBorders->setStyleSheet("border: 7px solid #18191C;"
                                 "border-radius: 47px;");

    if (client.avatar.isNull()) {
        avatar = new RoundedImage("res/images/png/user-icon-asset0.png", 80, 80, 40, container);
    } else {
        avatar = new RoundedImage(80, 80, 40, container);
        QString channelIconFileName = client.id + (client.avatar.indexOf("a_") == 0 ? ".gif" : ".png");
        rm->getImage([this](void *imageFileName){
            avatar->setImage(*reinterpret_cast<QString *>(imageFileName));
        }, "https://cdn.discordapp.com/avatars/" + client.id + "/" + client.avatar, channelIconFileName);
    }
    avatar->move(23, client.banner.isNull() ? 23 : 83);
    avatar->setStyleSheet("border-radius: 40px;"
                          "background-color: none;");
    
    imageUpload = new QLabel(container);
    imageUpload->setFixedSize(28, 28);
    imageUpload->move(75, client.banner.isNull() ? 21 : 81);
    imageUpload->setStyleSheet("background-color: #DCDDDE;"
                               "border-radius: 14px");
    QLabel *imageUploadIcon = new QLabel(imageUpload);
    imageUploadIcon->setFixedSize(18, 18);
    imageUploadIcon->setPixmap(QPixmap("res/images/svg/add-file-icon.svg"));
    imageUploadIcon->move(5, 5);

    ImageHoverWidget *avatarHover = new ImageHoverWidget("CHANGE\nAVATAR", avatar);
    QObject::connect(avatarHover, &ImageHoverWidget::clicked, [this](){
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
                this->setAvatar(imagePath, QString());
                emit avatarChanged(imagePath);
            }
        });
    });

    container->setFixedWidth(300);
    this->setFixedHeight(500);
    this->setFixedWidth(300);
    this->setStyleSheet("background-color: #18191C;"
                        "border-radius: 8px");
}

Profile::~Profile()
{
    timer->stop();
}

void Profile::setAvatar(QString file, QString color)
{
    if (file.isNull()) {
        avatar->clear();
        avatar->setStyleSheet("border-radius: 40px;"
                              "background-color:" + color);
    } else {
        avatar->setImage(file);
        avatar->repaint();
    }
}

void Profile::setBanner(QString file, QString color)
{
    if (file.isNull()) {
        banner->setPixmap(QPixmap());
        banner->repaint();
        banner->setStyleSheet("background-color:" + color);
        banner->setFixedHeight(60);
        
        avatarBorders->move(16, 16);
        avatar->move(23, 23);
        imageUpload->move(75, 21);
    } else {
        avatar->setPixmap(QPixmap(file));
        avatar->repaint();
        banner->setFixedHeight(120);

        avatarBorders->move(16, 76);
        avatar->move(23, 83);
        imageUpload->move(75, 81);
    }
}

void Profile::setBio(QString bioText)
{
    if (bioText.isNull() && bio != nullptr) {
        layout->removeItem(layout->itemAt(1));
    } else if (!bioText.isNull()) {
        if (bio == nullptr) {
            QFont font;
            font.setPixelSize(12);
            font.setFamily("whitney");
            
            QVBoxLayout *bioLayout = new QVBoxLayout(bioSection);
            bioLayout->setContentsMargins(0, 0, 0, 0);
            bioLayout->setSpacing(8);
            
            Divider *divider = new Divider();
            divider->setContentsMargins(0, 0, 0, 4);
            divider->setFixedHeight(5);

            QLabel *bioTitle = new QLabel("ABOUT ME", bioSection);
            bioTitle->setFont(font);
            bioTitle->setStyleSheet("color: #B9BBBE");

            bio = new QLabel(bioText, bioSection);
            bio->setFont(font);
            bio->setStyleSheet("color: #DCDDDE");

            bioLayout->addWidget(divider);
            bioLayout->addWidget(bioTitle);
            bioLayout->addWidget(bio);

            layout->insertWidget(1, bioSection);
        } else {
            bio->setText(bioText);
        }
    }
}

}

#include "profile.moc"