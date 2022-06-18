#include "ui/settings/privacysafety.h"

#include "ui/settings/radiobutton.h"
#include "ui/settings/settingsbutton.h"
#include "ui/settings/popup.h"

namespace Ui {

PrivacySafety::PrivacySafety(Api::RessourceManager *rmp, QWidget *parent)
    : QScrollArea()
{
    rm = rmp;

    container = new QWidget(this);
    container->setMaximumWidth(740);
    container->setMinimumHeight(2050);
    container->setContentsMargins(40, 60, 40, 80);
    container->setStyleSheet("background-color: #36393F");
    QVBoxLayout *layout = new QVBoxLayout(container);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);

    QFont font;
    font.setPixelSize(20);
    font.setFamily("whitney");

    QLabel *title = new QLabel("Privacy & Safety", container);
    title->setFont(font);
    title->setStyleSheet("color: #FFF;");

    font.setPixelSize(12);
    QLabel *safeDMDescription = new QLabel("Automatically scan and delete direct messages you receive that contain explicit media content.", container);
    safeDMDescription->setFont(font);
    safeDMDescription->setStyleSheet("color: #B9BBBE");

    layout->addWidget(title);
    layout->addSpacing(20);
    layout->addWidget(createTitle("SAFE DIRECT MESSAGING"));
    layout->addSpacing(8);
    layout->addWidget(safeDMDescription);
    layout->addSpacing(8);

