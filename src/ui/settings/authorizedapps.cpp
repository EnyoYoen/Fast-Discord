#include "ui/settings/authorizedapps.h"

#include "ui/common/roundedimage.h"
#include "ui/settings/settingsbutton.h"

namespace Ui {

class AuthorizedApp : public Widget
{
    Q_OBJECT
public:
    AuthorizedApp(Api::RessourceManager *rm, Api::AuthorizedApp *app, QWidget *parent)
        : Widget(parent)
    {
        QVBoxLayout *layout = new QVBoxLayout(this);
        layout->setContentsMargins(20, 20, 20, 20);
        layout->setSpacing(0);

        Widget *header = new Widget(this);
        header->setFixedHeight(32);
        QHBoxLayout *headerLayout = new QHBoxLayout(header);
        headerLayout->setContentsMargins(0, 0, 0, 0);
        headerLayout->setSpacing(10);

        RoundedImage *avatar = new RoundedImage(32, 32, 16, header);
        rm->getImage([avatar](void *imageFileName){
            avatar->setRoundedImage(*reinterpret_cast<QString *>(imageFileName));
        }, "https://cdn.discordapp.com/app-icons/" + app->application->id + "/" + app->application->icon + ".webp", app->application->id + ".webp");

        QFont font;
        font.setPixelSize(14);
        font.setFamily("whitney");

        Label *appName = new Label(app->application->name, header);
        appName->setFixedSize(QFontMetrics(font).horizontalAdvance(app->application->name), 18);
        appName->setTextColor(Settings::TextNormal);
        appName->setFont(font);

        deauthorize = new SettingsButton(SettingsButton::Type::Critical, "Deauthorize", header);
        deauthorize->hide();
        QObject::connect(deauthorize, &SettingsButton::clicked, [rm, app, this](){
            rm->requester->deleteAuthorizedApp(app->id);
            emit deleted();
            this->deleteLater();
        });

        headerLayout->addWidget(avatar);
        headerLayout->addWidget(appName);
        headerLayout->addStretch(1);
        headerLayout->addWidget(deauthorize);

        layout->addWidget(header);
        layout->addSpacing(20);

        font.setPixelSize(12);
        font.setBold(true);
        Label *aboutTitle = new Label("ABOUT THIS APP", this);
        aboutTitle->setTextColor(Settings::HeaderSecondary);
        aboutTitle->setFont(font);

        layout->addWidget(aboutTitle);
        layout->addSpacing(8);

        font.setPixelSize(14);
        font.setBold(false);
        Label *description = new Label(app->application->description, this);
        description->setTextColor(Settings::TextNormal);
        description->setFont(font);

        layout->addWidget(description);
        layout->addSpacing(20);

        font.setPixelSize(12);
        font.setBold(true);
        Label *permissions = new Label("PERMISSIONS", this);
        permissions->setTextColor(Settings::HeaderSecondary);
        permissions->setFont(font);

        layout->addWidget(permissions);

        font.setPixelSize(14);
        font.setBold(false);
        QVector<QString> scopes = app->scopes;
        for (int i = 0 ; i < scopes.size() ; i++) {
            Widget *permission = new Widget(this);
            permission->setFixedHeight(18);
            QHBoxLayout *permissionLayout = new QHBoxLayout(permission);
            permissionLayout->setContentsMargins(0, 0, 0, 0);
            permissionLayout->setSpacing(5);

            Widget *checkmark = new Widget(permission);
            checkmark->setFixedSize(18, 18);
            checkmark->setImage("res/images/svg/checkmark-icon.svg");

            Label *permissionDesc = new Label(scopesStrings.contains(scopes[i]) ? AuthorizedApp::scopesStrings[scopes[i]] : scopes[i], nullptr);
            permissionDesc->setTextColor(Settings::TextNormal);
            permissionDesc->setFont(font);

            permissionLayout->addWidget(checkmark);
            permissionLayout->addWidget(permissionDesc);

            layout->addSpacing(8);
            layout->addWidget(permission);
        }

        this->setFixedHeight(170 + scopes.size() * 26);
        this->setBorderRadius(5);
        this->setBorderSize(1);
        this->setBorderColor(Settings::BackgroundTertiary);
    }

signals:
    void deleted();

private:
    void enterEvent(QEvent *) override
    {
        deauthorize->show();
    }
    void leaveEvent(QEvent *) override
    {
        deauthorize->hide();
    }

    SettingsButton *deauthorize;

