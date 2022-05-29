#include "ui/settings/popup.h"

#include "ui/settings/settingsbutton.h"

#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QPainter>
#include <QGraphicsOpacityEffect>

namespace Ui {

class PopUpCloseButton : public QLabel
{
    Q_OBJECT
public:
    PopUpCloseButton(QWidget *parent) : QLabel(parent)
    {
        this->setFixedSize(24, 24);
        this->setPixmap(QPixmap("res/images/svg/close-settings-icon.svg"));
    }

signals:
    void clicked();

private:
    void mouseReleaseEvent(QMouseEvent *event) override
    {
        if (event->button() == Qt::LeftButton)
            emit clicked();
    }
    void enterEvent(QEvent *) override
    {
        QPixmap img("res/images/svg/close-settings-icon.svg");
        QPainter qp(&img);
        qp.setCompositionMode(QPainter::CompositionMode_SourceIn);
        qp.fillRect(img.rect(),QColor(220, 221, 222));
        qp.end();
        this->setPixmap(img);
    }
    void leaveEvent(QEvent *) override
    {
        QPixmap img("res/images/svg/close-settings-icon.svg");
        QPainter qp(&img);
        qp.setCompositionMode(QPainter::CompositionMode_SourceIn);
        qp.fillRect(img.rect(),QColor(185, 187, 190));
        qp.end();
        this->setPixmap(img);
    }
};

PopUp::PopUp(QWidget *content, int maximumWidth, int maximumHeight, QString icon, 
    QString title, bool titleCentered, bool closeable, QString description,
    QString cancelButtonStr, QString doneButtonStr, bool controlButtons, 
    bool horizontalButtons, QSize winSize, QWidget *parent) : QWidget(parent)
{
    this->setAttribute(Qt::WA_StyledBackground);
    this->setStyleSheet("background-color: rgba(0, 0, 0, 0.85);");
    this->setMinimumSize(winSize);

    QHBoxLayout *layout = new QHBoxLayout(this);

    actualPopup = new QLabel(this);
    actualPopup->setFixedSize(maximumWidth, maximumHeight);
    actualPopup->setStyleSheet("border-radius: 4px;"
                               "background-color: #36393F;");

    QVBoxLayout *popUpLayout = new QVBoxLayout(actualPopup);
    popUpLayout->setSpacing(16);
    popUpLayout->setContentsMargins(0, 0, 0, 0);

    QWidget *header = new QWidget(actualPopup);
    header->setContentsMargins(20, 12, 16, 0);
    QHBoxLayout *headerLayout = new QHBoxLayout(header);
    
    QFont font;
    font.setPixelSize(24);
    font.setFamily("whitney");
    font.setBold(true);
    QLabel *titleWidget;
    if (titleCentered || !description.isNull()) {
        titleWidget = new QLabel(header);
        QVBoxLayout *titleLayout = new QVBoxLayout(titleWidget);
        titleLayout->setContentsMargins(0, 0, 0, 0);
        titleLayout->setSpacing(8);
        QLabel *titleCenteredWidget = new QLabel(title, titleWidget);
        titleCenteredWidget->setFont(font);
        titleLayout->addWidget(titleCenteredWidget, 0, Qt::AlignHCenter);
        if (!description.isNull()) {
            font.setBold(false);
            font.setPixelSize(16);
            QTextEdit *descriptionWidget = new QTextEdit(titleWidget);
            descriptionWidget->setHtml(description);
            descriptionWidget->setReadOnly(true);
            descriptionWidget->setFont(font);
            descriptionWidget->setFixedWidth(maximumWidth - 60);
            descriptionWidget->viewport()->setCursor(Qt::CursorShape::ArrowCursor);
            titleLayout->addWidget(descriptionWidget, 0, Qt::AlignHCenter);
        }
    } else {
        titleWidget = new QLabel(title, actualPopup);
        titleWidget->setFont(font);
    }
    headerLayout->addWidget(titleWidget);

    if (closeable) {
        PopUpCloseButton *closeButton = new PopUpCloseButton(header);
        QObject::connect(closeButton, &PopUpCloseButton::clicked, [this](){emit cancelled();});
        headerLayout->addWidget(closeButton, 0, Qt::AlignTop);
    }

    popUpLayout->addWidget(header);
    popUpLayout->addWidget(content);
    
    if (controlButtons) {
        QWidget *footer = new QWidget(actualPopup);
        footer->setFixedHeight(horizontalButtons ? 70 : 116);
        footer->setContentsMargins(16, horizontalButtons ? 16 : 24, 16, 16);

        SettingsButton *doneButton = new SettingsButton(SettingsButton::Type::Normal, doneButtonStr, footer);
        SettingsButton *cancelButton = new SettingsButton(SettingsButton::Type::NoBackground, cancelButtonStr, footer);
        QObject::connect(doneButton, &SettingsButton::clicked, [this](){emit done();});
        QObject::connect(cancelButton, &SettingsButton::clicked, [this](){emit cancelled();});
        
        if (horizontalButtons) {
            footer->setStyleSheet("background-color: #2F3136");
            cancelButton->setFixedSize(96, 38);
            doneButton->setFixedSize(96, 38);
            QHBoxLayout *footerLayout = new QHBoxLayout(footer);
            footerLayout->setContentsMargins(0, 0, 0, 0);
            footerLayout->setSpacing(0);
            footerLayout->addStretch(1);
            footerLayout->addWidget(cancelButton);
            footerLayout->addWidget(doneButton);
        } else {
            cancelButton->setMinimumSize(0, 0);
            cancelButton->setMaximumWidth(408);
            cancelButton->setFixedHeight(34);
            doneButton->setMinimumSize(0, 0);
            doneButton->setMaximumWidth(408);
            doneButton->setFixedHeight(34);
            QVBoxLayout *footerLayout = new QVBoxLayout(footer);
            footerLayout->setContentsMargins(0, 0, 0, 0);
            footerLayout->setSpacing(0);
            footerLayout->addStretch(1);
            footerLayout->addWidget(doneButton);
            footerLayout->addWidget(cancelButton);
            footerLayout->addStretch(1);
        }
        popUpLayout->addWidget(footer);
    }
    layout->addWidget(actualPopup, 0, Qt::AlignCenter);

    this->show();
}

void PopUp::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && 
        !QRect(actualPopup->pos().x(), actualPopup->pos().y(), actualPopup->width(), actualPopup->height()).contains(event->pos()))
        emit cancelled();
}

} // namespace Ui

#include "popup.moc"