    rm->getClientSettings([this, layout](void *settingsPtr){
        Api::ClientSettings *settings = reinterpret_cast<Api::ClientSettings *>(settingsPtr);

        QVector<RadioParameters> radioParameters;
        radioParameters.append(RadioParameters{"Keep me safe", "Scan direct messages from everyone.", RadioParameters::Green});
        radioParameters.append(RadioParameters{"My friends are nice", "Scan direct messages from everyone unless they are a friend.", RadioParameters::Orange});
        radioParameters.append(RadioParameters{"Do not scan", "Direct messages will not be scanned for explicit content.", RadioParameters::Red});
        RadioGroup *safeDMRadio = new RadioGroup(radioParameters, 2 - settings->explicitContentFilter, container);
        QObject::connect(safeDMRadio, &RadioGroup::clicked, [this](int index){
            QString settings;
            if (index == 0)
                settings = "MjFKAggBUgIIAVoCCAFiAggBagIIAXICCAF6AIIBAggBigEAmgECCAKiAQIIAaoBAggB";
            else if (index == 1)
                settings = "MjFKAggBUgIIAVoCCAFiAggBagIIAXICCAF6AIIBAggBigEAmgECCAGiAQIIAaoBAggB";
            else
                settings = "Mi9KAggBUgIIAVoCCAFiAggBagIIAXICCAF6AIIBAggBigEAmgEAogECCAGqAQIIAQ==";
            rm->requester->setSettingsProto([](void *){}, settings);
        });

        layout->addWidget(safeDMRadio);
        layout->addSpacing(44);
        layout->addWidget(createTitle("SERVER PRIVACY DEFAULT"));
        layout->addSpacing(8);
        layout->addWidget(createSection(std::function<void(bool)>([this](bool active){
            if (active)
                rm->requester->setSettings([](void *){}, "{\"default_guilds_restricted\":false,\"resticted_guilds\":[]}");
            else
                rm->getGuilds([this](void *guildsPtr){
                    QVector<Api::Guild *> guilds = *reinterpret_cast<QVector<Api::Guild *> *>(guildsPtr);
                    QString settings("{\"default_guilds_restricted\":true,\"resticted_guilds\":[");
                    for (unsigned int i = 0 ; i < guilds.size() ; i++)
                        settings += guilds[i]->id + (i + 1 == guilds.size() ? "]}" : ",");
                    rm->requester->setSettings([](void *){}, settings);
                });
        }), "Allow direct messages from server members", 
            "This setting is applied when you join a new server. It does not apply retroactively to your existing servers.", (optbool)settings->defaultGuildsRestricted, SectionType::None));
        layout->addSpacing(20);
        layout->addWidget(createSection(std::function<void(bool)>([this](bool active){
            QString settings;
            if (active)
                settings = "MjFKAggBUgIIAVoCCAFiAggBagIIAXICCAF6AIIBAggBigEAmgECCAKiAQIIAaoBAggB";
            else
                settings = "Mi9KAggBUgIIAVoCCAFiAggBagIIAXICCAF6AIIBAggBigEAmgECCAKiAQCqAQIIAQ==";
            rm->requester->setSettingsProto([](void *){}, settings);
        }), "Allow access to age-restricted servers on iOS", 
            "After joining on desktop, view your servers for people 18+ on iOS devices.", (optbool)settings->viewNsfwGuilds, SectionType::None));
        layout->addSpacing(60);

        all = settings->friendSourceFlags->all;
        mutualFriends = settings->friendSourceFlags->mutualFriends || settings->friendSourceFlags->all;
        mutualGuilds = settings->friendSourceFlags->mutualGuilds || settings->friendSourceFlags->all;
        layout->addWidget(createTitle("WHO CAN SEND YOU A FRIEND REQUEST"));        
        layout->addSpacing(8);
        layout->addWidget(createSection(std::function<void(bool)>([this](bool active){
            all = active;
            if (active) {
                mutualFriends = true;
                mutualGuilds = true;
                mutualFriendsSwitch->setState(true);
                mutualGuildsSwitch->setState(true);
            }
            rm->requester->setSettings([](void *){}, "{\"friend_source_flags\":{"
                "\"all\":" + QString(all ? "true," : "false,") +
                "\"mutual_friends\":" + QString(mutualFriends ? "true," : "false,") +
                "\"mutual_guilds\":" + QString(mutualGuilds ? "true}}" : "false}}")
            );
        }), "Everyone", QString(), (optbool)settings->friendSourceFlags->all, SectionType::All));
        layout->addSpacing(20);
        layout->addWidget(createSection(std::function<void(bool)>([this](bool active){
            mutualFriends = active;
            if (!active && all) {
                all = false;
                allSwitch->setState(false);
            } else if (active && mutualGuilds) {
                all = true;
                allSwitch->setState(true);
            }
            rm->requester->setSettings([](void *){}, "{\"friend_source_flags\":{"
                "\"all\":" + QString(all ? "true," : "false,") +
                "\"mutual_friends\":" + QString(mutualFriends ? "true," : "false,") +
                "\"mutual_guilds\":" + QString(mutualGuilds ? "true}}" : "false}}")
            );
        }), "Friends of Friends", QString(), (optbool)(settings->friendSourceFlags->mutualFriends || settings->friendSourceFlags->all), SectionType::MutualFriends));
        layout->addSpacing(20);
        layout->addWidget(createSection(std::function<void(bool)>([this](bool active){
            mutualGuilds = active;
            if (!active && all) {
                all = false;
                allSwitch->setState(false);
            } else if (active && mutualFriends) {
                all = true;
                allSwitch->setState(true);
            }
            rm->requester->setSettings([](void *){}, "{\"friend_source_flags\":{"
                "\"all\":" + QString(all ? "true," : "false,") +
                "\"mutual_friends\":" + QString(mutualFriends ? "true," : "false,") +
                "\"mutual_guilds\":" + QString(mutualGuilds ? "true}}" : "false}}")
            );
        }), "Server Members", QString(), (optbool)(settings->friendSourceFlags->mutualGuilds || settings->friendSourceFlags->all), SectionType::MutualGuilds));
        layout->addSpacing(60);

        layout->addWidget(createTitle("RICH PRESENCE"));  
        layout->addSpacing(8);
        layout->addWidget(createSection(std::function<void(bool)>([this](bool active){
            QString settings;
            if (active)
                settings = "QhYKAggBQgIIAUoCCAFSAggBWgIIDmIA";
            else
                settings = "QhQKAEICCAFKAggBUgIIAVoCCA5iAA==";
            rm->requester->setSettingsProto([](void *){}, settings);
        }), "Allow friends to join your game.", 
            "This setting allow friends to join your game without sending a request.", (optbool)Optional::None, SectionType::None));
        layout->addSpacing(20);
        layout->addWidget(createSection(std::function<void(bool)>([this](bool active){
            QString settings;
            if (active)
                settings = "QhoKAggBEgIIAUICCAFKAggBUgIIAVoCCA5iAA==";
            else
                settings = "QhgKAggBEgBCAggBSgIIAVICCAFaAggOYgA=";
            rm->requester->setSettingsProto([](void *){}, settings);
        }), "Allow voice channel participants to join your game.", 
            "This setting allow people that are in the same voice channel as you to join your game without sending a request.\nThis feature only works in non-community servers.", (optbool)Optional::None, SectionType::None));
        layout->addSpacing(60);

        rm->requester->getConsent([layout, settings, this](void *dataPtr){
            QJsonDocument data = *reinterpret_cast<QJsonDocument *>(dataPtr);

            layout->addWidget(createTitle("HOW WE USE YOUR DATA"));  
            layout->addSpacing(8);
            layout->addWidget(createSection(std::function<void(bool)>([this](bool active){
                if (active)
                    rm->requester->setConsent([](void *){}, "\"usage_statistics\"", QString());
                else
                    rm->requester->setConsent([](void *){}, QString(), "\"usage_statistics\"");
            }), "Use data to improve Discord", "This setting allows us to use and process information about how you navigate and use Discord for analytical\npurposes. For example, it allows us to include you in new feature experiments we test.\n Learn more at https://support.discord.com/hc/en-us/articles/360004109911", data["personalization"]["consented"].toInt(), SectionType::None));
            layout->addSpacing(20);
            layout->addWidget(createSection(std::function<void(bool)>([this](bool active){
                if (active)
                    rm->requester->setConsent([](void *){}, "\"personalization\"", QString());
                else
                    rm->requester->setConsent([](void *){}, QString(), "\"personalization\"");
            }), "Use data to customize my Discord experience", "This setting allows us to use information, such as who you talk to and what games you play, to customize Discord\nfor you. Learn more at https://support.discord.com/hc/en-us/articles/360004109911", data["usage_statistics"]["consented"].toInt(), SectionType::None));
            layout->addSpacing(20);
            layout->addWidget(createSection(std::function<void(bool)>([this](bool active){
                QString settings;
                if (active)
                    settings = "QhwKAggBEgIIATgBQgIIAUoCCAFSAggBWgIIDmIA";
                else
                    settings = "QhoKAggBEgIIAUICCAFKAggBUgIIAVoCCA5iAA==";
                rm->requester->setSettingsProto([](void *){}, settings);
            }), "Allow Discord to track screen reader usage", "This setting allows us to record when you use a screen reader while using Discord so that we can improve\naccessibility. Learn more at https://support.discord.com/hc/en-us/articles/360035966492", settings->allowAccessibilityDetection, SectionType::None));
            layout->addSpacing(20);
            layout->addWidget(createSection(std::function<void(bool)>([this](bool active){}), "Use data to make Discord work", "We need to store and process some data in order to provide you the basic Discord service, such as your messages,\nwhat servers you're in and you Direct Messages. By using Discord, you allow us to provide this basic service. You\ncan stop this by Disabling or Deleting your account (in the My Account menu).", (optbool)Optional::None, SectionType::None));
            layout->addSpacing(20);
            layout->addWidget(createSection(std::function<void(bool)>([this](bool active){}), "Request all of my Data", "Learn more at https://support.discord.com/hc/en-us/articles/360004027692 about how getting a copy of your personnal data\nworks.", (optbool)Optional::None, SectionType::None));

            QFont font;
            font.setPixelSize(12);
            font.setFamily("whitney");
            
            rm->requester->getHarvest([layout, font, this](void *dataPtr){
                QJsonDocument data = *reinterpret_cast<QJsonDocument *>(dataPtr);
                
                if (data["created_at"].isUndefined() || (QDateTime::fromString(data["created_at"].toString(), Qt::ISODate).addMonths(1).toLocalTime() < QDateTime::currentDateTime())) {
                    SettingsButton *requestButton = new SettingsButton(SettingsButton::Type::NormalOutlined, "Request Data", container);
                    QObject::connect(requestButton, &SettingsButton::clicked, [font, requestButton, layout, this](){
                        QWidget *parentWidget = this;
                        while (parentWidget->parent()) parentWidget = (QWidget *)parentWidget->parent();
                        PopUp *popUp = new PopUp(new QWidget(), 440, 206, QString(), "Submit Data Request", false, false, "It may take us up to 30 days to collect your data. We will send you an email to the address you registered with when to package is ready.", "I've changed my mind", "Request My Data", true, true, parentWidget->size(), parentWidget);
                        QObject::connect(popUp, &PopUp::cancelled, [popUp](){popUp->deleteLater();});
                        QObject::connect(popUp, &PopUp::done, [font, parentWidget, requestButton, popUp, layout, this](){
                            rm->requester->harvestData();

                            QWidget *requestData = new QWidget(container);
                            requestData->setFixedHeight(62);
                            requestData->setStyleSheet("background-color: rgba(32, 34, 37, 0.6);"
                                                       "border-radius: 5px;"
                                                       "border: 1px solid #202225");
                            QHBoxLayout *requestLayout = new QHBoxLayout(requestData);
                            requestLayout->setContentsMargins(0, 0, 0, 0);
                            QLabel *requestText = new QLabel();

                            QDateTime time = QDateTime::currentDateTime().addMonths(1);
                            int dayMod = (time.date().day() - 1) % 10;
                            requestText->setText("You've recently requested a copy of your data. You can request again on " + QLocale(QLocale::English).monthName(time.date().month())
                                + " " + QString::number(time.date().day() - 1) + (dayMod == 1 ? "st " : dayMod == 2 ? "nd " : dayMod == 3 ? "rd " : "th ")
                                + QString::number(time.date().year()));
                            requestText->setStyleSheet("color: #FFF;"
                                                       "border: none;"  
                                                       "background-color: none");

                            requestText->setFont(font);
                            requestLayout->addWidget(requestText, 0, Qt::AlignCenter);

                            requestButton->deleteLater();
                            layout->insertWidget(20, requestData);                            

                            popUp->deleteLater();
                            PopUp *resultPopUp = new PopUp(new QWidget(), 440, 200, QString(), QString(), false, false, "Our privacy farmers have begun harvesting your data. This can take up to 30 days, but we'll email you when it's done.", QString(), QString(), false, true, parentWidget->size(), parentWidget);
                            QObject::connect(resultPopUp, &PopUp::cancelled, [resultPopUp](){resultPopUp->deleteLater();});
                        });
                    });
                    layout->addWidget(requestButton);
                } else {
                    QWidget *requestData = new QWidget(container);
                    requestData->setFixedHeight(62);
                    requestData->setStyleSheet("background-color: rgba(32, 34, 37, 0.6);"
                                            "border-radius: 5px;"
                                            "border: 1px solid #202225");
                    QHBoxLayout *requestLayout = new QHBoxLayout(requestData);
                    requestLayout->setContentsMargins(0, 0, 0, 0);
                    QLabel *requestText = new QLabel();

                    QDateTime time = QDateTime::fromString(data["created_at"].toString(), Qt::ISODate).addMonths(1).toLocalTime();
                    int dayMod = (time.date().day() - 1) % 10;
                    requestText->setText("You've recently requested a copy of your data. You can request again on " + QLocale(QLocale::English).monthName(time.date().month())
                        + " " + QString::number(time.date().day() - 1) + (dayMod == 1 ? "st " : dayMod == 2 ? "nd " : dayMod == 3 ? "rd " : "th ")
                        + QString::number(time.date().year()));
                    requestText->setStyleSheet("color: #FFF;"
                                               "border: none;"  
                                               "background-color: none");

                    requestText->setFont(font);
                    requestLayout->addWidget(requestText, 0, Qt::AlignCenter);

                    layout->addWidget(requestData);
                }
                layout->addSpacing(60);

                QWidget *footer = new QWidget(container);
                footer->setFixedHeight(62);
                footer->setStyleSheet("background-color: rgba(32, 34, 37, 0.6);"
                                      "border-radius: 5px;"
                                      "border: 1px solid #202225");
                QHBoxLayout *footerLayout = new QHBoxLayout(footer);
                footerLayout->setContentsMargins(0, 0, 0, 0);
                QLabel *footerText = new QLabel("Check out our Terms of Service (https://discord.com/terms) and Privacy Policy (https://discord.com/privacy)");
                footerText->setFont(font);
                footerText->setStyleSheet("color: #FFF;"
                                          "border: none;"  
                                          "background-color: none");
                footerLayout->addWidget(footerText, 0, Qt::AlignCenter);

                layout->addWidget(footer);
                layout->addSpacing(60);
            });
        });
    });

