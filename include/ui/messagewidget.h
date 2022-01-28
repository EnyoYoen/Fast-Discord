#pragma once

#include "ui/roundedimage.h"
#include "ui/markdownlabel.h"
#include "api/message.h"
#include "api/ressourcemanager.h"

#include <QWidget>
#include <QLabel>

namespace Ui {

// A widget to show messages
class MessageWidget : public QWidget
{
    Q_OBJECT
public:
    MessageWidget(Api::RessourceManager *rm, Api::Message *message, bool isFirst, bool separatorBefore, QWidget *parent);

    Api::RessourceManager *rm; // To request the API

private:
    void enterEvent(QEvent *) override;
    void leaveEvent(QEvent *) override;
    void setAvatar(const std::string& avatarFileName);

    QString hoveredTimestamp; // The timestamp we show
    RoundedImage *avatar;     // The avatar if there is one
    QLabel *timestampLabel;   // The label of the timestamp
    MarkdownLabel *content;
    bool isFirst;             // Is the message is not grouped

};

} // namespace Ui
