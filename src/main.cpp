#include "../include/ui/mainwindow.h"

#include <QApplication>

#include <string>

int main(int argc, char *argv[])
{
    qRegisterMetaType<Api::Message>();

    QApplication a(argc, argv);
    Ui::MainWindow w;
    w.show();
    return a.exec();
}
