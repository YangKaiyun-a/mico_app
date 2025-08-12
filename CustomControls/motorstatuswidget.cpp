#include "motorstatuswidget.h"
#include "ui_motorstatuswidget.h"

MotorStatusWidget::MotorStatusWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MotorStatusWidget)
{
    ui->setupUi(this);
}

MotorStatusWidget::~MotorStatusWidget()
{
    delete ui;
}
