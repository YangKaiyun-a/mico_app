#include "operationlogwidget.h"
#include "ui_operationlogwidget.h"

OperationLogWidget::OperationLogWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::OperationLogWidget)
{
    ui->setupUi(this);
}

OperationLogWidget::~OperationLogWidget()
{
    delete ui;
}
