#include "ui/settings/connections.h"

#include "ui/settings/switchbutton.h"
#include "ui/settings/popup.h"
#include "api/objects/connection.h"

#include <QPainter>
#include <QPixmap>

namespace Ui {

class AccountButton : public QLabel
{
public:
    AccountButton(Api::RessourceManager *rmp, QString typep, QWidget *parent)
        : QLabel(parent)
    {
        rm = rmp;
        type = typep;
        this->setFixedSize(48, 48);
        this->setStyleSheet("border-radius: 5px;"
                            "background-color: #2F3136;"
                            "background-repeat: no-repeat;"
                            "background-position: center;"
                            "background-image: url(\"res/images/svg/" + type + "-icon.svg\")");
    }

    static QMap<QString, QString> names;

private:
    Api::RessourceManager *rm;
    QString type;

    void mouseReleaseEvent(QMouseEvent *) override
    {
        // TODO
    }
    void enterEvent(QEvent *) override
    {
        this->setStyleSheet("border-radius: 5px;"
                            "background-color: #36393F;"
                            "background-repeat: no-repeat;"
                            "background-position: center;"
                            "background-image: url(\"res/images/svg/" + type + "-icon.svg\")");
    }
    void leaveEvent(QEvent *) override
    {
        this->setStyleSheet("border-radius: 5px;"
                            "background-color: #2F3136;"
                            "background-repeat: no-repeat;"
                            "background-position: center;"
                            "background-image: url(\"res/images/svg/" + type + "-icon.svg\")");
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

class RemoveAccountButton : public QLabel
{
    Q_OBJECT
public:
    RemoveAccountButton(Api::RessourceManager *rmp, QWidget *parent)
        : QLabel(parent)
    {
        pressed = false;
        rm = rmp;
        this->setFixedSize(16, 16);
        QPixmap img("res/images/svg/close-icon.svg");
        QPainter qp(&img);
        qp.setCompositionMode(QPainter::CompositionMode_SourceIn);
        qp.fillRect(img.rect(),QColor(185, 187, 190));
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
        qp.fillRect(img.rect(),QColor(185, 187, 190));
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
        qp.fillRect(img.rect(),QColor(255, 255, 255));
        qp.end();
        this->setPixmap(img.scaled(16, 16));
    }

    void enterEvent(QEvent *)
    {
        if (!pressed) {
            QPixmap img("res/images/svg/close-icon.svg");
            QPainter qp(&img);
            qp.setCompositionMode(QPainter::CompositionMode_SourceIn);
            qp.fillRect(img.rect(),QColor(220, 221, 222));
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
            qp.fillRect(img.rect(),QColor(185, 187, 190));
            qp.end();
            this->setPixmap(img.scaled(16, 16));
        }
    }
};

class ConnectedAccount : public QLabel
{
    Q_OBJECT
public:
    ConnectedAccount(Api::RessourceManager *rm, Api::Connection *connection, QWidget *parent)
        : QLabel(parent)
    {
        this->setFixedHeight(136);
        this->setStyleSheet("border-radius: 8px;"
                            "background-color: #2F3136");

        QVBoxLayout *layout = new QVBoxLayout(this);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(0);

        QLabel *header = new QLabel(this);
        header->setFixedHeight(72);
        header->setStyleSheet("background-color: #292B2F");
        QHBoxLayout *headerLayout = new QHBoxLayout(header);
        headerLayout->setContentsMargins(20, 20, 20, 20);
        headerLayout->setSpacing(0);

        QLabel *icon = new QLabel(header);
        icon->setFixedSize(32, 32);
        icon->setStyleSheet("background-repeat: no-repeat;"
                            "background-image: url(\"res/images/svg/" + connection->type + "-icon.svg\")");
        
        QWidget *nameContainer = new QWidget(header);
        nameContainer->setFixedHeight(32);
        QVBoxLayout *nameLayout = new QVBoxLayout(nameContainer);
        nameLayout->setContentsMargins(0, 0, 0, 0);
        nameLayout->setSpacing(0);

        QFont font;
        font.setPixelSize(14);
        font.setFamily("whitney");
        QLabel *username = new QLabel(connection->name, nameContainer);
        username->setFixedHeight(18);
        username->setFont(font);
        username->setStyleSheet("color: #FFF");
        QLabel *name = new QLabel((AccountButton::names.contains(connection->type) ? AccountButton::names[connection->type] : connection->type), nameContainer);
        font.setPixelSize(12);
        name->setFixedHeight(14);
        name->setFont(font);
        name->setStyleSheet("color: #B9BBBE");

        nameLayout->addWidget(username);
        nameLayout->addWidget(name);

        RemoveAccountButton *button = new RemoveAccountButton(rm, header);
        QObject::connect(button, &RemoveAccountButton::clicked, [rm, connection, this](){
            QWidget *parentWidget = this;
            while (parentWidget->parent()) parentWidget = (QWidget *)parentWidget->parent();
            PopUp *popUp = new PopUp(new QWidget(), 440, 210, QString(), "DISCONNECT " + connection->type.toUpper(), true, false, "Disconnecting your account might remove you from servers\nyou joined via this account.", "Cancel", "Disconnect", true, true, parentWidget->size(), parentWidget);
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


        QLabel *switchContainer = new QLabel(this);
        switchContainer->setFixedHeight(64);
        QHBoxLayout *switchLayout = new QHBoxLayout(switchContainer);
        switchLayout->setContentsMargins(20, 20, 20, 20);
        switchLayout->setSpacing(0);

        font.setPixelSize(16);
        QLabel *description = new QLabel("Display on profile", switchContainer);
        description->setFixedHeight(24);
        description->setFont(font);
        description->setStyleSheet("color: #FFF");

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

    QWidget *container = new QWidget(this);
    container->setMaximumWidth(740);
    container->setContentsMargins(40, 60, 40, 80);
    container->setStyleSheet("background-color: #36393F");
    layout = new QVBoxLayout(container);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(8);

    QFont font;
    font.setPixelSize(20);
    font.setFamily("whitney");

    QLabel *title = new QLabel("Connections", container);
    title->setFont(font);
    title->setStyleSheet("color: #FFF");

    layout->addWidget(title);
    layout->addSpacing(20);


    QLabel *accountList = new QLabel(container);
    accountList->setMinimumHeight(134);
    accountList->setStyleSheet("border-radius: 8px;"
                               "background-color: #292B2F");
    QVBoxLayout *accountLayout = new QVBoxLayout(accountList);
    accountLayout->setContentsMargins(20, 20, 20, 20);
    accountLayout->setSpacing(0);

    font.setPixelSize(12);
    font.setBold(true);
    QLabel *accountTitle = new QLabel("CONNECT YOUR ACCOUNTS", accountList);
    accountTitle->setFont(font);
    accountTitle->setStyleSheet("color: #B9BBBE");

    font.setPixelSize(14);
    font.setBold(false);
    QLabel *description = new QLabel("Connect these accounts and unlock special Discord integrations.", accountList);
    description->setFont(font);
    description->setStyleSheet("color: #DCDDDE");

    QWidget *accountGrid = new QWidget(accountList);
    QHBoxLayout *accountGridLayout = new QHBoxLayout(accountGrid);
    accountGridLayout->setContentsMargins(0, 0, 0, 8);
    accountGridLayout->setSpacing(0);
    QList<QString> keys = AccountButton::names.keys();
    accountGridLayout->addWidget(new AccountButton(rm, keys.at(0), accountGrid));
    for (unsigned int i = 1 ; i < keys.size() ; i++) {
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
            empty();
        } else {
            for (unsigned int i = 0 ; i < connections.size() ; i++) {
                ConnectedAccount *account = new ConnectedAccount(rm, connections[i], container);
                QObject::connect(account, &ConnectedAccount::removed, [this](){
                    if (layout->count() <= 2)
                        empty();
                });
                layout->insertWidget(3 + i, account);
                layout->insertSpacing(4 + i, 30);
            }
        }
    });

    layout->addStretch(1);


    this->setWidgetResizable(true);
    this->setWidget(container);
    this->setStyleSheet("QScrollBar::handle {border: none; border-radius: 2px; background-color: #202225;}"
                        "QScrollBar {border: none; background-color: #36393F; border-radius: 8px; width: 3px;}"
                        "QScrollBar::add-line, QScrollBar::sub-line {border:none; background: none; height: 0;}");
}

void Connections::empty()
{
    QWidget *noConnections = new QWidget();
    QVBoxLayout *noConnectionsLayout = new QVBoxLayout(noConnections);
    noConnectionsLayout->setContentsMargins(0, 0, 0, 0);
    noConnectionsLayout->setSpacing(0);
    
    QLabel *image = new QLabel(noConnections);
    image->setFixedSize(262, 226);
    image->setStyleSheet("background-image: url(\"res/images/svg/no-connections-icon.svg\")");

    noConnectionsLayout->addWidget(image, 0, Qt::AlignHCenter);
    noConnectionsLayout->addSpacing(40);

    QFont font;
    font.setPixelSize(17);
    font.setBold(true);
    font.setFamily("whitney");

    QLabel *title = new QLabel("NO CONNECTIONS", noConnections);
    title->setStyleSheet("color: #A3A6AA");
    title->setFont(font);

    noConnectionsLayout->addWidget(title, 0, Qt::AlignHCenter);
    noConnectionsLayout->addSpacing(8);
    
    font.setBold(false);
    QLabel *desc = new QLabel("Connect your account to unlock special Discord integrations", noConnections);
    desc->setStyleSheet("color: #A3A6AA");
    desc->setFont(font);

    noConnectionsLayout->addWidget(desc, 0, Qt::AlignHCenter);

    layout->insertWidget(3, noConnections);
}

} // namespace Ui

#include "connections.moc"