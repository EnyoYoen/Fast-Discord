#include "ui/leftcolumn/homebutton.h"

#include <QHBoxLayout>
#include <QPixmap>
#include <QPainter>
#include <QMouseEvent>

namespace Ui {

HomeButton::HomeButton(QWidget *parent)
    : QWidget(parent)
{
    // Attribute initialization (the home page is shown at the start)
    clic = true;

    QHBoxLayout *layout = new QHBoxLayout(this);
    image = new Widget(this);
    pill = new GuildPill(this);

    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(Settings::scale(8));
    layout->addWidget(pill);
    layout->addWidget(image);

    // Style
    this->setFixedSize(Settings::scale(60), Settings::scale(48));
    pill->setFixedHeight(Settings::scale(40));
    image->setFixedSize(Settings::scale(48), Settings::scale(48));
    image->setBackgroundColor(Settings::BrandExperiment);
    image->setBorderRadius(Settings::scale(16));
    image->setImage(":home-icon.svg");
}

void HomeButton::mouseReleaseEvent(QMouseEvent *event)
{
    // Emit signals when clicked to open the home page
    if (event->button() == Qt::LeftButton) {
        emit clicked();
    }
}

void HomeButton::unclicked()
{
    // Reset the stylesheet of this widget if currently clicked
    if (clic) {
        clic = false;
        QPixmap img(":home-icon.svg");
        QPainter qp(&img);
        qp.setCompositionMode(QPainter::CompositionMode_SourceIn);
        qp.fillRect(QRect(img.rect().topLeft(), QSize(Settings::scale(img.rect().width()), Settings::scale(img.rect().height()))), QColor(220, 221, 222));
        qp.end();
        image->setPixmap(img);
        image->setBackgroundColor(Settings::BackgroundPrimary);
        image->setBorderRadius(Settings::scale(24));
    }
    pill->setFixedHeight(0);
}

void HomeButton::mousePressEvent(QMouseEvent *)
{
    // Widget clicked : change the stylesheet
    if (!clic) {
        QPixmap img(":home-icon.svg");
        QPainter qp(&img);
        qp.setCompositionMode(QPainter::CompositionMode_SourceIn);
        qp.fillRect(img.rect(),Qt::white);
        qp.end();
        image->setPixmap(img);
        image->setBackgroundColor(Settings::BrandExperiment);
        image->setBorderRadius(Settings::scale(16));
        clic = true;
    }
    pill->setFixedHeight(40);
}

void HomeButton::enterEvent(QEvent *)
{
    // Mouse hover : change the stylesheet
    if (!clic) {
        QPixmap img(":home-icon.svg");
        QPainter qp(&img);
        qp.setCompositionMode(QPainter::CompositionMode_SourceIn);
        qp.fillRect(img.rect(),Qt::white);
        qp.end();
        image->setPixmap(img);
        image->setBackgroundColor(Settings::BrandExperiment);
        image->setBorderRadius(Settings::scale(16));
    }
    if (pill->height() != Settings::scale(40)) pill->setFixedHeight(Settings::scale(20));
}

void HomeButton::leaveEvent(QEvent *)
{
    // Reset the stylesheet if not clicked
    if (!clic) {
        QPixmap img(":home-icon.svg");
        QPainter qp(&img);
        qp.setCompositionMode(QPainter::CompositionMode_SourceIn);
        qp.fillRect(img.rect(),QColor(220, 221, 222));
        qp.end();
        image->setPixmap(img);
        image->setBackgroundColor(Settings::BackgroundPrimary);
        image->setBorderRadius(Settings::scale(24));
    }
    if (pill->height() != Settings::scale(40)) pill->setFixedHeight(0);
}

} // namespace Ui