    this->setWidget(container);
    this->setStyleSheet("QScrollBar::handle {border: none; border-radius: 2px; background-color: #202225;}"
                        "QScrollBar {border: none; background-color: #36393F; border-radius: 8px; width: 3px;}"
                        "QScrollBar::add-line, QScrollBar::sub-line {border:none; background: none; height: 0;}");
}

QLabel *PrivacySafety::createTitle(QString text)
{
    QFont font;
    font.setPixelSize(11);
    font.setBold(true);
    font.setFamily("whitney");

    QLabel *title = new QLabel(text, container);
    title->setFont(font);
    title->setStyleSheet("color: #B9BBBE");

    return title;
}

QWidget *PrivacySafety::createSection(std::function<void(bool)> callback, QString titleStr, QString descriptionStr, optbool active, SectionType type)
{
    QFont font;
    font.setPixelSize(14);
    font.setFamily("whitney");

    QWidget *section = new QWidget(container);
    QVBoxLayout *layout = new QVBoxLayout(section);
    layout->setSpacing(8);
    layout->setContentsMargins(0, 0, 0, 0);


    QWidget *header = new QWidget(section);
    QHBoxLayout *headerLayout = new QHBoxLayout(header);
    headerLayout->setSpacing(0);
    headerLayout->setContentsMargins(0, 0, 0, 0);

    QLabel *title = new QLabel(titleStr, header);
    title->setFont(font);
    title->setStyleSheet("color: #FFF");

    headerLayout->addWidget(title);

    if (active != (optbool)Optional::None) {
        SwitchButton *button = new SwitchButton(active);
        if (type != SectionType::None) {
            if (type == SectionType::All)
                allSwitch = button;
            else if (type == SectionType::MutualFriends)
                mutualFriendsSwitch = button;
            else
                mutualGuildsSwitch = button;
        }
        QObject::connect(button, &SwitchButton::clicked, callback);
        headerLayout->addStretch(1);
        headerLayout->addWidget(button);
    }

    layout->addWidget(header);


    if (!descriptionStr.isNull()) {
        font.setPixelSize(12);
        QLabel *description = new QLabel(descriptionStr, section);
        description->setFont(font);
        description->setStyleSheet("color: #B9BBBE");

        layout->addWidget(description);
    }

    layout->addSpacing(12);

    if (titleStr != "Request all of my Data") {
        QWidget *line = new QWidget(section);
        line->setFixedHeight(1);
        line->setStyleSheet("background-color: rgba(79, 84, 92, 0.48)");
        layout->addWidget(line);
    }

    return section;
}

} // namespace Ui