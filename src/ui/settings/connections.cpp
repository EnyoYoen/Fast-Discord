#include "ui/settings/connections.h"

#include "ui/common/basicwidgets.h"
#include "ui/settings/switchbutton.h"
#include "ui/settings/popup.h"
#include "api/objects/connection.h"

#include <QPainter>
#include <QPixmap>

namespace Ui {

class AccountButton : public Widget
{
public:
    AccountButton(Api::RessourceManager *rmp, QString type, QWidget *parent)
        : Widget(parent)
    {
        rm = rmp;
        this->setFixedSize(48, 48);
        this->setBorderRadius(5);
        this->setBackgroundColor(Settings::BackgroundSecondary);
        this->setImage("res/images/svg/" + type + "-icon.svg");
    }

    static QMap<QString, QString> names;

private:
    Api::RessourceManager *rm;

    void mouseReleaseEvent(QMouseEvent *) override
    {
        // TODO
    }
    void enterEvent(QEvent *) override
    {
        this->setBackgroundColor(Settings::BackgroundPrimary);
    }
    void leaveEvent(QEvent *) override
    {
        this->setBackgroundColor(Settings::BackgroundSecondary);
    }
};
QMap<QString, QString> AccountButton::names = QMap<QString, QString>({
    {"battle.net", "Battle.net"},
    {"facebook", "Facebook"},
    {"github", "GitHub"},
    {"playstation network", "PlayStation Network"},
    {"reddit", "Reddit"},
    {"spotify", "Spotify"},
    {"steam", "Steam"},
    {"twitch", "Twitch"},
    {"twitter", "Twitter"},
    {"xbox", "Xbox"},
    {"youtube", "YouTube"}
});

class RemoveAccountButton : public Widget
{
    Q_OBJECT
public:
    RemoveAccountButton(Api::RessourceManager *rmp, QWidget *parent)
        : Widget(parent)
    {
        pressed = false;
        rm = rmp;
        this->setFixedSize(16, 16);
        this->setBackgroundColor(Settings::BackgroundSecondaryAlt);
        QPixmap img("res/images/svg/close-icon.svg");
        QPainter qp(&img);
        qp.setCompositionMode(QPainter::CompositionMode_SourceIn);
        qp.fillRect(img.rect(), Settings::colors[Settings::InteractiveNormal]);
        qp.end();
        this->setPixmap(img.scaled(16, 16));
    }

signals:
    void clicked();

private:
    Api::RessourceManager *rm;
    bool pressed;

    void mouseReleaseEvent(QMouseEvent *event)
    {
        QPixmap img("res/images/svg/close-icon.svg");
        QPainter qp(&img);
        qp.setCompositionMode(QPainter::CompositionMode_SourceIn);
        qp.fillRect(img.rect(), Settings::colors[Settings::InteractiveNormal]);
        qp.end();
        this->setPixmap(img.scaled(16, 16));
        emit clicked();
    }

    void mousePressEvent(QMouseEvent *event)
    {
        pressed = true;
        QPixmap img("res/images/svg/close-icon.svg");
        QPainter qp(&img);
        qp.setCompositionMode(QPainter::CompositionMode_SourceIn);
        qp.fillRect(img.rect(), Settings::colors[Settings::InteractiveActive]);
        qp.end();
        this->setPixmap(img.scaled(16, 16));
    }

    void enterEvent(QEvent *)
    {
        if (!pressed) {
            QPixmap img("res/images/svg/close-icon.svg");
            QPainter qp(&img);
            qp.setCompositionMode(QPainter::CompositionMode_SourceIn);
            qp.fillRect(img.rect(), Settings::colors[Settings::InteractiveHover]);
            qp.end();
            this->setPixmap(img.scaled(16, 16));
        }
    }

