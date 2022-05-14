#include "ui/rightcolumn/messageseparator.h"

#include <QHBoxLayout>
#include <QLabel>

namespace Ui {

MessageSeparator::MessageSeparator(const QDate& date, QWidget *parent)
    : QWidget(parent)
{
    // Create the widgets
    QHBoxLayout *layout = new QHBoxLayout(this);
    QWidget *line1 = new QWidget(this);
    QWidget *line2 = new QWidget(this);

    // Determine the month of the separator timestamp
    QString month("");
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
    // Create the label with the timestamp
    QLabel *dateLabel = new QLabel(month + " " + QString::number(date.day()) + ", " + QString::number(date.year()), this);
    QFont font;
    font.setPixelSize(12);
    font.setFamily("whitney");
    dateLabel->setFont(font);

    // Add the widgets to the layout
    layout->addWidget(line1);
    layout->addWidget(dateLabel);
    layout->addWidget(line2);

    // Style the lines
    line1->setStyleSheet("background-color: #42454B;");
    line2->setStyleSheet("background-color: #42454B;");
    line1->setFixedHeight(1);
    line2->setFixedHeight(1);

    // Style the date label
    dateLabel->setStyleSheet("color: #72767D;");
    dateLabel->setFixedHeight(21);
    dateLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    // Style the layout
    layout->setContentsMargins(16, 24, 14, 8);
}

} // namespace Ui
