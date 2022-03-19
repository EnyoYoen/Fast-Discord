#include "ui/mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    qRegisterMetaType<Api::Message>();

    QApplication a(argc, argv);
    Ui::MainWindow w;
    w.show();
    return a.exec();
}
