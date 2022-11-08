#include "ui/settings/components/profile.h"

#include "ui/settings/components/divider.h"
#include "ui/settings/components/popup.h"
#include "ui/settings/components/selectimagebutton.h"

#include <QVBoxLayout>
#include <QPixmap>
#include <QFileDialog>

namespace Ui {

class ImageHoverWidget : public Widget
{
    Q_OBJECT
public: 
    ImageHoverWidget(QString text, QWidget *parent)
        : Widget(parent)
    {
        this->setAttribute(Qt::WA_StyledBackground);
        this->setBackgroundColor(Settings::None);

        QVBoxLayout *layout = new QVBoxLayout(parent);
        layout->setContentsMargins(0, 0, 0, 0);
        QVBoxLayout *thisLayout = new QVBoxLayout(this);
        thisLayout->setContentsMargins(0, 0, 0, 0);
        textLabel = new Label(text, this);
        textLabel->hide();
        QFont font;
        font.setPixelSize(Settings::scale(11));
        font.setFamily("whitney");
        font.setBold(true);

        QString temp = text;
        QString final = text;
        int maxSize = 0;
        int idx = -1;
        while ((idx = temp.indexOf('\n')) != -1) {
            QString tmp = temp.mid(0, idx);
            if (tmp.size() > maxSize) {
                maxSize = tmp.size();
                final = tmp;
            }
            temp = temp.mid(idx + 1);
        }
        
        textLabel->setMinimumSize(QFontMetrics(font).horizontalAdvance(final), Settings::scale(14) * (text.count('\n') + 1));
        textLabel->setFont(font);
        textLabel->setTextColor(Settings::White);
        textLabel->setBackgroundColor(Settings::None);
        thisLayout->addWidget(textLabel, 0, Qt::AlignCenter);
        layout->addWidget(this);
    }

signals:
    void clicked();

private:
    Label *textLabel;
    void mouseReleaseEvent(QMouseEvent *event) override
    {
        if (event->button() == Qt::LeftButton)
            emit clicked();
    }
    void enterEvent(QEvent *) override
    {
        this->setBackgroundColor(Settings::Black40);
        textLabel->setBackgroundColor(Settings::None);
        textLabel->show();
    }
    void leaveEvent(QEvent *) override
    {
        this->setBackgroundColor(Settings::None);
        textLabel->setBackgroundColor(Settings::None);
        textLabel->hide();
    }
};

Profile::Profile(Api::RessourceManager *rm, Api::Client client, QWidget *parent)
    : Widget(parent)
{
    Widget *container = new Widget(this);
    container->setBorderRadius(Settings::scale(8));
    QVBoxLayout *mainLayout = new QVBoxLayout(container);

    QFont font;
    font.setPixelSize(Settings::scale(20));
    font.setFamily("whitney");
    font.setBold(true);


    banner = new Widget(container);
    banner->setBorderRadius(Settings::scale(8), Settings::scale(8), 0, 0);
    banner->setFixedWidth(Settings::scale(300));
    if (!client.banner.isNull()) {
        banner->setFixedHeight(Settings::scale(120));
        rm->getImage([this](Api::CallbackStruct cb){
            banner->setPixmap(QPixmap(*reinterpret_cast<QString *>(cb.data)).scaledToWidth(300));
        }, "https://cdn.discordapp.com/banners/" + client.id + "/" + client.banner + ".png?size=600", client.banner);
    } else if (client.bannerColor != 0) {
        banner->setFixedHeight(Settings::scale(60));
        banner->setBackgroundColor(QColor((client.bannerColor & 0xFF000000) >> 24, (client.bannerColor & 0x00FF0000) >> 16, (client.bannerColor & 0x0000FF00) >> 8));
    } else {
        if (client.avatar.isNull()) {
            banner->setBackgroundColor(QColor(0, 0, 0));
        } else {
            QString channelIconFileName = client.id + (client.avatar.indexOf("a_") == 0 ? ".gif" : ".png");
            rm->getImage([this](Api::CallbackStruct cb){
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
        banner->setFixedHeight(Settings::scale(60));
    }

    ImageHoverWidget *bannerHover = new ImageHoverWidget((client.purchasedFlags ? "CHANGE BANNER" : "UNLOCK BANNER"), banner);
    bannerHover->setBorderRadius(Settings::scale(8), Settings::scale(8), 0, 0);
    if (client.purchasedFlags) {
        QObject::connect(bannerHover, &ImageHoverWidget::clicked, [this](){
            QWidget *parentWidget = this;
            while (parentWidget->parent()) parentWidget = (QWidget *)parentWidget->parent();

            QFont font;
            font.setPixelSize(12);
            font.setFamily("whitney");
            Widget *container = new Widget(nullptr);
            container->setFixedSize(Settings::scale(200), Settings::scale(212));
            QHBoxLayout *mainLayout = new QHBoxLayout(container);
            mainLayout->setContentsMargins(Settings::scale(16), 0, Settings::scale(16), Settings::scale(16));
            Label *content = new Label(container);
            content->setFixedSize(Settings::scale(160), Settings::scale(196));
            content->setBackgroundColor(Settings::BackgroundSecondaryAlt);
            QVBoxLayout *layout = new QVBoxLayout(content);
            layout->setSpacing(Settings::scale(32));
            layout->setContentsMargins(Settings::scale(16), Settings::scale(16), Settings::scale(16), Settings::scale(16));
            SelectImageButton *button = new SelectImageButton(content);
            QLabel *desc = new QLabel("Upload Image", content);
            desc->setFont(font);
            layout->addWidget(button);
            layout->addWidget(desc, 0, Qt::AlignHCenter);
            mainLayout->addWidget(content, 0, Qt::AlignCenter);

            PopUp *popUp = new PopUp(container, Settings::scale(240), Settings::scale(280), QString(), "Select Image", false, true, QString(), QString(), QString(), false, false, parentWidget->size(), parentWidget);
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
            PopUp *popUp = new PopUp(new Widget(nullptr), Settings::scale(440), Settings::scale(120), QString(), "Get Nitro", true, false, "<div style=\"text-align: center\">You can get nitro at <a style=\"color: #00AFF4; text-decoration: none;\" href=\"https://discord.com/nitro\">https://discord.com/nitro</a></div>", QString(), QString(), false, false, parentWidget->size(), parentWidget);
            QObject::connect(popUp, &PopUp::cancelled, [popUp](){popUp->deleteLater();});
        });
    }

    Widget *content = new Widget(container);
    content->move(0, client.banner.isNull() ? Settings::scale(124) : Settings::scale(184));
    content->setBackgroundColor(Settings::BackgroundFloating);
    layout = new QVBoxLayout(content);
    layout->setSpacing(Settings::scale(16));
    layout->setContentsMargins(Settings::scale(16), 0, Settings::scale(16), 0);

    Widget *username = new Widget(content);
    username->setBackgroundColor(Settings::BackgroundFloating);
    username->setFixedHeight(Settings::scale(32));
    QHBoxLayout *usernameLayout = new QHBoxLayout(username);
    usernameLayout->setSpacing(0);
    usernameLayout->setContentsMargins(0, 0, Settings::scale(16), 0);

    Label *name = new Label(client.username, username);
    name->setFixedSize(QFontMetrics(font).horizontalAdvance(client.username), Settings::scale(24));
    name->setFont(font);
    name->setTextColor(Settings::HeaderPrimary);

    Label *discriminator = new Label("#" + client.discriminator, username);
    discriminator->setFixedSize(QFontMetrics(font).horizontalAdvance("#" + client.discriminator), Settings::scale(24));
    discriminator->setFont(font);
    discriminator->setTextColor(Settings::HeaderSecondary);
    
    usernameLayout->addWidget(name);
    usernameLayout->addWidget(discriminator);
    usernameLayout->addStretch(1);

    layout->addWidget(username);

    if (!client.bio.isNull() && !client.bio.isEmpty()) {
        bioSection = new Widget(content);
        bioSection->setBackgroundColor(Settings::BackgroundFloating);
        username->setBackgroundColor(Settings::BackgroundFloating);
        QVBoxLayout *bioLayout = new QVBoxLayout(bioSection);
        bioLayout->setContentsMargins(0, 0, 0, 0);
        bioLayout->setSpacing(Settings::scale(8));
        
        Divider *divider = new Divider();
        divider->setContentsMargins(0, 0, 0, Settings::scale(4));
        divider->setFixedHeight(Settings::scale(5));

        font.setPixelSize(Settings::scale(12));
        Label *bioTitle = new Label("ABOUT ME", bioSection);
        bioTitle->setFixedHeight(Settings::scale(16));
        bioTitle->setFont(font);
        bioTitle->setTextColor(Settings::HeaderSecondary);

        bio = new Label(client.bio, bioSection);
        bio->setFixedHeight(Settings::scale(16));
        bio->setFont(font);
        bio->setTextColor(Settings::TextNormal);

        bioLayout->addWidget(divider);
        bioLayout->addWidget(bioTitle);
        bioLayout->addWidget(bio);

        layout->addWidget(bioSection);
    } else {
        bioSection = new Widget(content);
        bio = nullptr;
    }

    Widget *customizingSection = new Widget(content);
    customizingSection->setBackgroundColor(Settings::BackgroundFloating);
    QVBoxLayout *customizingLayout = new QVBoxLayout(customizingSection);
    customizingLayout->setSpacing(Settings::scale(8));
    customizingLayout->setContentsMargins(0, 0, 0, 0);

    font.setPixelSize(Settings::scale(12));
    Label *customizingTitle = new Label("CUSTOMIZING MY PROFILE", customizingSection);
    customizingTitle->setFixedHeight(Settings::scale(16));
    customizingTitle->setFont(font);
    customizingTitle->setTextColor(Settings::HeaderSecondary);

    Widget *activityContainer = new Widget(customizingSection);
    activityContainer->setBackgroundColor(Settings::BackgroundFloating);
    QHBoxLayout *activityContainerLayout = new QHBoxLayout(activityContainer);
    activityContainerLayout->setSpacing(Settings::scale(16));
    activityContainerLayout->setContentsMargins(0, 0, 0, 0);

    Widget *icon = new Widget(activityContainer);
    icon->setBackgroundColor(Settings::BackgroundFloating);
    icon->setFixedSize(Settings::scale(64), Settings::scale(64));
    icon->setContentsMargins(Settings::scale(8), Settings::scale(8), Settings::scale(8), Settings::scale(8));
    Widget *image = new Widget(icon);
    image->setFixedSize(Settings::scale(48), Settings::scale(48));
    image->move(Settings::scale(8), Settings::scale(8));
    image->setPixmap(QPixmap(":pen.png").scaled(Settings::scale(48), Settings::scale(48)));

    Widget *activity = new Widget(activityContainer);
    activity->setBackgroundColor(Settings::BackgroundFloating);
    QVBoxLayout *activityLayout = new QVBoxLayout(activity);
    activityLayout->setSpacing(0);
    activityLayout->setContentsMargins(0, 0, 0, 0);

    Label *activityTitle = new Label("User Profile", activityContainer);
    activityTitle->setFixedHeight(Settings::scale(18));
    activityTitle->setFont(font);
    activityTitle->setTextColor(Settings::TextNormal);

    font.setBold(false);
    timer = new QTimer();
    timer->setInterval(1000);
    timer->setSingleShot(false);
    Label *timeElapsed = new Label("00:00 elapsed", activity);
    timeElapsed->setFixedHeight(Settings::scale(19));
    timeElapsed->setFont(font);
    timeElapsed->setTextColor(Settings::TextNormal);
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

    mainLayout->setSpacing(Settings::scale(64));
    mainLayout->setContentsMargins(0, 0, 0, Settings::scale(16));
    mainLayout->addWidget(banner);
    mainLayout->addWidget(content);


    avatarBorders = new Widget(container);
    avatarBorders->setFixedSize(Settings::scale(94), Settings::scale(94));
    avatarBorders->move(Settings::scale(16), client.banner.isNull() ? Settings::scale(16) : Settings::scale(76));
    avatarBorders->setBorderColor(Settings::BackgroundFloating);
    avatarBorders->setBorderSize(Settings::scale(7));
    avatarBorders->setBorderRadius(Settings::scale(47));

    if (client.avatar.isNull()) {
        avatar = new RoundedImage(":user-icon-asset0.png", Settings::scale(80), Settings::scale(80), Settings::scale(40), container);
    } else {
        avatar = new RoundedImage(Settings::scale(80), Settings::scale(80), Settings::scale(40), container);
        QString channelIconFileName = client.id + (client.avatar.indexOf("a_") == 0 ? ".gif" : ".png");
        rm->getImage([this](Api::CallbackStruct cb){
            avatar->setImage(*reinterpret_cast<QString *>(cb.data));
        }, "https://cdn.discordapp.com/avatars/" + client.id + "/" + client.avatar, channelIconFileName);
    }
    avatar->move(Settings::scale(23), client.banner.isNull() ? Settings::scale(23) : Settings::scale(83));
    avatar->setBackgroundColor(Settings::None);
    avatar->setBorderRadius(Settings::scale(40));
    
    imageUpload = new Widget(container);
    imageUpload->setFixedSize(Settings::scale(28), Settings::scale(28));
    imageUpload->move(Settings::scale(75), client.banner.isNull() ? Settings::scale(21) : Settings::scale(81));
    imageUpload->setBackgroundColor(Settings::BackgroundFloating);
    imageUpload->setBorderRadius(Settings::scale(14));
    Widget *imageUploadIcon = new Widget(imageUpload);
    imageUploadIcon->setFixedSize(Settings::scale(18), Settings::scale(18));
    imageUploadIcon->setBackgroundColor(Settings::BackgroundFloating);
    imageUploadIcon->setPixmap(QPixmap(":add-file-icon.svg"));
    imageUploadIcon->move(Settings::scale(5), Settings::scale(5));

    ImageHoverWidget *avatarHover = new ImageHoverWidget("CHANGE\nAVATAR", avatar);
    avatarHover->setBorderRadius(Settings::scale(40));
    QObject::connect(avatarHover, &ImageHoverWidget::clicked, [this](){
        QWidget *parentWidget = this;
        while (parentWidget->parent()) parentWidget = (QWidget *)parentWidget->parent();

        QFont font;
        font.setPixelSize(Settings::scale(12));
        font.setFamily("whitney");
        Widget *container = new Widget(nullptr);
        container->setFixedSize(Settings::scale(200), Settings::scale(212));
        QHBoxLayout *mainLayout = new QHBoxLayout(container);
        mainLayout->setContentsMargins(Settings::scale(16), 0, Settings::scale(16), Settings::scale(16));
        Label *content = new Label(container);
        content->setFixedSize(Settings::scale(160), Settings::scale(196));
        content->setBackgroundColor(Settings::BackgroundSecondaryAlt);
        QVBoxLayout *layout = new QVBoxLayout(content);
        layout->setSpacing(Settings::scale(32));
        layout->setContentsMargins(Settings::scale(16), Settings::scale(16), Settings::scale(16), Settings::scale(16));
        SelectImageButton *button = new SelectImageButton(content);
        QLabel *desc = new QLabel("Upload Image", content);
        desc->setFont(font);
        layout->addWidget(button);
        layout->addWidget(desc, 0, Qt::AlignHCenter);
        mainLayout->addWidget(content, 0, Qt::AlignCenter);

        PopUp *popUp = new PopUp(container, Settings::scale(240), Settings::scale(280), QString(), "Select Image", false, true, QString(), QString(), QString(), false, false, parentWidget->size(), parentWidget);
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

    container->setFixedWidth(Settings::scale(300));
    this->setFixedHeight(Settings::scale(500));
    this->setFixedWidth(Settings::scale(300));
    this->setBackgroundColor(Settings::BackgroundFloating);
    this->setBorderRadius(Settings::scale(8));
}

Profile::~Profile()
{
    timer->stop();
}

void Profile::setAvatar(QString file, QColor color)
{
    if (file.isNull()) {
        avatar->clear();
        avatar->setBackgroundColor(color);
    } else {
        avatar->setImage(file);
        avatar->repaint();
    }
}

void Profile::setBanner(QString file, QColor color)
{
    if (file.isNull()) {
        banner->setPixmap(QPixmap());
        banner->repaint();
        banner->setBackgroundColor(color);
        banner->setFixedHeight(Settings::scale(60));
        
        avatarBorders->move(Settings::scale(16), Settings::scale(16));
        avatar->move(Settings::scale(23), Settings::scale(23));
        imageUpload->move(Settings::scale(75), Settings::scale(21));
    } else {
        avatar->setPixmap(QPixmap(file));
        avatar->repaint();
        banner->setFixedHeight(Settings::scale(120));

        avatarBorders->move(Settings::scale(16), Settings::scale(76));
        avatar->move(Settings::scale(23), Settings::scale(83));
        imageUpload->move(Settings::scale(75), Settings::scale(81));
    }
}

void Profile::setBio(QString bioText)
{
    if (bioText.isNull() && bio != nullptr) {
        layout->removeItem(layout->itemAt(1));
    } else if (!bioText.isNull()) {
        if (bio == nullptr) {
            QFont font;
            font.setPixelSize(Settings::scale(12));
            font.setFamily("whitney");
            
            bioSection->setFixedHeight(Settings::scale(45) + Settings::scale(18) * bioText.count());

            QVBoxLayout *bioLayout = new QVBoxLayout(bioSection);
            bioLayout->setContentsMargins(0, 0, 0, 0);
            bioLayout->setSpacing(Settings::scale(8));
            
            Divider *divider = new Divider();
            divider->setContentsMargins(0, 0, 0, Settings::scale(4));
            divider->setFixedHeight(Settings::scale(5));

            Label *bioTitle = new Label("ABOUT ME", bioSection);
            bioTitle->setFixedHeight(Settings::scale(16));
            bioTitle->setFont(font);
            bioTitle->setTextColor(Settings::HeaderSecondary);

            bio = new Label(bioText, bioSection);
            bio->setFixedHeight(bioText.count() * Settings::scale(18));
            bio->setFont(font);
            bio->setTextColor(Settings::TextNormal);

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