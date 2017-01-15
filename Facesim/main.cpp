#include "mainwindow.h"
#include <QApplication>

#include "facer.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    indexar();
    comparation();

    return a.exec();
}
