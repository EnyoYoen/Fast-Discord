#include "ui/settings/settingsbutton.h"

#include <QHBoxLayout>
#include <QFontMetrics>

namespace Ui {

SettingsButton::SettingsButton(SettingsButton::Type typep, QString text, QWidget *parent)
    : QLabel(parent)
{
    type = typep;
    pressed = false;

    switch (type)
    {
        case SettingsButton::Type::NoBackground:
            this->setStyleSheet("color: #FFF;");
            break;
        case SettingsButton::Type::Edit:
            this->setStyleSheet("background-color: #4F545C;"
                                "color: #FFF;"
                                "border-radius: 3px;");
            break;
        case SettingsButton::Type::Important:
            this->setStyleSheet("background-color: #ED4245;"
                                "color: #FFF;"
                                "border-radius: 3px;");
            break;
        case SettingsButton::Type::Critical:
            this->setStyleSheet("border: 1px solid #ED4245;"
                                "color: #FFF;"
                                "border-radius: 3px;");
            break;
        case SettingsButton::Type::NormalOutlined:
            this->setStyleSheet("border: 1px solid #5865F2;"
                                "color: #FFF;"
                                "border-radius: 3px;");
            break;
        case SettingsButton::Type::Normal:    
        default:
            this->setStyleSheet("background-color: #5865F2;"
                                "color: #FFF;"
                                "border-radius: 3px;");
    }

    QFont font;
    font.setPixelSize(13);
    font.setFamily("whitney");
    QHBoxLayout *layout = new QHBoxLayout(this);
    content = new QLabel(text, this);
    content->setFont(font);
    content->setStyleSheet("border: none");
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(content, 0, Qt::AlignHCenter);

    this->setCursor(Qt::CursorShape::PointingHandCursor);
    this->setContentsMargins(16, 2, 16, 2);
    this->setFixedSize(QFontMetrics(font).width(text) + (type == Type::Critical ? 36 : 32), 32);
}


void SettingsButton::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        pressed = false;
        switch (type)
        {
            case SettingsButton::Type::NoBackground:
                this->setStyleSheet("color: #FFF;");
                break;
            case SettingsButton::Type::Edit:
                this->setStyleSheet("background-color: #4F545C;"
                                    "color: #FFF;"
                                    "border-radius: 3px;");
                break;
            case SettingsButton::Type::Important:
                this->setStyleSheet("background-color: #ED4245;"
                                    "color: #FFF;"
                                    "border-radius: 3px;");
                break;
            case SettingsButton::Type::Critical:
                this->setStyleSheet("border: 1px solid #ED4245;"
                                    "color: #FFF;"
                                    "border-radius: 3px;");
                break;
            case SettingsButton::Type::NormalOutlined:
                this->setStyleSheet("border: 1px solid #5865F2;"
                                    "color: #FFF;"
                                    "border-radius: 3px;");
                break;
            case SettingsButton::Type::Normal:    
            default:
                this->setStyleSheet("background-color: #5865F2;"
                                    "color: #FFF;"
                                    "border-radius: 3px;");
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
            this->setStyleSheet("color: #FFF;");
            break;
        }
        case SettingsButton::Type::Edit:
            this->setStyleSheet("background-color: #72767D;"
                                "color: #FFF;"
                                "border-radius: 3px;");
            break;
        case SettingsButton::Type::Important:
            this->setStyleSheet("background-color: #ED4245;"
                                "color: #FFF;"
                                "border-radius: 3px;");
            break;
        case SettingsButton::Type::Critical:
            this->setStyleSheet("background-color: #ED4245;"
                                "border: 1px solid #ED4245;"
                                "color: #FFF;"
                                "border-radius: 3px;");
            break;
        case SettingsButton::Type::NormalOutlined:
            this->setStyleSheet("background-color: #4752C4;"
                                "border: 1px solid #4752C4;"
                                "color: #FFF;"
                                "border-radius: 3px;");
            break;
        case SettingsButton::Type::Normal:    
        default:
            this->setStyleSheet("background-color: #3C45A5;"
                                "color: #FFF;"
                                "border-radius: 3px;");
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
                this->setStyleSheet("color: #FFF;");
                break;
            }
            case SettingsButton::Type::Edit:
                this->setStyleSheet("background-color: #686D73;"
                                    "color: #FFF;"
                                    "border-radius: 3px;");
                break;
            case SettingsButton::Type::Important:
                this->setStyleSheet("background-color: #C03537;"
                                    "color: #FFF;"
                                    "border-radius: 3px;");
                break;
            case SettingsButton::Type::Critical:
                this->setStyleSheet("background-color: #ED4245;"
                                    "border: 1px solid #ED4245;"
                                    "color: #FFF;"
                                    "border-radius: 3px;");
                break;
            case SettingsButton::Type::NormalOutlined:
                this->setStyleSheet("background-color: #5865F2;"
                                    "border: 1px solid #5865F2;"
                                    "color: #FFF;"
                                    "border-radius: 3px;");
                break;
            case SettingsButton::Type::Normal:    
            default:
                this->setStyleSheet("background-color: #4752C4;"
                                    "color: #FFF;"
                                    "border-radius: 3px;");
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
                this->setStyleSheet("color: #FFF;");
                break;
            }
            case SettingsButton::Type::Edit:
                this->setStyleSheet("background-color: #4F545C;"
                                    "color: #FFF;"
                                    "border-radius: 3px;");
                break;
            case SettingsButton::Type::Important:
                this->setStyleSheet("background-color: #ED4245;"
                                    "color: #FFF;"
                                    "border-radius: 3px;");
                break;
            case SettingsButton::Type::Critical:
                this->setStyleSheet("border: 1px solid #ED4245;"
                                    "color: #FFF;"
                                    "border-radius: 3px;");
                break;
            case SettingsButton::Type::NormalOutlined:
                this->setStyleSheet("border: 1px solid #5865F2;"
                                    "color: #FFF;"
                                    "border-radius: 3px;");
                break;
            case SettingsButton::Type::Normal:    
            default:
                this->setStyleSheet("background-color: #5865F2;"
                                    "color: #FFF;"
                                    "border-radius: 3px;");
        }
    }
}

} // namespace Ui