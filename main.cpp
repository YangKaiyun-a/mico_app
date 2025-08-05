#include "mainwindow.h"
#include "application.h"
#include "easylogging++.h"

#include <QApplication>
#include <QFile>
#include <QDebug>

INITIALIZE_EASYLOGGINGPP

void setStyle()
{
    QFile file(":/winJk.qss");
    if(file.open(QFile::ReadOnly))
    {
        qDebug() << "QSS文件打开成功";
        QTextStream filetext(&file);
        QString stylesheet = filetext.readAll();
        qApp->setStyleSheet(stylesheet);
        file.close();
    }
    else
    {
        qDebug() << "QSS文件打开失败" << file.errorString();
    }
}


int main(int argc, char *argv[])
{
    Application::instance(argc, argv)->init();

    setStyle();

    MainWindow w;
    w.show();
    return  Application::instance()->exec();
}
