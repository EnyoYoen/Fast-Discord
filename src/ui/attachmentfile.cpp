#include "ui/attachmentfile.h"

#include <cmath>

namespace Ui {

AttachmentFile::AttachmentFile(Api::RessourceManager *rmp, Api::Attachment *attachmentp, QWidget *parent)
    : QWidget(parent)
{
    rm = rmp;
    attachment = attachmentp;

    QWidget *container = new QWidget(this);

    this->setFixedHeight(62);
    container->setFixedHeight(62);
    container->setStyleSheet("background-color: #2F3136;"
                             "border-color: #292B2F;");

    QPixmap pix("res/images/svg/archive-icon.svg");
    QLabel *image = new QLabel(container);
    image->setPixmap(pix.scaled(30, 40, Qt::KeepAspectRatio));
    image->setFixedSize(30, 40);
    image->move(10, 10);

    QLabel *filename = new QLabel(attachment->filename->c_str(), container);
    filename->move(48, 10);
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
    QLabel *filesize = new QLabel(sizeStr, container);
    filesize->move(48, 26);
    filesize->setStyleSheet("color: #72767D;");
}

/*void AttachmentFile::mouseReleaseEvent(QMouseEvent *event)
{

}*/

} // namespace Ui
