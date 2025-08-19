#include "taskfinishedwidget.h"
#include "ui_taskfinishedwidget.h"

taskFinishedWidget::taskFinishedWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::taskFinishedWidget)
{
    ui->setupUi(this);
}

taskFinishedWidget::~taskFinishedWidget()
{
    delete ui;
}
