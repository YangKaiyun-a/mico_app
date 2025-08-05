#include "robotstatuswidget.h"
#include "ui_robotstatuswidget.h"

RobotStatusWidget::RobotStatusWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::RobotStatusWidget)
{
    ui->setupUi(this);
}

RobotStatusWidget::~RobotStatusWidget()
{
    delete ui;
}
