#include "ui/settings/authorizedapps.h"

#include "ui/settings/settingsbutton.h"

namespace Ui {

class AuthorizedApp : public QLabel
{
    Q_OBJECT
public:
    AuthorizedApp(Api::RessourceManager *rm, Api::AuthorizedApp *app, QWidget *parent)
        : QLabel(parent)
    {
        QVBoxLayout *layout = new QVBoxLayout(this);
        layout->setContentsMargins(20, 20, 20, 20);
        layout->setSpacing(0);

        QWidget *header = new QWidget(this);
        header->setFixedHeight(32);
        header->setStyleSheet("border: none");
        QHBoxLayout *headerLayout = new QHBoxLayout(header);
        headerLayout->setContentsMargins(0, 0, 0, 0);
        headerLayout->setSpacing(10);

        RoundedImage *avatar = new RoundedImage(32, 32, 16, header);
        rm->getImage([avatar](void *imageFileName){
            avatar->setImage(*reinterpret_cast<QString *>(imageFileName));
        }, "https://cdn.discordapp.com/app-icons/" + app->application->id + "/" + app->application->icon + ".webp", app->application->id + ".webp");

        QFont font;
        font.setPixelSize(14);
        font.setFamily("whitney");

        QLabel *appName = new QLabel(app->application->name, header);
        appName->setStyleSheet("color: #DCDDDE");
        appName->setFont(font);

        deauthorize = new SettingsButton(SettingsButton::Type::Critical, "Deauthorize", header);
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
        QLabel *aboutTitle = new QLabel("ABOUT THIS APP", this);
        aboutTitle->setStyleSheet("color: #B9BBBE;"
                                  "border: none");
        aboutTitle->setFont(font);

        layout->addWidget(aboutTitle);
        layout->addSpacing(8);

        font.setPixelSize(14);
        font.setBold(false);
        QLabel *description = new QLabel(app->application->description, this);
        description->setStyleSheet("color: #DCDDDE;"
                                   "border: none");
        description->setFont(font);

        layout->addWidget(description);
        layout->addSpacing(20);

        font.setPixelSize(12);
        font.setBold(true);
        QLabel *permissions = new QLabel("PERMISSIONS", this);
        permissions->setStyleSheet("color: #B9BBBE;"
                                   "border: none");
        permissions->setFont(font);

        layout->addWidget(permissions);

        font.setPixelSize(14);
        font.setBold(false);
        QVector<QString> scopes = app->scopes;
        for (unsigned int i = 0 ; i < scopes.size() ; i++) {
            QWidget *permission = new QWidget(this);
            permission->setStyleSheet("border: none");
            permission->setFixedHeight(18);
            QHBoxLayout *permissionLayout = new QHBoxLayout(permission);
            permissionLayout->setContentsMargins(0, 0, 0, 0);
            permissionLayout->setSpacing(5);

            QLabel *checkmark = new QLabel(permission);
            checkmark->setFixedSize(18, 18);
            checkmark->setStyleSheet("background-image: url(\"res/images/svg/checkmark-icon.svg\")");

            QLabel *permissionDesc = new QLabel(scopesStrings.contains(scopes[i]) ? AuthorizedApp::scopesStrings[scopes[i]] : scopes[i]);
            permissionDesc->setStyleSheet("color: #DCDDDE");
            permissionDesc->setFont(font);

            permissionLayout->addWidget(checkmark);
            permissionLayout->addWidget(permissionDesc);

            layout->addSpacing(8);
            layout->addWidget(permission);
        }

        this->setFixedHeight(170 + scopes.size() * 26);
        this->setStyleSheet("border: 1px solid #202225;"
                            "border-radius: 5px");
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

    container = new QWidget(this);
    container->setMaximumWidth(740);
    container->setContentsMargins(40, 60, 40, 80);
    container->setStyleSheet("background-color: #36393F");
    layout = new QVBoxLayout(container);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(8);

    QFont font;
    font.setPixelSize(20);
    font.setFamily("whitney");

    QLabel *title = new QLabel("Authorized Apps", container);
    title->setFont(font);
    title->setStyleSheet("color: #FFF;");

    layout->addWidget(title);
    layout->addSpacing(12);

    QWidget *description = new QWidget(container);
    description->setMinimumHeight(98);
    description->setMaximumHeight(116);
    description->setStyleSheet("background-color: rgba(32, 34, 37, 0.6);"
                               "border-radius: 5px;"
                               "border: 1px solid #202225");
    QVBoxLayout *descriptionLayout = new QVBoxLayout(description);
    descriptionLayout->setContentsMargins(20, 20, 20, 20);
    descriptionLayout->setSpacing(4);
    font.setPixelSize(12);
    QLabel *descriptionTitle = new QLabel("APPLICATIONS AND CONNECTIONS");
    descriptionTitle->setFont(font);
    descriptionTitle->setStyleSheet("color: #B9BBBE;"
                                    "border: none;"  
                                    "background-color: none");
    QLabel *descriptionText = new QLabel("Here's all the apps that are doing super cool things to make\nyour Discord experience super cooler. If it gets too chilly, you\ncan remove them at any time.");
    descriptionText->setFont(font);
    descriptionText->setStyleSheet("color: #DCDDDE;"
                                   "border: none;"  
                                   "background-color: none");
    descriptionLayout->addWidget(descriptionTitle);
    descriptionLayout->addWidget(descriptionText);

    layout->addWidget(description);
    layout->addSpacing(32);

    rm->requester->getAuthorizedApp([this](void *appsPtr){
        QVector<Api::AuthorizedApp *> apps = *reinterpret_cast<QVector<Api::AuthorizedApp *> *>(appsPtr);
        if (apps.isEmpty()) {
            empty();
        } else {
            for (unsigned int i = 0 ; i < apps.size() ; i++) {
                AuthorizedApp *app = new AuthorizedApp(rm, apps[i], container);
                QObject::connect(app, &AuthorizedApp::deleted, [this](){
                    if (layout->count() <= 2)
                        empty();
                });
                layout->addWidget(app);
            }
        }
    });

    this->setWidgetResizable(true);
    this->setWidget(container);
    this->setStyleSheet("QScrollBar::handle {border: none; border-radius: 2px; background-color: #202225;}"
                        "QScrollBar {border: none; background-color: #36393F; border-radius: 8px; width: 3px;}"
                        "QScrollBar::add-line, QScrollBar::sub-line {border:none; background: none; height: 0;}");
}

void AuthorizedApps::empty()
{
    QWidget *noApps = new QWidget(container);
    QVBoxLayout *noAppsLayout = new QVBoxLayout(noApps);
    noAppsLayout->setContentsMargins(0, 0, 0, 0);
    noAppsLayout->setSpacing(0);
    
    QLabel *image = new QLabel(noApps);
    image->setFixedSize(438, 270);
    image->setStyleSheet("background-image: url(\"res/images/svg/no-app-icon.svg\")");

    noAppsLayout->addWidget(image, 0, Qt::AlignHCenter);
    noAppsLayout->addSpacing(40);

    QFont font;
    font.setPixelSize(17);
    font.setBold(true);
    font.setFamily("whitney");

    QLabel *title = new QLabel("NO AUTHORIZED APPS", noApps);
    title->setStyleSheet("color: #A3A6AA");
    title->setFont(font);

    noAppsLayout->addWidget(title, 0, Qt::AlignHCenter);
    noAppsLayout->addSpacing(8);
    
    font.setBold(false);
    QLabel *desc = new QLabel("Enhance your Discord experience by authorizing and integrating\nthird-party services.", noApps);
    desc->setStyleSheet("color: #A3A6AA");
    desc->setFont(font);

    noAppsLayout->addWidget(desc, 0, Qt::AlignHCenter);

    layout->addWidget(noApps);
}

} // namespace Ui

#include "authorizedapps.moc"