#include "loggerwidget.h"
#include "ui_loggerwidget.h"

LoggerWidget::LoggerWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LoggerWidget)
{
    ui->setupUi(this);
}

LoggerWidget::~LoggerWidget()
{
    delete ui;
}
