#include "../include/ui/mainwindow.h"
#include "../include/api/request.h"

#include <QApplication>
#include <QInputDialog>

int main(int argc, char *argv[])
{
    qRegisterMetaType<Api::Message>();

    QApplication a(argc, argv);
    Api::Request::token = QInputDialog::getText(nullptr, "Token", "Enter your Discord token", QLineEdit::Normal, QString(), nullptr).toUtf8().constData();
    Ui::MainWindow w;
    w.show();
    return a.exec();
}
