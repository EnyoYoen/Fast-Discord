#include "ui/common/basicwidgets.h"

#include <QMouseEvent>

namespace Ui {

class SelectImageButton : public Label
{
    Q_OBJECT
public:
    SelectImageButton(QWidget *parent);

signals:
    void clicked();

private:
    void mouseReleaseEvent(QMouseEvent *event) override;
};

} // namespace Ui