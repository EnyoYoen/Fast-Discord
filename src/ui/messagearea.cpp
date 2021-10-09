#include "../../include/ui/messagearea.h"

#include "../../include/ui/messagewidget.h"

#include <QScrollBar>
#include <QGroupBox>
#include <QVBoxLayout>

namespace Ui {

MessageArea::MessageArea(const std::vector<Api::Message>& messages) : QScrollArea()
{
    QGroupBox *messageBox = new QGroupBox();
    QVBoxLayout *messageLayout = new QVBoxLayout();

    for (int i = messages.size() - 1; i >= 0 ; i--) {
        messageLayout->addWidget(new MessageWidget(messages[i]));
    }

    messageLayout->setSpacing(20);
    setStyleSheet("QScrollArea {border: none;}"
                  "QScrollBar::handle:vertical {border: none; border-radius: 8px; background-color: #202225;}"
                  "QScrollBar:vertical {border: none; background-color: #2F3136; border-radius: 8px; width: 16px;}"
                  "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {border:none; background: none; height: 0;}");

    messageBox->setLayout(messageLayout);
    setWidget(messageBox);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void MessageArea::showEvent(QShowEvent *event)
{
    QScrollBar *vsb = verticalScrollBar();
    vsb->setValue(vsb->maximum()); // TODO : Not Scrolled to the very end, idk why
}

} // namespace Ui
