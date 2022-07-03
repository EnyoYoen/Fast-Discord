#include "ui/settings/settingsbutton.h"

#include <QHBoxLayout>
#include <QFontMetrics>

namespace Ui {

SettingsButton::SettingsButton(SettingsButton::Type typep, QString text, QWidget *parent)
    : Widget(parent)
{
    type = typep;
    pressed = false;

    QFont font;
    font.setPixelSize(13);
    font.setFamily("whitney");
    content = new Label(text, this);
    content->setFixedSize(QFontMetrics(font).horizontalAdvance(text), 16);
    content->setFont(font);

    switch (type)
    {
        case SettingsButton::Type::NoBackground:
            content->setTextColor(Settings::SettingsButtonTextColor);
            break;
        case SettingsButton::Type::Edit:
            content->setTextColor(Settings::White);
            this->setBackgroundColor(Settings::ButtonSecondaryBackground);
            break;
        case SettingsButton::Type::Important:
            content->setTextColor(Settings::White);
            this->setBackgroundColor(Settings::ButtonDangerBackground);
            break;
        case SettingsButton::Type::Critical:
            content->setTextColor(Settings::ButtonOutlineDangerText);
            this->setBackgroundColor(Settings::None);
            this->setBorderColor(Settings::ButtonOutlineDangerBorder);
            this->setBorderSize(1);
            break;
        case SettingsButton::Type::NormalOutlined:
            content->setTextColor(Settings::White);
            this->setBackgroundColor(Settings::None);
            this->setBorderColor(Settings::BrandExperiment);
            this->setBorderSize(1);
            break;
        case SettingsButton::Type::Normal:    
        default:
            content->setTextColor(Settings::White);
            this->setBackgroundColor(Settings::BrandExperiment);
    }

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(content, 0, Qt::AlignHCenter);

    this->setCursor(Qt::CursorShape::PointingHandCursor);
    this->setBorderRadius(3);
    this->setContentsMargins(16, 2, 16, 2);
    this->setFixedSize(QFontMetrics(font).horizontalAdvance(text) + (type == Type::Critical ? 36 : 32), 32);
}


void SettingsButton::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        pressed = false;
        switch (type)
        {
            case SettingsButton::Type::NoBackground:
            {
                QFont font;
                font.setPixelSize(13);
                font.setFamily("whitney");
                font.setUnderline(false);
                content->setFont(font);
                content->setTextColor(Settings::SettingsButtonTextColor);
                break;
            }
            case SettingsButton::Type::Edit:
                content->setTextColor(Settings::White);
                this->setBackgroundColor(Settings::ButtonSecondaryBackground);
                break;
            case SettingsButton::Type::Important:
                content->setTextColor(Settings::White);
                this->setBackgroundColor(Settings::ButtonDangerBackground);
                break;
            case SettingsButton::Type::Critical:
                content->setTextColor(Settings::ButtonOutlineDangerText);
                this->setBackgroundColor(Settings::None);
                this->setBorderColor(Settings::ButtonOutlineDangerBorder);
                this->setBorderSize(1);
                break;
            case SettingsButton::Type::NormalOutlined:
                content->setTextColor(Settings::White);
                this->setBackgroundColor(Settings::None);
                this->setBorderColor(Settings::BrandExperiment);
                this->setBorderSize(1);
                break;
            case SettingsButton::Type::Normal:    
            default:
                content->setTextColor(Settings::White);
                this->setBackgroundColor(Settings::BrandExperiment);
        }
        emit clicked();
    }
}

