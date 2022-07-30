#include "ui/settings/popup.h"

#include "ui/settings/settingsbutton.h"

#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QPainter>
#include <QGraphicsOpacityEffect>

namespace Ui {

class PopUpCloseButton : public Widget
{
    Q_OBJECT
public:
    PopUpCloseButton(QWidget *parent) : Widget(parent)
    {
        this->setFixedSize(Settings::scale(24), Settings::scale(24));
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

PopUp::PopUp(Widget *content, int maximumWidth, int maximumHeight, QString icon, 
    QString title, bool titleCentered, bool closeable, QString description,
    QString cancelButtonStr, QString doneButtonStr, bool controlButtons, 
    bool horizontalButtons, QSize winSize, QWidget *parent) : Widget(parent)
{
    this->setAttribute(Qt::WA_StyledBackground);
    this->setBackgroundColor(Settings::Black85);
    this->setMinimumSize(winSize);

    QHBoxLayout *layout = new QHBoxLayout(this);

    actualPopup = new Widget(this);
    actualPopup->setFixedSize(maximumWidth, maximumHeight);
    actualPopup->setBorderRadius(Settings::scale(4));
    actualPopup->setBackgroundColor(Settings::BackgroundPrimary);

    QVBoxLayout *popUpLayout = new QVBoxLayout(actualPopup);
    popUpLayout->setSpacing(Settings::scale(16));
    popUpLayout->setContentsMargins(0, 0, 0, 0);

    Widget *header = new Widget(actualPopup);
    header->setContentsMargins(Settings::scale(20), Settings::scale(12), Settings::scale(16), 0);
    header->setBackgroundColor(Settings::None);
    QHBoxLayout *headerLayout = new QHBoxLayout(header);
    
    QFont font;
    font.setPixelSize(Settings::scale(24));
    font.setFamily("whitney");
    font.setBold(true);
    Label *titleWidget;
    if (titleCentered || !description.isNull()) {
        titleWidget = new Label(header);
        QVBoxLayout *titleLayout = new QVBoxLayout(titleWidget);
        titleLayout->setContentsMargins(0, 0, 0, 0);
        titleLayout->setSpacing(Settings::scale(8));
        Label *titleCenteredWidget = new Label(title, titleWidget);
        titleCenteredWidget->setTextColor(Settings::HeaderPrimary);
        titleCenteredWidget->setFont(font);
        titleCenteredWidget->setFixedSize(QFontMetrics(font).horizontalAdvance(title), Settings::scale(30));
        titleCenteredWidget->setFlags(Qt::AlignCenter);
        titleLayout->addWidget(titleCenteredWidget, 0, Qt::AlignHCenter);
        if (!description.isNull()) {
            font.setBold(false);
            font.setPixelSize(Settings::scale(16));
            QTextEdit *descriptionWidget = new QTextEdit(titleWidget);
            descriptionWidget->setHtml(description);
            descriptionWidget->setReadOnly(true);
            descriptionWidget->setFont(font);
            descriptionWidget->setFixedWidth(maximumWidth - Settings::scale(60));
            descriptionWidget->viewport()->setCursor(Qt::CursorShape::ArrowCursor);
            descriptionWidget->setStyleSheet("border:none;background-color:" + Settings::colors[Settings::BackgroundPrimary].name() + ";color:" + Settings::colors[Settings::HeaderSecondary].name());
            titleLayout->addWidget(descriptionWidget, 0, Qt::AlignHCenter);
        }
    } else {
        titleWidget = new Label(title, actualPopup);
        titleWidget->setTextColor(Settings::HeaderPrimary);
        titleWidget->setFont(font);
        titleWidget->setFixedSize(QFontMetrics(font).horizontalAdvance(title), Settings::scale(30));
        titleWidget->setFlags(Qt::AlignCenter);
    }
    titleWidget->setBackgroundColor(Settings::None);
    headerLayout->addWidget(titleWidget);

    if (closeable) {
        PopUpCloseButton *closeButton = new PopUpCloseButton(header);
        QObject::connect(closeButton, &PopUpCloseButton::clicked, [this](){emit cancelled();});
        headerLayout->addWidget(closeButton, 0, Qt::AlignTop);
    }

    popUpLayout->addWidget(header);
    content->setBackgroundColor(Settings::None);
    popUpLayout->addWidget(content);
    
    if (controlButtons) {
        Widget *footer = new Widget(actualPopup);
        footer->setFixedHeight(horizontalButtons ? Settings::scale(70) : Settings::scale(116));
        footer->setContentsMargins(Settings::scale(16), horizontalButtons ? Settings::scale(16) : Settings::scale(24), Settings::scale(16), Settings::scale(16));

        SettingsButton *doneButton = new SettingsButton(SettingsButton::Type::Normal, doneButtonStr, footer);
        SettingsButton *cancelButton = new SettingsButton(SettingsButton::Type::NoBackground, cancelButtonStr, footer);
        QObject::connect(doneButton, &SettingsButton::clicked, [this](){emit done();});
        QObject::connect(cancelButton, &SettingsButton::clicked, [this](){emit cancelled();});
        
        if (horizontalButtons) {
            footer->setBackgroundColor(Settings::BackgroundSecondary);
            cancelButton->setFixedSize(Settings::scale(96), Settings::scale(38));
            doneButton->setFixedSize(Settings::scale(96), Settings::scale(38));
            QHBoxLayout *footerLayout = new QHBoxLayout(footer);
            footerLayout->setContentsMargins(0, 0, 0, 0);
            footerLayout->setSpacing(0);
            footerLayout->addStretch(1);
            footerLayout->addWidget(cancelButton);
            footerLayout->addWidget(doneButton);
        } else {
            cancelButton->setMinimumSize(0, 0);
            cancelButton->setMaximumWidth(Settings::scale(408));
            cancelButton->setFixedHeight(Settings::scale(34));
            doneButton->setMinimumSize(0, 0);
            doneButton->setMaximumWidth(Settings::scale(408));
            doneButton->setFixedHeight(Settings::scale(34));
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