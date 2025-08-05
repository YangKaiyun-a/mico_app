#include "currenttaskwidget.h"
#include "ui_currenttaskwidget.h"

CurrentTaskWidget::CurrentTaskWidget(const QString& title, const QString& taskName, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CurrentTaskWidget)
{
    ui->setupUi(this);
    setTite(title);
    seTaskName(taskName);
    setContent("0%");
}

CurrentTaskWidget::~CurrentTaskWidget()
{
    delete ui;
}

void CurrentTaskWidget::setTite(const QString &title)
{
    m_title = title;
    ui->labTitle->setText(m_title);
}

void CurrentTaskWidget::setContent(const QString &content)
{
    m_content = content;
    ui->labContent->setText(m_content);
}

void CurrentTaskWidget::seTaskName(const QString &taskName)
{
    m_taskName = taskName;
    ui->labTaskName->setText(taskName);
}
