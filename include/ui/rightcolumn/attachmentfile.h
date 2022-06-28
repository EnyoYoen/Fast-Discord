#pragma once

#include "ui/common/basicwidgets.h"
#include "api/request.h"

#include <QLabel>
#include <QMouseEvent>

namespace Ui {

class AttachmentFile : public Widget
{
    Q_OBJECT
public:
    AttachmentFile(Api::Requester *requester, const Api::Attachment *attachment, QWidget *parent);
};

} // namespace Ui
