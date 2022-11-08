#include "ui/settings/menus/privacysafety.h"

#include "ui/settings/components/radiobutton.h"
#include "ui/settings/components/settingsbutton.h"
#include "ui/settings/components/popup.h"

namespace Ui {

PrivacySafety::PrivacySafety(Api::RessourceManager *rmp, QWidget *parent)
    : QScrollArea()
{
    rm = rmp;

    container = new Widget(this);
    container->setMaximumWidth(Settings::scale(740));
    container->setMinimumHeight(Settings::scale(2050));
    container->setContentsMargins(Settings::scale(40), Settings::scale(60), Settings::scale(40), Settings::scale(80));
    container->setBackgroundColor(Settings::BackgroundPrimary);
    QVBoxLayout *layout = new QVBoxLayout(container);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);

    QFont font;
    font.setPixelSize(Settings::scale(20));
    font.setFamily("whitney");

    Label *title = new Label("Privacy & Safety", container);
    title->setFixedSize(QFontMetrics(font).horizontalAdvance("Privacy & Safety"), Settings::scale(16));
    title->setFont(font);
    title->setTextColor(Settings::HeaderPrimary);

    font.setPixelSize(Settings::scale(12));
    Label *safeDMDescription = new Label("Automatically scan and delete direct messages you receive that contain explicit media content.", container);
    safeDMDescription->setFixedSize(QFontMetrics(font).horizontalAdvance("Automatically scan and delete direct messages you receive that contain explicit media content."), Settings::scale(20));
    safeDMDescription->setFont(font);
    safeDMDescription->setTextColor(Settings::HeaderSecondary);

    layout->addWidget(title);
    layout->addSpacing(Settings::scale(20));
    layout->addWidget(createTitle("SAFE DIRECT MESSAGING"));
    layout->addSpacing(Settings::scale(8));
    layout->addWidget(safeDMDescription);
    layout->addSpacing(Settings::scale(8));

