#include "ui/messageseparator.h"

#include <QHBoxLayout>
#include <QLabel>

#include <string>

namespace Ui {

MessageSeparator::MessageSeparator(QDate date, QWidget *parent) : QWidget(parent)
{
    QHBoxLayout *layout = new QHBoxLayout();
    QWidget *line1 = new QWidget();
    QWidget *line2 = new QWidget();

    std::string month("");
    switch(date.month()) {
        case 1:
            month = "January";
            break;
        case 2:
            month = "February";
            break;
        case 3:
            month = "March";
            break;
        case 4:
            month = "April";
            break;
        case 5:
            month = "May";
            break;
        case 6:
            month = "June";
            break;
        case 7:
            month = "July";
            break;
        case 8:
            month = "August";
            break;
        case 9:
            month = "September";
            break;
        case 10:
            month = "October";
            break;
        case 11:
            month = "November";
            break;
        case 12:
            month = "December";
            break;
    }
    QLabel *dateLabel = new QLabel((month + " " + std::to_string(date.day()) + ", " + std::to_string(date.year())).c_str());

    layout->addWidget(line1);
    layout->addWidget(dateLabel);
    layout->addWidget(line2);

    line1->setStyleSheet("background-color: #42454B;");
    line2->setStyleSheet("background-color: #42454B;");
    dateLabel->setStyleSheet("color: #72767D;");

    line1->setFixedHeight(1);
    line2->setFixedHeight(1);
    dateLabel->setFixedHeight(21);
    dateLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    layout->setContentsMargins(16, 24, 14, 8);

    setLayout(layout);
}

} // namespace Ui