    void leaveEvent(QEvent *)
    {
        if (!pressed) {
            QPixmap img("res/images/svg/close-icon.svg");
            QPainter qp(&img);
            qp.setCompositionMode(QPainter::CompositionMode_SourceIn);
            qp.fillRect(img.rect(), Settings::colors[Settings::InteractiveNormal]);
            qp.end();
            this->setPixmap(img.scaled(16, 16));
        }
    }
};

class ConnectedAccount : public Widget
{
    Q_OBJECT
public:
    ConnectedAccount(Api::RessourceManager *rm, Api::Connection *connection, QWidget *parent)
        : Widget(parent)
    {
        this->setFixedHeight(136);
        this->setBorderRadius(8);

        QVBoxLayout *layout = new QVBoxLayout(this);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(0);

        Widget *header = new Widget(this);
        header->setFixedHeight(72);
        header->setBackgroundColor(Settings::BackgroundSecondaryAlt);
        header->setBorderRadius(8, 8, 0, 0);
        QHBoxLayout *headerLayout = new QHBoxLayout(header);
        headerLayout->setContentsMargins(20, 20, 20, 20);
        headerLayout->setSpacing(0);

        Widget *icon = new Widget(header);
        icon->setFixedSize(32, 32);
        icon->setImage("res/images/svg/" + connection->type + "-icon.svg");
        
        Widget *nameContainer = new Widget(header);
        nameContainer->setFixedHeight(32);
        QVBoxLayout *nameLayout = new QVBoxLayout(nameContainer);
        nameLayout->setContentsMargins(0, 0, 0, 0);
        nameLayout->setSpacing(0);

        QFont font;
        font.setPixelSize(14);
        font.setFamily("whitney");
        Label *username = new Label(connection->name, nameContainer);
        username->setFixedSize(QFontMetrics(font).horizontalAdvance(connection->name), 18);
        username->setFont(font);
        username->setTextColor(Settings::HeaderPrimary);
        Label *name = new Label((AccountButton::names.contains(connection->type) ? AccountButton::names[connection->type] : connection->type), nameContainer);
        font.setPixelSize(12);
        name->setFixedHeight(14);
        name->setFixedSize(QFontMetrics(font).horizontalAdvance(connection->name), 14);
        name->setFont(font);
        name->setTextColor(Settings::HeaderSecondary);

        nameLayout->addWidget(username);
        nameLayout->addWidget(name);

        RemoveAccountButton *button = new RemoveAccountButton(rm, header);
        QObject::connect(button, &RemoveAccountButton::clicked, [rm, connection, this](){
            QWidget *parentWidget = this;
            while (parentWidget->parent()) parentWidget = (QWidget *)parentWidget->parent();
            PopUp *popUp = new PopUp(new Widget(nullptr), 440, 210, QString(), "DISCONNECT " + connection->type.toUpper(), true, false, "Disconnecting your account might remove you from servers\nyou joined via this account.", "Cancel", "Disconnect", true, true, parentWidget->size(), parentWidget);
            QObject::connect(popUp, &PopUp::cancelled, [popUp](){popUp->deleteLater();});
            QObject::connect(popUp, &PopUp::done, [popUp, connection, rm, this](){
                rm->requester->removeConnection(connection->type, connection->id);
                emit removed();
                popUp->deleteLater();
                this->deleteLater();
            });
        });

        headerLayout->addWidget(icon);
        headerLayout->addSpacing(20);
        headerLayout->addWidget(nameContainer);
        headerLayout->addStretch(1);
        headerLayout->addWidget(button, 0, Qt::AlignVCenter);


        Widget *switchContainer = new Widget(this);
        switchContainer->setBackgroundColor(Settings::BackgroundSecondary);
        switchContainer->setBorderRadius(0, 0, 8, 8);
        switchContainer->setFixedHeight(64);
        QHBoxLayout *switchLayout = new QHBoxLayout(switchContainer);
        switchLayout->setContentsMargins(20, 20, 20, 20);
        switchLayout->setSpacing(0);

        font.setPixelSize(16);
        Label *description = new Label("Display on profile", switchContainer);
        description->setFixedSize(QFontMetrics(font).horizontalAdvance("Display on profile"), 24);
        description->setFont(font);
        description->setTextColor(Settings::HeaderPrimary);

        SwitchButton *switchButton = new SwitchButton(connection->visibility);
        QObject::connect(switchButton, &SwitchButton::clicked, [rm, connection](bool state){
            rm->requester->setConnection(connection->type, connection->id, (int)state);
        });

        switchLayout->addWidget(description);
        switchLayout->addStretch(1);
        switchLayout->addWidget(switchButton);

        layout->addWidget(header);
        layout->addWidget(switchContainer);
    }

signals:
    void removed();
};

Connections::Connections(Api::RessourceManager *rmp, QWidget *parent)
    : QScrollArea()
{
    rm = rmp;

    Widget *container = new Widget(this);
    container->setMaximumWidth(740);
    container->setContentsMargins(40, 60, 40, 80);
    container->setBackgroundColor(Settings::BackgroundPrimary);
    layout = new QVBoxLayout(container);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(8);

    QFont font;
    font.setPixelSize(20);
    font.setFamily("whitney");

    Label *title = new Label("Connections", container);
    title->setFixedSize(QFontMetrics(font).horizontalAdvance("Connections"), 20);
    title->setFont(font);
    title->setTextColor(Settings::HeaderPrimary);

    layout->addWidget(title);
    layout->addSpacing(20);


    Widget *accountList = new Widget(container);
    accountList->setMinimumHeight(134);
    accountList->setBackgroundColor(Settings::BackgroundSecondaryAlt);
    accountList->setBorderRadius(8);
    QVBoxLayout *accountLayout = new QVBoxLayout(accountList);
    accountLayout->setContentsMargins(20, 20, 20, 20);
    accountLayout->setSpacing(0);

    font.setPixelSize(12);
    font.setBold(true);
    Label *accountTitle = new Label("CONNECT YOUR ACCOUNTS", accountList);
    accountTitle->setFont(font);
    accountTitle->setTextColor(Settings::HeaderSecondary);

    font.setPixelSize(14);
    font.setBold(false);
    Label *description = new Label("Connect these accounts and unlock special Discord integrations.", accountList);
    description->setFont(font);
    description->setTextColor(Settings::TextNormal);

    Widget *accountGrid = new Widget(accountList);
    QHBoxLayout *accountGridLayout = new QHBoxLayout(accountGrid);
    accountGridLayout->setContentsMargins(0, 0, 0, 8);
    accountGridLayout->setSpacing(0);
    QList<QString> keys = AccountButton::names.keys();
    accountGridLayout->addWidget(new AccountButton(rm, keys.at(0), accountGrid));
    for (int i = 1 ; i < keys.size() ; i++) {
        accountGridLayout->addSpacing(8);
        accountGridLayout->addWidget(new AccountButton(rm, keys.at(i), accountGrid));
    }

    accountLayout->addWidget(accountTitle);
    accountLayout->addSpacing(4);
    accountLayout->addWidget(description);
    accountLayout->addSpacing(8);
    accountLayout->addWidget(accountGrid);

    layout->addWidget(accountList);
    layout->addSpacing(20);

    rm->requester->getConnections([container, this](void *connectionsPtr){
        QVector<Api::Connection *> connections = *reinterpret_cast<QVector<Api::Connection *> *>(connectionsPtr);
        if (connections.size() == 0) {
            layout->addStretch(1);
            empty();
        } else {
            for (int i = 0 ; i < connections.size() ; i++) {
                ConnectedAccount *account = new ConnectedAccount(rm, connections[i], container);
                QObject::connect(account, &ConnectedAccount::removed, [this](){
                    if (layout->count() <= 3)
                        empty();
                });
                layout->insertWidget(3 + i, account);
                layout->insertSpacing(4 + i, 30);
            }
            layout->addStretch(1);
        }
    });


    this->setWidgetResizable(true);
    this->setWidget(container);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setStyleSheet("* {border: none; background-color: " + Settings::colors[Settings::BackgroundPrimary].name() + "}"
                        "QScrollBar::handle:vertical {border: none; border-radius: 2px; background-color: " + Settings::colors[Settings::BackgroundTertiary].name() + ";}"
                        "QScrollBar:vertical {border: none; background-color: " + Settings::colors[Settings::BackgroundSecondary].name() + "; border-radius: 8px; width: 3px;}"
                        "QScrollBar::add-line, QScrollBar::sub-line {border:none; background: none; height: 0;}"
                        "QScrollBar:left-arrow:vertical, QScrollBar::right-arrow:vertical {background: none;}"
                        "QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {background: none;}");
}

void Connections::empty()
{
    Widget *noConnections = new Widget();
    QVBoxLayout *noConnectionsLayout = new QVBoxLayout(noConnections);
    noConnectionsLayout->setContentsMargins(0, 0, 0, 0);
    noConnectionsLayout->setSpacing(0);
    
    Widget *image = new Widget(noConnections);
    image->setFixedSize(262, 226);
    image->setImage("res/images/svg/no-connections-icon.svg");

    noConnectionsLayout->addWidget(image, 0, Qt::AlignHCenter);
    noConnectionsLayout->addSpacing(40);

    QFont font;
    font.setPixelSize(17);
    font.setBold(true);
    font.setFamily("whitney");

    Label *title = new Label("NO CONNECTIONS", noConnections);
    title->setFixedSize(QFontMetrics(font).horizontalAdvance("NO CONNECTIONS"), 22);
    title->setTextColor(Settings::TextMuted);
    title->setFont(font);

    noConnectionsLayout->addWidget(title, 0, Qt::AlignHCenter);
    noConnectionsLayout->addSpacing(8);
    
    font.setBold(false);
    Label *desc = new Label("Connect your account to unlock special Discord integrations", noConnections);
    desc->setFixedSize(QFontMetrics(font).horizontalAdvance("Connect your account to unlock special Discord integrations"), 20);
    desc->setTextColor(Settings::TextMuted);
    desc->setFont(font);

    noConnectionsLayout->addWidget(desc, 0, Qt::AlignHCenter);

    layout->insertWidget(4, noConnections);
}

} // namespace Ui

#include "connections.moc"