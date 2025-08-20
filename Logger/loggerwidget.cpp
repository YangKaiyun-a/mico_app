#include "loggerwidget.h"
#include "ui_loggerwidget.h"
#include "global.h"
#include "logtablemanager.h"

LoggerWidget::LoggerWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LoggerWidget)
{
    ui->setupUi(this);
    init();
}

LoggerWidget::~LoggerWidget()
{
    delete ui;
}

void LoggerWidget::init()
{
    initData();
    initUI();
}

void LoggerWidget::initData()
{
    // 初始化数据库表
    LOGTableManager->initTables();
}

void LoggerWidget::initUI()
{
    m_operationWidget = new OperationLogWidget(this);
    ui->stackedWidget->insertWidget(ENUM_CLASS::LOG_INDEX::OPERATION, m_operationWidget);

    ui->stackedWidget->setCurrentIndex(ENUM_CLASS::LOG_INDEX::OPERATION);
}

void LoggerWidget::enterLogBefore()
{
    if(!m_operationWidget)
    {
        return;
    }

    m_operationWidget->enterLogBefore();
    ui->btnOperationLog->setChecked(true);
    ui->stackedWidget->setCurrentIndex(ENUM_CLASS::LOG_INDEX::OPERATION);
}

void LoggerWidget::on_btnOperationLog_clicked()
{
    if(!m_operationWidget)
    {
        return;
    }

    m_operationWidget->enterLogBefore();
    ui->stackedWidget->setCurrentIndex(ENUM_CLASS::LOG_INDEX::OPERATION);
}


void LoggerWidget::on_btnSystemLog_clicked()
{
    // TODO: 增加判空
    ui->stackedWidget->setCurrentIndex(ENUM_CLASS::LOG_INDEX::SYSTEM);
}

