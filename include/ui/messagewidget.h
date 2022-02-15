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
    void defaultMessage(Api::Message *message, bool separatorBefore);
    void recipientMessage(Api::Message *message);
    void callMessage(Api::Message *message);

    void enterEvent(QEvent *) override;
    void leaveEvent(QEvent *) override;
    void setAvatar(const std::string& avatarFileName);
    void addImage(const std::string& filename, int width, int height);
    QString processTime(QTime time);
    QString processTimestamp(QDateTime dateTime);

    QString hoveredTimestamp;           // The timestamp we show
    RoundedImage *avatar = nullptr;     // The avatar if there is one
    QLabel *timestampLabel = nullptr;   // The label of the timestamp
    MarkdownLabel *content = nullptr;
    QVBoxLayout *dataLayout = nullptr;
    bool isFirst;             // Is the message is not grouped

};

} // namespace Ui
