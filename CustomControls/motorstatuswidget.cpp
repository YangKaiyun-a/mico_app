#include "motorstatuswidget.h"
#include "ui_motorstatuswidget.h"

MotorStatusWidget::MotorStatusWidget(const QString &labName, const QString &labStatus, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MotorStatusWidget), m_labName(labName), m_labStatus(labStatus)
{
    ui->setupUi(this);
    ui->labName->setText(m_labName);
    ui->labStatus->setText(m_labStatus);
}

MotorStatusWidget::~MotorStatusWidget()
{
    delete ui;
}

void MotorStatusWidget::setLabName(const QString &newLabName)
{
    m_labName = newLabName;
    ui->labName->setText(m_labName);
}

void MotorStatusWidget::setLabStatus(const QString &newLabStatus)
{
    m_labStatus = newLabStatus;
    ui->labStatus->setText(m_labStatus);
}
