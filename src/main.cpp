#include "../include/api/message.h"
#include "../include/ui/mainwindow.h"
#include "../include/token.h"

#include <QApplication>
#include <QInputDialog>

Q_DECLARE_METATYPE(Api::Message);

std::string token;

int main(int argc, char *argv[])
{
    qRegisterMetaType<Api::Message>();

    QApplication a(argc, argv);
    token = QInputDialog::getText(nullptr, "Token", "Enter your Discord token", QLineEdit::Normal, QString(), nullptr).toUtf8().constData();
    Ui::MainWindow w;
    w.show();
    return a.exec();
}
