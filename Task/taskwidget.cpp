#include "taskwidget.h"
#include "ui_taskwidget.h"
#include "db/tasktablemanager.h"
#include "global.h"

TaskWidget::TaskWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TaskWidget)
{
    ui->setupUi(this);
    init();
}

TaskWidget::~TaskWidget()
{
    delete ui;
}

void TaskWidget::init()
{
    initData();
    initUI();
}

void TaskWidget::initData()
{
    // 初始化数据库表
    TaskTableManager::instance()->initTables();
}

void TaskWidget::initUI()
{
    m_taskListWidget = new TaskListWidget();
    ui->stackedWidget->insertWidget(ENUM_CLASS::TASK_INDEX::TASK_LIST, m_taskListWidget);

    m_taskFinishedWidget = new TaskFinishedWidget();
    ui->stackedWidget->insertWidget(ENUM_CLASS::TASK_INDEX::TASK_FINISHED, m_taskFinishedWidget);

    ui->stackedWidget->setCurrentIndex(ENUM_CLASS::TASK_INDEX::TASK_LIST);
}

void TaskWidget::enterLogBefore()
{
    if(!m_taskListWidget)
    {
        return;
    }

    m_taskListWidget->enterLogBefore();
    ui->btnTaskList->setChecked(true);
    ui->stackedWidget->setCurrentIndex(ENUM_CLASS::TASK_INDEX::TASK_LIST);
}


void TaskWidget::on_btnTaskList_clicked()
{
    m_taskListWidget->enterLogBefore();
    ui->stackedWidget->setCurrentIndex(ENUM_CLASS::TASK_INDEX::TASK_LIST);
}


void TaskWidget::on_btnTaskFinished_clicked()
{
    m_taskFinishedWidget->enterLogBefore();
    ui->stackedWidget->setCurrentIndex(ENUM_CLASS::TASK_INDEX::TASK_FINISHED);
}

