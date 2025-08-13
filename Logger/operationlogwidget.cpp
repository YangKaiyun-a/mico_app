#include "operationlogwidget.h"
#include "ui_operationlogwidget.h"

OperationLogWidget::OperationLogWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::OperationLogWidget)
{
    ui->setupUi(this);
    init();
}

OperationLogWidget::~OperationLogWidget()
{
    delete ui;
}

void OperationLogWidget::init()
{
    initData();
    initUI();
}

void OperationLogWidget::initData()
{
    m_tabModel = LOGTableManager->tableModel(ENUM_CLASS::LOG_INDEX::OPERATION);
}

void OperationLogWidget::initUI()
{

}









