#include "../../include/ui/messagewidget.h"

#include "../../include/api/request.h"
#include "../../include/api/jsonutils.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>

#include <fstream>

namespace Ui {

MessageWidget::MessageWidget(const Api::Message& message, QWidget *parent) : QWidget(parent)
{
    QHBoxLayout *mainLayout = new QHBoxLayout();
    QWidget *data = new QWidget();
    QVBoxLayout *dataLayout = new QVBoxLayout();

    std::string avatarFileName;
    Api::User& author = *message.author;
    std::string *avatarId = author.avatar;
    if (avatarId == nullptr) {
        avatarFileName = "res/images/png/user-icon-asset.png";
    } else {
        avatarFileName = *author.id + (author.avatar->rfind("a_") == 0 ? ".gif" : ".webp");
        if (!std::ifstream(("cache/" + avatarFileName).c_str()).good()) {
            Api::Request::requestFile("https://cdn.discordapp.com/avatars/" + *author.id + "/" + avatarFileName, "cache/" + avatarFileName);
        }
        avatarFileName = "cache/" + avatarFileName;
    }

    name = new QLabel((*author.username).c_str());
    name->setTextInteractionFlags(Qt::TextSelectableByMouse);
    name->setCursor(QCursor(Qt::IBeamCursor));
    name->setStyleSheet("color: #fff");

    content = new QLabel((*message.content).c_str());
    content->setTextInteractionFlags(Qt::TextSelectableByMouse);
    content->setCursor(QCursor(Qt::IBeamCursor));
    content->setWordWrap(true);
    content->setStyleSheet("color: #dcddde");

    dataLayout->setSpacing(6);
    dataLayout->setContentsMargins(0, 0, 0, 0);
    dataLayout->addWidget(name);
    dataLayout->addWidget(content);

    data->setLayout(dataLayout);

    mainLayout->addWidget(new RoundedImage(avatarFileName, 40, 40, 20));
    mainLayout->addWidget(data);

    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    setLayout(mainLayout);
}

} // namespace Ui
