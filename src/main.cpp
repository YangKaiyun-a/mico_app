#include "mainwindow.h"
#include "application.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    Application a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
