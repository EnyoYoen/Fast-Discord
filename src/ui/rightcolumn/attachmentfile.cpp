#include "ui/rightcolumn/attachmentfile.h"

#include "ui/rightcolumn/downloadbutton.h"
#include "ui/rightcolumn/downloadlink.h"

#include <QHBoxLayout>
#include <QVBoxLayout>

namespace Ui {

AttachmentFile::AttachmentFile(Api::Requester *requester, const Api::Attachment *attachment, QWidget *parent)
    : Widget(parent)
{
    Widget *container = new Widget(this);
    QHBoxLayout *layout = new QHBoxLayout(container);
    QWidget *infos = new QWidget(container);
    QVBoxLayout *infosLayout = new QVBoxLayout(infos);

    QPixmap pix("res/images/svg/archive-icon.svg");
    QLabel *image = new QLabel(container);
    image->setPixmap(pix.scaled(30, 40, Qt::KeepAspectRatio));
    image->setFixedSize(30, 40);

    QString sizeStr;
    int size = attachment->size;
    if (size < 1024) {
        sizeStr.setNum(size) += " O";
    } else if (size < 1024 * 1024) {
        sizeStr.setNum(qRound(size / 1024. * 100) / 100) += " KB";
    } else {
        sizeStr.setNum(qRound(size / (1024. * 1024.) * 100) / 100) += " MB";
    }
    Label *filesize = new Label(sizeStr, infos);
    QFont font;
    font.setPixelSize(12);
    font.setFamily("whitney");
    filesize->setFont(font);
    filesize->setFixedSize(QFontMetrics(font).horizontalAdvance(sizeStr), 16);
    filesize->setTextColor(Settings::FileSizeColor);
    filesize->setCursor(Qt::IBeamCursor);

    infosLayout->addWidget(new DownloadLink(attachment->url, attachment->filename, requester, container));
    infosLayout->addWidget(filesize);
    infosLayout->setContentsMargins(0, 0, 0, 0);
    infosLayout->setSpacing(0);

    layout->addWidget(image);
    layout->addWidget(infos);
    layout->addWidget(new DownloadButton(attachment->url, requester, container));
    layout->setContentsMargins(10, 10, 10, 10);
    layout->setSpacing(8);

    container->setFixedHeight(62);
    container->setBackgroundColor(Settings::BackgroundSecondary);
    container->setBorderSize(1);
    container->setBorderColor(Settings::BackgroundSecondaryAlt);
    this->setFixedHeight(62);
    this->setBorderRadius(3);
}

} // namespace Ui
