#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "loginwidget.h"
#include "mainwidget.h"

#include <QMainWindow>


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:

private:
    Ui::MainWindow *ui;
    void init();
    void initData();
    void initUI();
    void initConnect();

private:
    LoginWidget * m_loginWidget = nullptr;
    MainWidget * m_mainWidget = nullptr;

};
#endif  // MAINWINDOW_H
