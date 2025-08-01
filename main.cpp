#include "mainwindow.h"
#include "application.h"
#include "easylogging++.h"

#include <QApplication>

INITIALIZE_EASYLOGGINGPP

int main(int argc, char *argv[])
{
    Application::instance(argc, argv)->init();

    MainWindow w;
    w.show();
    return  Application::instance()->exec();
}