    rm->getClientSettings([this, layout](Api::CallbackStruct cb){
        Api::ClientSettings *settings = reinterpret_cast<Api::ClientSettings *>(cb.data);

        QVector<RadioParameters> radioParameters;
        radioParameters.append(RadioParameters{"Keep me safe", "Scan direct messages from everyone.", Settings::RadioBarGreen});
        radioParameters.append(RadioParameters{"My friends are nice", "Scan direct messages from everyone unless they are a friend.", Settings::RadioBarOrange});
        radioParameters.append(RadioParameters{"Do not scan", "Direct messages will not be scanned for explicit content.", Settings::RadioBarRed});
        RadioGroup *safeDMRadio = new RadioGroup(radioParameters, 2 - settings->explicitContentFilter, container);
        QObject::connect(safeDMRadio, &RadioGroup::clicked, [this](int index){
            QString settings;
            if (index == 0)
                settings = "MjFKAggBUgIIAVoCCAFiAggBagIIAXICCAF6AIIBAggBigEAmgECCAKiAQIIAaoBAggB";
            else if (index == 1)
                settings = "MjFKAggBUgIIAVoCCAFiAggBagIIAXICCAF6AIIBAggBigEAmgECCAGiAQIIAaoBAggB";
            else
                settings = "Mi9KAggBUgIIAVoCCAFiAggBagIIAXICCAF6AIIBAggBigEAmgEAogECCAGqAQIIAQ==";
            rm->requester->setSettingsProto([](Api::CallbackStruct cb){}, settings);
        });

        layout->addWidget(safeDMRadio);
        layout->addSpacing(Settings::scale(44));
        layout->addWidget(createTitle("SERVER PRIVACY DEFAULT"));
        layout->addSpacing(Settings::scale(8));
        layout->addWidget(createSection(std::function<void(bool)>([this](bool active){
            if (active)
                rm->requester->setSettings([](Api::CallbackStruct cb){}, "{\"default_guilds_restricted\":false,\"resticted_guilds\":[]}");
            else
                rm->getGuilds([this](Api::CallbackStruct cb){
                    QVector<Api::Guild *> guilds = *reinterpret_cast<QVector<Api::Guild *> *>(cb.data);
                    QString settings("{\"default_guilds_restricted\":true,\"resticted_guilds\":[");
                    for (int i = 0 ; i < guilds.size() ; i++)
                        settings += guilds[i]->id + (i + 1 == guilds.size() ? "]}" : ",");
                    rm->requester->setSettings([](Api::CallbackStruct cb){}, settings);
                });
        }), "Allow direct messages from server members", 
            "This setting is applied when you join a new server. It does not apply retroactively to your existing servers.", (optbool)settings->defaultGuildsRestricted, SectionType::None));
        layout->addSpacing(Settings::scale(20));
        layout->addWidget(createSection(std::function<void(bool)>([this](bool active){
            QString settings;
            if (active)
                settings = "MjFKAggBUgIIAVoCCAFiAggBagIIAXICCAF6AIIBAggBigEAmgECCAKiAQIIAaoBAggB";
            else
                settings = "Mi9KAggBUgIIAVoCCAFiAggBagIIAXICCAF6AIIBAggBigEAmgECCAKiAQCqAQIIAQ==";
            rm->requester->setSettingsProto([](Api::CallbackStruct cb){}, settings);
        }), "Allow access to age-restricted servers on iOS", 
            "After joining on desktop, view your servers for people 18+ on iOS devices.", (optbool)settings->viewNsfwGuilds, SectionType::None));
        layout->addSpacing(Settings::scale(60));

        all = settings->friendSourceFlags->all;
        mutualFriends = settings->friendSourceFlags->mutualFriends || settings->friendSourceFlags->all;
        mutualGuilds = settings->friendSourceFlags->mutualGuilds || settings->friendSourceFlags->all;
        layout->addWidget(createTitle("WHO CAN SEND YOU A FRIEND REQUEST"));        
        layout->addSpacing(Settings::scale(8));
        layout->addWidget(createSection(std::function<void(bool)>([this](bool active){
            all = active;
            if (active) {
                mutualFriends = true;
                mutualGuilds = true;
                mutualFriendsSwitch->setState(true);
                mutualGuildsSwitch->setState(true);
            }
            rm->requester->setSettings([](Api::CallbackStruct cb){}, "{\"friend_source_flags\":{"
                "\"all\":" + QString(all ? "true," : "false,") +
                "\"mutual_friends\":" + QString(mutualFriends ? "true," : "false,") +
                "\"mutual_guilds\":" + QString(mutualGuilds ? "true}}" : "false}}")
            );
        }), "Everyone", QString(), (optbool)settings->friendSourceFlags->all, SectionType::All));
        layout->addSpacing(Settings::scale(20));
        layout->addWidget(createSection(std::function<void(bool)>([this](bool active){
            mutualFriends = active;
            if (!active && all) {
                all = false;
                allSwitch->setState(false);
            } else if (active && mutualGuilds) {
                all = true;
                allSwitch->setState(true);
            }
            rm->requester->setSettings([](Api::CallbackStruct cb){}, "{\"friend_source_flags\":{"
                "\"all\":" + QString(all ? "true," : "false,") +
                "\"mutual_friends\":" + QString(mutualFriends ? "true," : "false,") +
                "\"mutual_guilds\":" + QString(mutualGuilds ? "true}}" : "false}}")
            );
        }), "Friends of Friends", QString(), (optbool)(settings->friendSourceFlags->mutualFriends || settings->friendSourceFlags->all), SectionType::MutualFriends));
        layout->addSpacing(Settings::scale(20));
        layout->addWidget(createSection(std::function<void(bool)>([this](bool active){
            mutualGuilds = active;
            if (!active && all) {
                all = false;
                allSwitch->setState(false);
            } else if (active && mutualFriends) {
                all = true;
                allSwitch->setState(true);
            }
            rm->requester->setSettings([](Api::CallbackStruct cb){}, "{\"friend_source_flags\":{"
                "\"all\":" + QString(all ? "true," : "false,") +
                "\"mutual_friends\":" + QString(mutualFriends ? "true," : "false,") +
                "\"mutual_guilds\":" + QString(mutualGuilds ? "true}}" : "false}}")
            );
        }), "Server Members", QString(), (optbool)(settings->friendSourceFlags->mutualGuilds || settings->friendSourceFlags->all), SectionType::MutualGuilds));
        layout->addSpacing(Settings::scale(60));

        layout->addWidget(createTitle("RICH PRESENCE"));  
        layout->addSpacing(Settings::scale(8));
        layout->addWidget(createSection(std::function<void(bool)>([this](bool active){
            QString settings;
            if (active)
                settings = "QhYKAggBQgIIAUoCCAFSAggBWgIIDmIA";
            else
                settings = "QhQKAEICCAFKAggBUgIIAVoCCA5iAA==";
            rm->requester->setSettingsProto([](Api::CallbackStruct cb){}, settings);
        }), "Allow friends to join your game.", 
            "This setting allow friends to join your game without sending a request.", (optbool)Optional::None, SectionType::None));
        layout->addSpacing(Settings::scale(20));
        layout->addWidget(createSection(std::function<void(bool)>([this](bool active){
            QString settings;
            if (active)
                settings = "QhoKAggBEgIIAUICCAFKAggBUgIIAVoCCA5iAA==";
            else
                settings = "QhgKAggBEgBCAggBSgIIAVICCAFaAggOYgA=";
            rm->requester->setSettingsProto([](Api::CallbackStruct cb){}, settings);
        }), "Allow voice channel participants to join your game.", 
            "This setting allow people that are in the same voice channel as you to join your game without sending a request.\nThis feature only works in non-community servers.", (optbool)Optional::None, SectionType::None));
        layout->addSpacing(Settings::scale(60));

        rm->requester->getConsent([layout, settings, this](Api::CallbackStruct cb){
            QJsonDocument data = *reinterpret_cast<QJsonDocument *>(cb.data);

            layout->addWidget(createTitle("HOW WE USE YOUR DATA"));  
            layout->addSpacing(Settings::scale(8));
            layout->addWidget(createSection(std::function<void(bool)>([this](bool active){
                if (active)
                    rm->requester->setConsent([](Api::CallbackStruct cb){}, "\"usage_statistics\"", QString());
                else
                    rm->requester->setConsent([](Api::CallbackStruct cb){}, QString(), "\"usage_statistics\"");
            }), "Use data to improve Discord", "This setting allows us to use and process information about how you navigate and use Discord for analytical\npurposes. For example, it allows us to include you in new feature experiments we test.\n Learn more at https://support.discord.com/hc/en-us/articles/360004109911", data["personalization"]["consented"].toInt(), SectionType::None));
            layout->addSpacing(Settings::scale(20));
            layout->addWidget(createSection(std::function<void(bool)>([this](bool active){
                if (active)
                    rm->requester->setConsent([](Api::CallbackStruct cb){}, "\"personalization\"", QString());
                else
                    rm->requester->setConsent([](Api::CallbackStruct cb){}, QString(), "\"personalization\"");
            }), "Use data to customize my Discord experience", "This setting allows us to use information, such as who you talk to and what games you play, to customize Discord\nfor you. Learn more at https://support.discord.com/hc/en-us/articles/360004109911", data["usage_statistics"]["consented"].toInt(), SectionType::None));
            layout->addSpacing(Settings::scale(20));
            layout->addWidget(createSection(std::function<void(bool)>([this](bool active){
                QString settings;
                if (active)
                    settings = "QhwKAggBEgIIATgBQgIIAUoCCAFSAggBWgIIDmIA";
                else
                    settings = "QhoKAggBEgIIAUICCAFKAggBUgIIAVoCCA5iAA==";
                rm->requester->setSettingsProto([](Api::CallbackStruct cb){}, settings);
            }), "Allow Discord to track screen reader usage", "This setting allows us to record when you use a screen reader while using Discord so that we can improve\naccessibility. Learn more at https://support.discord.com/hc/en-us/articles/360035966492", settings->allowAccessibilityDetection, SectionType::None));
            layout->addSpacing(Settings::scale(20));
            layout->addWidget(createSection(std::function<void(bool)>([this](bool active){}), "Use data to make Discord work", "We need to store and process some data in order to provide you the basic Discord service, such as your messages,\nwhat servers you're in and you Direct Messages. By using Discord, you allow us to provide this basic service. You\ncan stop this by Disabling or Deleting your account (in the My Account menu).", (optbool)Optional::None, SectionType::None));
            layout->addSpacing(Settings::scale(20));
            layout->addWidget(createSection(std::function<void(bool)>([this](bool active){}), "Request all of my Data", "Learn more at https://support.discord.com/hc/en-us/articles/360004027692 about how getting a copy of your personnal data\nworks.", (optbool)Optional::None, SectionType::None));

            QFont font;
            font.setPixelSize(Settings::scale(12));
            font.setFamily("whitney");
            
            rm->requester->getHarvest([layout, font, this](Api::CallbackStruct cb){
                QJsonDocument data = *reinterpret_cast<QJsonDocument *>(cb.data);
                
                if (data["created_at"].isUndefined() || (QDateTime::fromString(data["created_at"].toString(), Qt::ISODate).addMonths(1).toLocalTime() < QDateTime::currentDateTime())) {
                    SettingsButton *requestButton = new SettingsButton(SettingsButton::Type::NormalOutlined, "Request Data", container);
                    QObject::connect(requestButton, &SettingsButton::clicked, [font, requestButton, layout, this](){
                        QWidget *parentWidget = this;
                        while (parentWidget->parent()) parentWidget = (QWidget *)parentWidget->parent();
                        PopUp *popUp = new PopUp(new Widget(nullptr), 440, 206, QString(), "Submit Data Request", false, false, "It may take us up to 30 days to collect your data. We will send you an email to the address you registered with when to package is ready.", "I've changed my mind", "Request My Data", true, true, parentWidget->size(), parentWidget);
                        QObject::connect(popUp, &PopUp::cancelled, [popUp](){popUp->deleteLater();});
                        QObject::connect(popUp, &PopUp::done, [font, parentWidget, requestButton, popUp, layout, this](){
                            rm->requester->harvestData([](Api::CallbackStruct cb){});

                            Widget *requestData = new Widget(nullptr);
                            requestData->setFixedHeight(Settings::scale(62));
                            requestData->setBackgroundColor(Settings::DeprecatedCardBg);
                            requestData->setBorderRadius(Settings::scale(5));
                            requestData->setBorderSize(Settings::scale(1));
                            requestData->setBorderColor(Settings::BackgroundTertiary);
                            QHBoxLayout *requestLayout = new QHBoxLayout(requestData);
                            requestLayout->setContentsMargins(0, 0, 0, 0);
                            Label *requestText = new Label(nullptr);

                            QDateTime time = QDateTime::currentDateTime().addMonths(1);
                            int dayMod = (time.date().day() - 1) % 10;
                            requestText->setText("You've recently requested a copy of your data. You can request again on " + QLocale(QLocale::English).monthName(time.date().month())
                                + " " + QString::number(time.date().day() - 1) + (dayMod == 1 ? "st " : dayMod == 2 ? "nd " : dayMod == 3 ? "rd " : "th ")
                                + QString::number(time.date().year()));
                            requestText->setBackgroundColor(Settings::None);
                            requestText->setTextColor(Settings::HeaderPrimary);

                            requestText->setFont(font);
                            requestLayout->addWidget(requestText, 0, Qt::AlignCenter);

                            requestButton->deleteLater();
                            layout->insertWidget(20, requestData);                            

                            popUp->deleteLater();
                            PopUp *resultPopUp = new PopUp(new Widget(nullptr), Settings::scale(440), Settings::scale(200), QString(), QString(), false, false, "Our privacy farmers have begun harvesting your data. This can take up to 30 days, but we'll email you when it's done.", QString(), QString(), false, true, parentWidget->size(), parentWidget);
                            QObject::connect(resultPopUp, &PopUp::cancelled, [resultPopUp](){resultPopUp->deleteLater();});
                        });
                    });
                    layout->addWidget(requestButton);
                } else {
                    Widget *requestData = new Widget(container);
                    requestData->setFixedHeight(Settings::scale(62));
                    requestData->setBackgroundColor(Settings::DeprecatedCardBg);
                    requestData->setBorderRadius(Settings::scale(5));
                    requestData->setBorderSize(Settings::scale(1));
                    requestData->setBorderColor(Settings::BackgroundTertiary);
                    QHBoxLayout *requestLayout = new QHBoxLayout(requestData);
                    requestLayout->setContentsMargins(0, 0, 0, 0);
                    Label *requestText = new Label(nullptr);

                    QDateTime time = QDateTime::fromString(data["created_at"].toString(), Qt::ISODate).addMonths(1).toLocalTime();
                    int dayMod = (time.date().day() - 1) % 10;
                    QString requestStr = "You've recently requested a copy of your data. You can request again on " + QLocale(QLocale::English).monthName(time.date().month())
                        + " " + QString::number(time.date().day() - 1) + (dayMod == 1 ? "st " : dayMod == 2 ? "nd " : dayMod == 3 ? "rd " : "th ")
                        + QString::number(time.date().year());
                    requestText->setText(requestStr);
                    requestText->setTextColor(Settings::HeaderPrimary);
                    requestText->setBackgroundColor(Settings::DeprecatedCardBg);
                    requestText->setFixedSize(QFontMetrics(font).horizontalAdvance(requestStr), Settings::scale(40));

                    requestText->setFont(font);
                    requestLayout->addWidget(requestText, 0, Qt::AlignCenter);

                    layout->addWidget(requestData);
                }
                layout->addSpacing(Settings::scale(60));

                Widget *footer = new Widget(container);
                footer->setFixedHeight(Settings::scale(62));
                footer->setBackgroundColor(Settings::DeprecatedCardBg);
                footer->setBorderRadius(Settings::scale(5));
                footer->setBorderSize(Settings::scale(1));
                footer->setBorderColor(Settings::BackgroundTertiary);
                QHBoxLayout *footerLayout = new QHBoxLayout(footer);
                footerLayout->setContentsMargins(0, 0, 0, 0);
                Label *footerText = new Label("Check out our Terms of Service (https://discord.com/terms) and Privacy Policy (https://discord.com/privacy)", nullptr);
                footerText->setFont(font);
                footerText->setBackgroundColor(Settings::None);
                footerText->setTextColor(Settings::HeaderPrimary);
                footerText->setFixedSize(QFontMetrics(font).horizontalAdvance("Check out our Terms of Service (https://discord.com/terms) and Privacy Policy (https://discord.com/privacy)"), Settings::scale(20));
                footerLayout->addWidget(footerText, 0, Qt::AlignCenter);

                layout->addWidget(footer);
                layout->addSpacing(Settings::scale(60));
            });
        });
    });

