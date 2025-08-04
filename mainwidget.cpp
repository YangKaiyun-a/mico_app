#include "mainwidget.h"
#include "ui_mainwidget.h"
#include "homewidget.h"
#include "taskwidget.h"
#include "mapwidget.h"
#include "loggerwidget.h"
#include "accountwidget.h"
#include "debugwidget.h"

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
    ui->stackedWidget->insertWidget(HOME, homeWgt);

    TaskWidget *taskWgt = new TaskWidget(this);
    ui->stackedWidget->insertWidget(TASK, taskWgt);

    MapWidget *mapWgt = new MapWidget(this);
    ui->stackedWidget->insertWidget(MAP, mapWgt);

    LoggerWidget *loggerWgt = new LoggerWidget(this);
    ui->stackedWidget->insertWidget(RECORD, loggerWgt);

    AccountWidget *accountWgt = new AccountWidget(this);
    ui->stackedWidget->insertWidget(ACCOUNT, accountWgt);

    DebugWidget *debugWgt = new DebugWidget(this);
    ui->stackedWidget->insertWidget(DEBUG, debugWgt);

    ui->stackedWidget->setCurrentIndex(HOME);
}
