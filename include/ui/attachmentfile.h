#pragma once

#include "api/request.h"

#include <QLabel>
#include <QMouseEvent>

namespace Ui {

class AttachmentFile : public QLabel
{
    Q_OBJECT
public:
    AttachmentFile(Api::Requester *requester, Api::Attachment *attachment, QWidget *parent);
};

} // namespace Ui