    this->setWidgetResizable(true);
    this->setWidget(container);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setStyleSheet("* {border: none; background-color: " + Settings::colors[Settings::BackgroundPrimary].name() + "}"
                        "QScrollBar::handle:vertical {border: none; border-radius: " + QString::number(Settings::scale(2)) + "px; background-color: " + Settings::colors[Settings::BackgroundTertiary].name() + ";}"
                        "QScrollBar:vertical {border: none; background-color: " + Settings::colors[Settings::BackgroundSecondary].name() + "; border-radius: " + QString::number(Settings::scale(8)) + "px; width: " + QString::number(Settings::scale(3)) + "px;}"
                        "QScrollBar::add-line, QScrollBar::sub-line {border:none; background: none; height: 0;}"
                        "QScrollBar:left-arrow:vertical, QScrollBar::right-arrow:vertical {background: none;}"
                        "QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {background: none;}");
}

Label *PrivacySafety::createTitle(QString text)
{
    QFont font;
    font.setPixelSize(Settings::scale(11));
    font.setBold(true);
    font.setFamily("whitney");

    Label *title = new Label(text, container);
    title->setFont(font);
    title->setTextColor(Settings::HeaderSecondary);
    title->setMaximumHeight(Settings::scale(20));

    return title;
}

QWidget *PrivacySafety::createSection(std::function<void(bool)> callback, QString titleStr, QString descriptionStr, optbool active, SectionType type)
{
    QFont font;
    font.setPixelSize(Settings::scale(14));
    font.setFamily("whitney");

    QWidget *section = new QWidget(container);
    QVBoxLayout *layout = new QVBoxLayout(section);
    layout->setSpacing(Settings::scale(8));
    layout->setContentsMargins(0, 0, 0, 0);


    QWidget *header = new QWidget(section);
    QHBoxLayout *headerLayout = new QHBoxLayout(header);
    headerLayout->setSpacing(0);
    headerLayout->setContentsMargins(0, 0, 0, 0);

    Label *title = new Label(titleStr, header);
    title->setFixedSize(QFontMetrics(font).horizontalAdvance(titleStr), Settings::scale(24));
    title->setFont(font);
    title->setTextColor(Settings::HeaderPrimary);

    headerLayout->addWidget(title);
    headerLayout->addStretch(1);

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
        headerLayout->addWidget(button);
    }

    layout->addWidget(header);


    if (!descriptionStr.isNull()) {
        font.setPixelSize(Settings::scale(12));
        Label *description = new Label(descriptionStr, section);
        description->setFont(font);
        description->setTextColor(Settings::HeaderSecondary);
        description->setFixedSize(QFontMetrics(font).horizontalAdvance(descriptionStr), Settings::scale(20) * (descriptionStr.count('\n') + 1));

        layout->addWidget(description);
    }

    layout->addSpacing(Settings::scale(12));

    if (titleStr != "Request all of my Data") {
        Widget *line = new Widget(section);
        line->setFixedHeight(Settings::scale(1));
        line->setBackgroundColor(Settings::BackgroundModifierAccent);
        layout->addWidget(line);
    }

    return section;
}

} // namespace Ui