#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCoreApplication::setOrganizationName("brillview");
    QCoreApplication::setOrganizationDomain("brillview.com");
    QCoreApplication::setApplicationName("DebugTrace");

    MainWindow w;
    w.show();

    return a.exec();
}
