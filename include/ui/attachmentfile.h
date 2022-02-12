#pragma once

#include "api/ressourcemanager.h"

#include <QWidget>
#include <QMouseEvent>

namespace Ui {

class AttachmentFile : public QWidget
{
    Q_OBJECT
public:
    AttachmentFile(Api::RessourceManager *rm, Api::Attachment *attachment, QWidget *parent);

private:
    //void mouseReleaseEvent(QMouseEvent *event) override;

    Api::RessourceManager *rm;
    Api::Attachment *attachment;
};

} // namespace Ui
