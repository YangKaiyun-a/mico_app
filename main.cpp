#include "mainwindow.h"
#include "application.h"
#include "easylogging++.h"

#include <QApplication>

INITIALIZE_EASYLOGGINGPP

int main(int argc, char *argv[])
{
    Application a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
