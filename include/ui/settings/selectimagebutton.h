#include <QLabel>
#include <QMouseEvent>

namespace Ui {

class SelectImageButton : public QLabel
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