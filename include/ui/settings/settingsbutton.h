#pragma once

#include <QLabel>
#include <QMouseEvent>

namespace Ui {

class SettingsButton : public QLabel
{
    Q_OBJECT
public:
    enum class Type : quint8 {
        NoBackground,
        Edit,
        Normal,
        Important,
        Critical
    };

    SettingsButton(Type type, QString text, QWidget *parent);

signals:
    void clicked();

private:
    void mouseReleaseEvent(QMouseEvent *) override;
    void mousePressEvent(QMouseEvent *) override;
    void enterEvent(QEvent *) override;
    void leaveEvent(QEvent *) override;

    QLabel *content;
    Type type;
    bool pressed;
};

} // namespace Ui