#include "loggerwidget.h"
#include "ui_loggerwidget.h"
#include "global.h"

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
    m_operationWidget = new OperationLogWidget();
    ui->stackedWidget->insertWidget(ENUM_CLASS::LOG_INDEX::OPERATION, m_operationWidget);

    ui->stackedWidget->setCurrentIndex(ENUM_CLASS::LOG_INDEX::OPERATION);
}
