#pragma once

#include "api/request.h"

#include <QLabel>
#include <QMouseEvent>

namespace Ui {

class FileUploadButton : public QLabel
{
    Q_OBJECT
public:
    FileUploadButton(QWidget *parent);

signals:
    void fileSelected(const QString& filePath);

private:
    void mouseReleaseEvent(QMouseEvent *event) override;
    void enterEvent(QEvent *) override;
    void leaveEvent(QEvent *) override;
};

}