void SettingsButton::mousePressEvent(QMouseEvent *)
{
    pressed = true;
    switch (type)
    {
        case SettingsButton::Type::NoBackground:
        {
            QFont font;
            font.setPixelSize(13);
            font.setFamily("whitney");
            font.setUnderline(true);
            content->setFont(font);
            content->setTextColor(Settings::SettingsButtonTextColor);
            break;
        }
        case SettingsButton::Type::Edit:
            content->setTextColor(Settings::White);
            this->setBackgroundColor(Settings::ButtonSecondaryBackgroundActive);
            break;
        case SettingsButton::Type::Important:
            content->setTextColor(Settings::White);
            this->setBackgroundColor(Settings::ButtonDangerBackgroundActive);
            break;
        case SettingsButton::Type::Critical:
            content->setTextColor(Settings::ButtonOutlineDangerTextActive);
            this->setBackgroundColor(Settings::ButtonOutlineDangerBackgroundActive);
            this->setBorderColor(Settings::ButtonOutlineDangerBorderActive);
            this->setBorderSize(1);
            break;
        case SettingsButton::Type::NormalOutlined:
            content->setTextColor(Settings::White);
            this->setBackgroundColor(Settings::BrandExperiment560);
            this->setBorderColor(Settings::BrandExperiment560);
            this->setBorderSize(1);
            break;
        case SettingsButton::Type::Normal:    
        default:
            content->setTextColor(Settings::White);
            this->setBackgroundColor(Settings::BrandExperiment560);
    }
}

void SettingsButton::enterEvent(QEvent *)
{
    if (!pressed) {
        switch (type)
        {
            case SettingsButton::Type::NoBackground:
            {
                QFont font;
                font.setPixelSize(13);
                font.setFamily("whitney");
                font.setUnderline(true);
                content->setFont(font);
                content->setTextColor(Settings::SettingsButtonTextColor);
                break;
            }
            case SettingsButton::Type::Edit:
                content->setTextColor(Settings::White);
                this->setBackgroundColor(Settings::ButtonSecondaryBackgroundHover);
                break;
            case SettingsButton::Type::Important:
                content->setTextColor(Settings::White);
                this->setBackgroundColor(Settings::ButtonDangerBackgroundActive);
                break;
            case SettingsButton::Type::Critical:
                content->setTextColor(Settings::ButtonOutlineDangerTextHover);
                this->setBackgroundColor(Settings::ButtonOutlineDangerBackgroundHover);
                this->setBorderColor(Settings::ButtonOutlineDangerBorderHover);
                this->setBorderSize(1);
                break;
            case SettingsButton::Type::NormalOutlined:
                content->setTextColor(Settings::White);
                this->setBackgroundColor(Settings::BrandExperiment);
                this->setBorderColor(Settings::BrandExperiment);
                this->setBorderSize(1);
                break;
            case SettingsButton::Type::Normal:    
            default:
                content->setTextColor(Settings::White);
                this->setBackgroundColor(Settings::BrandExperiment560);
        }
    }
}

void SettingsButton::leaveEvent(QEvent *)
{
    if (!pressed) {
        switch (type)
        {
            case SettingsButton::Type::NoBackground:
            {
                QFont font;
                font.setPixelSize(13);
                font.setFamily("whitney");
                font.setUnderline(false);
                content->setFont(font);
                content->setTextColor(Settings::SettingsButtonTextColor);
                break;
            }
            case SettingsButton::Type::Edit:
                content->setTextColor(Settings::White);
                this->setBackgroundColor(Settings::ButtonSecondaryBackground);
                break;
            case SettingsButton::Type::Important:
                content->setTextColor(Settings::White);
                this->setBackgroundColor(Settings::ButtonDangerBackground);
                break;
            case SettingsButton::Type::Critical:
                content->setTextColor(Settings::ButtonOutlineDangerText);
                this->setBackgroundColor(Settings::None);
                this->setBorderColor(Settings::ButtonOutlineDangerBorder);
                this->setBorderSize(1);
                break;
            case SettingsButton::Type::NormalOutlined:
                content->setTextColor(Settings::White);
                this->setBackgroundColor(Settings::None);
                this->setBorderColor(Settings::BrandExperiment);
                this->setBorderSize(1);
                break;
            case SettingsButton::Type::Normal:    
            default:
                content->setTextColor(Settings::White);
                this->setBackgroundColor(Settings::BrandExperiment);
        }
    }
}

} // namespace Ui