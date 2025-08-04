#include "debugwidget.h"
#include "ui_debugwidget.h"

DebugWidget::DebugWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::DebugWidget)
{
    ui->setupUi(this);
}

DebugWidget::~DebugWidget()
{
    delete ui;
}
