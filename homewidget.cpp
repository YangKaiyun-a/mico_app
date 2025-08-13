#include "homewidget.h"
#include "ui_homewidget.h"
#include "dbmanager.h"
#include "config_manager.h"

#include <QGridLayout>
#include <QDebug>

HomeWidget::HomeWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::HomeWidget)
{
    ui->setupUi(this);
    init();
}

HomeWidget::~HomeWidget()
{
    delete ui;
}

void HomeWidget::init()
{
    initData();
    initUI();
}

void HomeWidget::initData()
{
    m_taskMonitorMap.clear();
}

void HomeWidget::initUI()
{
    initTaskWidget();
    initRobotWidget();
    initDeviceWidget();
}

void HomeWidget::initTaskWidget()
{
    QGridLayout *gridLayout = new QGridLayout();
    gridLayout->setContentsMargins(0, 0, 0, 0);

    for(int row = 0; row < 2; ++row)
    {
        gridLayout->setRowStretch(row, 1);
    }

    for(int col = 0; col < 3; ++col)
    {
        gridLayout->setColumnStretch(col, 1);
    }

    ui->taskWidget->setLayout(gridLayout);

    m_currentTaskWidget = new CurrentTaskWidget("当前任务", "试剂转载");
    gridLayout->addWidget(m_currentTaskWidget, 0, 0, 1 ,2);

    StatusMonitorWidget *waitingMonitor = new StatusMonitorWidget("待执行");
    gridLayout->addWidget(waitingMonitor, 0, 2, 1 ,1);
    m_taskMonitorMap.insert(ENUM_CLASS::WAITING, waitingMonitor);

    StatusMonitorWidget *executingMonitor = new StatusMonitorWidget("执行中");
    gridLayout->addWidget(executingMonitor, 1, 0, 1 ,1);
    m_taskMonitorMap.insert(ENUM_CLASS::EXECUTING, executingMonitor);

    StatusMonitorWidget *finishMonitor = new StatusMonitorWidget("已完成");
    gridLayout->addWidget(finishMonitor, 1, 1, 1 ,1);
    m_taskMonitorMap.insert(ENUM_CLASS::FINISH, finishMonitor);

    StatusMonitorWidget *failedMonitor = new StatusMonitorWidget("失败");
    gridLayout->addWidget(failedMonitor, 1, 2, 1 ,1);
    m_taskMonitorMap.insert(ENUM_CLASS::FAILED, failedMonitor);
}

void HomeWidget::initRobotWidget()
{

}

void HomeWidget::initDeviceWidget()
{
    if(!DbManager->getMotorStatus())
    {
        qDebug() << "获取电机模块失败！";
    }

    QGridLayout *gridLayout = new QGridLayout();
    gridLayout->setContentsMargins(0, 0, 0, 0);

    for(int row = 0; row < 2; ++row)
    {
        gridLayout->setRowStretch(row, 1);
    }

    for(int col = 0; col < 7; ++col)
    {
        gridLayout->setColumnStretch(col, 1);
    }

    ui->deviceWidget->setLayout(gridLayout);


    QMap<int, MotorStatus> motorStatusMap = CfgManager->motorStatusMap();

    int row = 0, col = 0;
    for(const MotorStatus &motor : motorStatusMap.values())
    {
        QString status = CfgManager->motorStatusToString(motor.status);

        MotorStatusWidget *wgt = new MotorStatusWidget(motor.motorName, status);
        gridLayout->addWidget(wgt, row, col, 1 ,1);
        ++col;
        if(col == 7)
        {
            col = 0;
            row = 1;
        }
    }
}
