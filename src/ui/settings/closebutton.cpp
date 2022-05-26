#include "ui/settings/closebutton.h"

#include <QVBoxLayout>
#include <QPainter>
#include <QPixmap>

namespace Ui {

CloseButton::CloseButton(QWidget *parent)
    : QWidget(parent)
{
    pressed = false;

    QWidget *mainContainer = new QWidget(this);
    QWidget *container = new QWidget(mainContainer);

    iconContainer = new QWidget(container);
    iconContainer->move(0, 60);
    iconContainer->setFixedSize(36, 36);
    iconContainer->setStyleSheet("border-radius: 18px;"
                                 "border: 2px solid #B9BBBE;");
    icon = new QLabel(iconContainer);
    icon->move(7, 7);
    icon->setFixedSize(18, 18);
    icon->setStyleSheet("border: none");
    QPixmap img("res/images/svg/close-settings-icon.svg");
    QPainter qp(&img);
    qp.setCompositionMode(QPainter::CompositionMode_SourceIn);
    qp.fillRect(img.rect(),QColor(185, 187, 190));
    qp.end();
    icon->setPixmap(img.scaled(18, 18));

    QFont font;
    font.setBold(true);
    font.setPixelSize(13);
    font.setFamily("whitney");
    text = new QLabel("  ESC", container);
    text->setFont(font);
    text->setFixedSize(36, 13);
    text->move(0, 104);
    text->setStyleSheet("color: #B9BBBE;");

    QWidget *spacer = new QWidget(this);
    spacer->setFixedHeight(52);

    QVBoxLayout *layout = new QVBoxLayout(container);
    layout->addWidget(spacer);
    layout->addWidget(iconContainer);
    layout->addWidget(text);
    layout->setSpacing(8);
    layout->setContentsMargins(0, 0, 0, 0);

    container->setFixedSize(36, 117);
    container->setStyleSheet("background-color: #36393F;");

    mainContainer->setFixedSize(57, 117);
    mainContainer->setStyleSheet("background-color: #36393F;");
    
    this->setFixedSize(57, 117);
    this->setStyleSheet("background-color: #36393F;");
}

void CloseButton::mouseReleaseEvent(QMouseEvent *event)
{
    pressed = false;
    text->setStyleSheet("color: #B9BBBE;");
    iconContainer->setStyleSheet("border-radius: 18px;"
                                 "border: 2px solid #B9BBBE;");
    QPixmap img("res/images/svg/close-settings-icon.svg");
    QPainter qp(&img);
    qp.setCompositionMode(QPainter::CompositionMode_SourceIn);
    qp.fillRect(img.rect(),QColor(185, 187, 190));
    qp.end();
    icon->setPixmap(img.scaled(18, 18));
    if (event->button() == Qt::LeftButton) {
        emit clicked();
    }
}

void CloseButton::mousePressEvent(QMouseEvent *event)
{
    pressed = true;
    text->setStyleSheet("color: #FFF;");
    iconContainer->setStyleSheet("border-radius: 18px;"
                                 "border: 2px solid #FFF;");
    QPixmap img("res/images/svg/close-settings-icon.svg");
    QPainter qp(&img);
    qp.setCompositionMode(QPainter::CompositionMode_SourceIn);
    qp.fillRect(img.rect(),QColor(255, 255, 255));
    qp.end();
    icon->setPixmap(img.scaled(18, 18));
}

void CloseButton::enterEvent(QEvent *)
{
    if (!pressed) {
        text->setStyleSheet("color: #DCDDDE;");
        iconContainer->setStyleSheet("border-radius: 18px;"
                                     "border: 2px solid #DCDDDE;");
        QPixmap img("res/images/svg/close-settings-icon.svg");
        QPainter qp(&img);
        qp.setCompositionMode(QPainter::CompositionMode_SourceIn);
        qp.fillRect(img.rect(),QColor(220, 221, 222));
        qp.end();
        icon->setPixmap(img.scaled(18, 18));
    }
}

void CloseButton::leaveEvent(QEvent *)
{
    if (!pressed) {
        text->setStyleSheet("color: #B9BBBE;");
        iconContainer->setStyleSheet("border-radius: 18px;"
                                     "border: 2px solid #B9BBBE;");
        QPixmap img("res/images/svg/close-settings-icon.svg");
        QPainter qp(&img);
        qp.setCompositionMode(QPainter::CompositionMode_SourceIn);
        qp.fillRect(img.rect(),QColor(185, 187, 190));
        qp.end();
        icon->setPixmap(img.scaled(18, 18));
    }
}

} // namespace Ui