#include "ui/attachmentfile.h"

#include "ui/downloadbutton.h"

#include <QHBoxLayout>
#include <QVBoxLayout>

#include <cmath>

namespace Ui {

AttachmentFile::AttachmentFile(Api::Requester *requester, Api::Attachment *attachment, QWidget *parent)
    : QLabel(parent)
{
    QWidget *container = new QWidget(this);
    QHBoxLayout *layout = new QHBoxLayout(container);
    QWidget *infos = new QWidget(container);
    QVBoxLayout *infosLayout = new QVBoxLayout(infos);

    QPixmap pix("res/images/svg/archive-icon.svg");
    QLabel *image = new QLabel(container);
    image->setPixmap(pix.scaled(30, 40, Qt::KeepAspectRatio));
    image->setFixedSize(30, 40);

    QLabel *filename = new QLabel(attachment->filename->c_str(), infos);
    filename->setStyleSheet("color: #00AFF4;");

    QString sizeStr;
    int size = attachment->size;
    if (size < 1024) {
        sizeStr.setNum(size) += " O";
    } else if (size < 1024 * 1024) {
        sizeStr.setNum(roundf(size / 1024. * 100) / 100) += " KB";
    } else {
        sizeStr.setNum(roundf(size / (1024. * 1024.) * 100) / 100) += " MB";
    }
    QLabel *filesize = new QLabel(sizeStr, infos);
    filesize->setStyleSheet("color: #72767D;");

    infosLayout->addWidget(filename);
    infosLayout->addWidget(filesize);
    infosLayout->setContentsMargins(0, 0, 0, 0);
    infosLayout->setSpacing(0);

    layout->addWidget(image);
    layout->addWidget(infos);
    layout->addWidget(new DownloadButton(*attachment->proxyUrl, requester, container));
    layout->setContentsMargins(10, 10, 10, 10);
    layout->setSpacing(8);

    container->setFixedHeight(62);
    container->setStyleSheet("background-color: #2F3136;"
                             "border-color: #292B2F;");
    this->setFixedHeight(62);
    this->setStyleSheet("border-radius: 5px;");
}

} // namespace Ui
