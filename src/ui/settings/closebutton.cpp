#include "ui/settings/closebutton.h"

#include <QVBoxLayout>
#include <QPainter>
#include <QPixmap>

namespace Ui {

CloseButton::CloseButton(QWidget *parent)
    : Widget(parent)
{
    pressed = false;

    Widget *mainContainer = new Widget(this);
    Widget *container = new Widget(mainContainer);

    iconContainer = new Widget(container);
    iconContainer->move(0, Settings::scale(60));
    iconContainer->setFixedSize(Settings::scale(36), Settings::scale(36));
    iconContainer->setBorderRadius(Settings::scale(18));
    iconContainer->setBorderSize(Settings::scale(2));
    iconContainer->setBorderColor(Settings::InteractiveNormal);
    icon = new Widget(iconContainer);
    icon->move(Settings::scale(7), Settings::scale(7));
    icon->setFixedSize(Settings::scale(18), Settings::scale(18));
    QPixmap img("res/images/svg/close-settings-icon.svg");
    QPainter qp(&img);
    qp.setCompositionMode(QPainter::CompositionMode_SourceIn);
    qp.fillRect(img.rect(), Settings::colors[Settings::InteractiveNormal]);
    qp.end();
    icon->setPixmap(img.scaled(Settings::scale(18), Settings::scale(18)));

    QFont font;
    font.setBold(true);
    font.setPixelSize(Settings::scale(13));
    font.setFamily("whitney");
    text = new Label("  ESC", container);
    text->setFont(font);
    text->setFixedSize(Settings::scale(36), Settings::scale(13));
    text->move(0, Settings::scale(104));
    text->setTextColor(Settings::InteractiveNormal);

    QWidget *spacer = new QWidget(this);
    spacer->setFixedHeight(Settings::scale(52));

    QVBoxLayout *layout = new QVBoxLayout(container);
    layout->addWidget(spacer);
    layout->addWidget(iconContainer);
    layout->addWidget(text);
    layout->setSpacing(Settings::scale(8));
    layout->setContentsMargins(0, 0, 0, 0);

    container->setFixedSize(Settings::scale(36), Settings::scale(117));
    container->setBackgroundColor(Settings::BackgroundPrimary);

    mainContainer->setFixedSize(Settings::scale(57), Settings::scale(117));
    mainContainer->setBackgroundColor(Settings::BackgroundPrimary);
    
    this->setFixedSize(Settings::scale(57), Settings::scale(117));
    this->setBackgroundColor(Settings::BackgroundPrimary);
}

void CloseButton::mouseReleaseEvent(QMouseEvent *event)
{
    pressed = false;
    text->setTextColor(Settings::InteractiveNormal);
    iconContainer->setBorderColor(Settings::InteractiveNormal);
    QPixmap img("res/images/svg/close-settings-icon.svg");
    QPainter qp(&img);
    qp.setCompositionMode(QPainter::CompositionMode_SourceIn);
    qp.fillRect(img.rect(), Settings::colors[Settings::InteractiveNormal]);
    qp.end();
    icon->setPixmap(img.scaled(Settings::scale(18), Settings::scale(18)));
    if (event->button() == Qt::LeftButton) {
        emit clicked();
    }
}

void CloseButton::mousePressEvent(QMouseEvent *event)
{
    pressed = true;
    text->setTextColor(Settings::InteractiveActive);
    iconContainer->setBorderColor(Settings::InteractiveActive);
    QPixmap img("res/images/svg/close-settings-icon.svg");
    QPainter qp(&img);
    qp.setCompositionMode(QPainter::CompositionMode_SourceIn);
    qp.fillRect(img.rect(), Settings::colors[Settings::InteractiveActive]);
    qp.end();
    icon->setPixmap(img.scaled(Settings::scale(18), Settings::scale(18)));
}

void CloseButton::enterEvent(QEvent *)
{
    if (!pressed) {
        text->setTextColor(Settings::InteractiveHover);
        iconContainer->setBorderColor(Settings::InteractiveHover);
        QPixmap img("res/images/svg/close-settings-icon.svg");
        QPainter qp(&img);
        qp.setCompositionMode(QPainter::CompositionMode_SourceIn);
        qp.fillRect(img.rect(), Settings::colors[Settings::InteractiveHover]);
        qp.end();
        icon->setPixmap(img.scaled(Settings::scale(18), Settings::scale(18)));
    }
}

void CloseButton::leaveEvent(QEvent *)
{
    if (!pressed) {
        text->setTextColor(Settings::InteractiveNormal);
        iconContainer->setBorderColor(Settings::InteractiveNormal);
        QPixmap img("res/images/svg/close-settings-icon.svg");
        QPainter qp(&img);
        qp.setCompositionMode(QPainter::CompositionMode_SourceIn);
        qp.fillRect(img.rect(), Settings::colors[Settings::InteractiveNormal]);
        qp.end();
        icon->setPixmap(img.scaled(Settings::scale(18), Settings::scale(18)));
    }
}

} // namespace Ui