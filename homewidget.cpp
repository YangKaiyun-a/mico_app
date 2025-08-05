#include "homewidget.h"
#include "ui_homewidget.h"
#include "robotstatuswidget.h"

#include <QGridLayout>

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
    RobotStatusWidget *robotWgt = new RobotStatusWidget();
    ui->horizontalLayout_2->addWidget(robotWgt);

    ui->horizontalLayout_2->setStretch(0, 1);
    ui->horizontalLayout_2->setStretch(1, 1);
}

void HomeWidget::initDeviceWidget()
{

}
