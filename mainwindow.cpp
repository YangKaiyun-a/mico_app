#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "define.h"
#include "signalmanager.h"
#include "logger.h"

#include <QDebug>
#include <QTimer>
#include <QThread>


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    init();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init()
{
    LOG_INFO("mico_app 程序启动" << QThread::currentThreadId());

    initData();
    initUI();
    initConnect();

    // 加载上次保存的配置
    QTimer::singleShot(50, this, [=]() {

    });
}

void MainWindow::initData()
{

}

void MainWindow::initUI()
{
    // 初始化登录页
    m_loginWidget = new LoginWidget(this);
    ui->stackedWidget->addWidget(m_loginWidget);

    // 主页面
    m_mainWidget = new MainWidget();
    ui->stackedWidget->addWidget(m_mainWidget);

    ui->stackedWidget->setCurrentWidget(m_loginWidget);
}

void MainWindow::initConnect()
{
    connect(SigManager, &SignalManager::sigLogin, this, [=](bool state){
        if(state)
        {
            // 登陆成功后跳转至主页面
            ui->stackedWidget->setCurrentWidget(m_mainWidget);
        }
        else
        {
            //处理注销的逻辑


        }
    });
}


