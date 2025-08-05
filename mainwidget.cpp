#include "mainwidget.h"
#include "ui_mainwidget.h"
#include "homewidget.h"
#include "taskwidget.h"
#include "mapwidget.h"
#include "loggerwidget.h"
#include "accountwidget.h"
#include "debugwidget.h"
#include "define.h"


MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWidget)
{
    ui->setupUi(this);
    init();
}

MainWidget::~MainWidget()
{
    delete ui;
}

void MainWidget::init()
{
    initStackWidget();
}

void MainWidget::initStackWidget()
{
    HomeWidget *homeWgt = new HomeWidget(this);
    ui->stackedWidget->insertWidget(ENUM_CLASS::HOME, homeWgt);

    TaskWidget *taskWgt = new TaskWidget(this);
    ui->stackedWidget->insertWidget(ENUM_CLASS::TASK, taskWgt);

    MapWidget *mapWgt = new MapWidget(this);
    ui->stackedWidget->insertWidget(ENUM_CLASS::MAP, mapWgt);

    LoggerWidget *loggerWgt = new LoggerWidget(this);
    ui->stackedWidget->insertWidget(ENUM_CLASS::RECORD, loggerWgt);

    AccountWidget *accountWgt = new AccountWidget(this);
    ui->stackedWidget->insertWidget(ENUM_CLASS::ACCOUNT, accountWgt);

    DebugWidget *debugWgt = new DebugWidget(this);
    ui->stackedWidget->insertWidget(ENUM_CLASS::DEBUG, debugWgt);

    ui->stackedWidget->setCurrentIndex(ENUM_CLASS::HOME);
}

void MainWidget::on_btnFirst_clicked()
{
    ui->stackedWidget->setCurrentIndex(ENUM_CLASS::HOME);
}


void MainWidget::on_btnTaskList_clicked()
{
    ui->stackedWidget->setCurrentIndex(ENUM_CLASS::TASK);
}


void MainWidget::on_btnMap_clicked()
{
    ui->stackedWidget->setCurrentIndex(ENUM_CLASS::MAP);
}


void MainWidget::on_btnLogger_clicked()
{
    ui->stackedWidget->setCurrentIndex(ENUM_CLASS::RECORD);
}


void MainWidget::on_btnUsers_clicked()
{
    ui->stackedWidget->setCurrentIndex(ENUM_CLASS::ACCOUNT);
}


void MainWidget::on_btnDebug_clicked()
{
    ui->stackedWidget->setCurrentIndex(ENUM_CLASS::DEBUG);
}