    static QMap<QString, QString> scopesStrings;
};

QMap<QString, QString> AuthorizedApp::scopesStrings = QMap<QString, QString>({
        {"activities.read"                         , "Access your activities"},
        {"activities.write"                        , "Update your activity"},
        /*{"applications.builds.read"                , "Read build data"},
        {"applications.builds.upload"              , "Upload build data"},
        {"applications.commands"                   , "Use commands in a guild"},
        {"applications.commands.update"            , "Update its commands"},
        {"applications.commands.permissions.update", "Update commands persmissions"},
        {"applications.entitlements"               , "Read entitlement"},
        {"applications.store.update"               , "Read and update store data"},*/
        {"bot"                                     , "Join your selected server"},
        {"connections"                             , "Access your connections"},
        {"dm_channels.read"                        , "See information about your DMs"},
        {"email"                                   , "Access your email address"},
        {"gdm.join"                                , "Add you in a group DM"},
        {"guilds"                                  , "Know what servers you're in"},
        {"guilds.join"                             , "Add you in a server"},
        {"guilds.members.read"                     , "See your member information in a server"},
        {"identify"                                , "Access your username, avatar, and banner"},
        {"messages.read"                           , "Read your messages"},
        {"relationships.read"                      , "Know your relationships"},
        {"rpc"                                     , "Control your Discord client"},
        {"rpc.activities.write"                    , "Update your activity"},
        {"rpc.notifications.read"                  , "Read notifications"},
        {"rpc.voice.read"                          , "Access your voice"},
        {"rpc.voice.write"                         , "Update your voice settings"},
        {"voice"                                   , "Join voice channels"},
        {"webhook.incoming"                        , "Webhook"}
    });

AuthorizedApps::AuthorizedApps(Api::RessourceManager *rmp, QWidget *parent)
    : QScrollArea()
{
    rm = rmp;

    container = new Widget(this);
    container->setMaximumWidth(740);
    container->setContentsMargins(40, 60, 40, 80);
    container->setBackgroundColor(Settings::BackgroundPrimary);
    layout = new QVBoxLayout(container);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(8);

    QFont font;
    font.setPixelSize(20);
    font.setFamily("whitney");

    Label *title = new Label("Authorized Apps", container);
    title->setFixedSize(QFontMetrics(font).horizontalAdvance("Authorized Apps"), 20);
    title->setFont(font);
    title->setTextColor(Settings::HeaderPrimary);

    layout->addWidget(title);
    layout->addSpacing(12);

    Widget *description = new Widget(container);
    description->setMinimumHeight(98);
    description->setMaximumHeight(116);
    description->setBorderRadius(5);
    description->setBorderSize(1);
    description->setBorderColor(Settings::BackgroundTertiary);
    description->setBackgroundColor(Settings::DeprecatedCardBg);
    QVBoxLayout *descriptionLayout = new QVBoxLayout(description);
    descriptionLayout->setContentsMargins(20, 20, 20, 20);
    descriptionLayout->setSpacing(4);
    font.setPixelSize(12);
    Label *descriptionTitle = new Label("APPLICATIONS AND CONNECTIONS", nullptr);
    descriptionTitle->setFont(font);
    descriptionTitle->setTextColor(Settings::HeaderSecondary);
    Label *descriptionText = new Label("Here's all the apps that are doing super cool things to make\nyour Discord experience super cooler. If it gets too chilly, you\ncan remove them at any time.", nullptr);
    descriptionText->setMinimumHeight(54);
    descriptionText->setFont(font);
    descriptionText->setTextColor(Settings::TextNormal);
    descriptionLayout->addWidget(descriptionTitle);
    descriptionLayout->addWidget(descriptionText);

    layout->addWidget(description);
    layout->addSpacing(32);

    rm->requester->getAuthorizedApp([this](void *appsPtr){
        QVector<Api::AuthorizedApp *> apps = *reinterpret_cast<QVector<Api::AuthorizedApp *> *>(appsPtr);
        if (apps.isEmpty()) {
            empty();
        } else {
            for (int i = 0 ; i < apps.size() ; i++) {
                AuthorizedApp *app = new AuthorizedApp(rm, apps[i], container);
                QObject::connect(app, &AuthorizedApp::deleted, [this](){
                    if (layout->count() <= 2)
                        empty();
                });
                layout->addWidget(app);
            }
        }
    });

    layout->addStretch(1);

    this->setWidgetResizable(true);
    this->setWidget(container);
    this->setStyleSheet("* {border: none;}"
                        "QScrollBar::handle {border: none; border-radius: 2px; background-color: #202225;}"
                        "QScrollBar {border: none; background-color: #36393F; border-radius: 8px; width: 3px;}"
                        "QScrollBar::add-line, QScrollBar::sub-line {border:none; background: none; height: 0;}");
}

void AuthorizedApps::empty()
{
    Widget *noApps = new Widget(container);
    QVBoxLayout *noAppsLayout = new QVBoxLayout(noApps);
    noAppsLayout->setContentsMargins(0, 0, 0, 0);
    noAppsLayout->setSpacing(0);
    
    Widget *image = new Widget(noApps);
    image->setFixedSize(438, 270);
    image->setImage("res/images/svg/no-app-icon.svg");

    noAppsLayout->addWidget(image, 0, Qt::AlignHCenter);
    noAppsLayout->addSpacing(40);

    QFont font;
    font.setPixelSize(17);
    font.setBold(true);
    font.setFamily("whitney");

    Label *title = new Label("NO AUTHORIZED APPS", noApps);
    title->setFixedSize(QFontMetrics(font).horizontalAdvance("NO AUTHORIZED APPS"), 22);
    title->setTextColor(Settings::TextMuted);
    title->setFont(font);

    noAppsLayout->addWidget(title, 0, Qt::AlignHCenter);
    noAppsLayout->addSpacing(8);
    
    font.setBold(false);
    Label *desc = new Label("Enhance your Discord experience by authorizing and integrating\nthird-party services.", noApps);
    desc->setFixedSize(QFontMetrics(font).horizontalAdvance("Enhance your Discord experience by authorizing and integrating\nthird-party services."), 40);
    desc->setTextColor(Settings::TextMuted);
    desc->setFont(font);

    noAppsLayout->addWidget(desc, 0, Qt::AlignHCenter);

    layout->addWidget(noApps);
}

} // namespace Ui

#include "authorizedapps.moc"