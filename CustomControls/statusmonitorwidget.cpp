#include "statusmonitorwidget.h"
#include "ui_statusmonitorwidget.h"

StatusMonitorWidget::StatusMonitorWidget(const QString& title, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::StatusMonitorWidget)
{
    ui->setupUi(this);
    setTite(title);
    setContent("0");
}

StatusMonitorWidget::~StatusMonitorWidget()
{
    delete ui;
}

void StatusMonitorWidget::setTite(const QString &title)
{
    m_title = title;
    ui->labTitle->setText(m_title);
}

void StatusMonitorWidget::setContent(const QString &content)
{
    m_content = content;
    ui->labContent->setText(m_content);
}
