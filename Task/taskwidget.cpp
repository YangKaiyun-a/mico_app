#include "taskwidget.h"
#include "ui_taskwidget.h"
#include "tasktablemanager.h"

const int PAGE_NUM = 10;    ///< 一页中显示的记录条数

TaskWidget::TaskWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TaskWidget)
{
    ui->setupUi(this);
    init();
}

TaskWidget::~TaskWidget()
{
    delete ui;
}

void TaskWidget::init()
{
    initData();
    initUI();
}

void TaskWidget::initData()
{
    // 初始化数据库表
    TaskTableManager::instance()->initTables();

    m_tableModel = TaskTableManager::instance()->tableModel();
}

void TaskWidget::initUI()
{

}

void TaskWidget::enterLogBefore()
{
    qDebug() << "刷新任务列表";

    updateTableModel("");
    updateTableWidget(0, PAGE_NUM);
}

void TaskWidget::updateTableModel(const QString &strFilter)
{
    if (!strFilter.isEmpty())
    {
        m_tableModel->setFilter(strFilter);
    }

    if (!m_tableModel->select())
    {
        qWarning() << "任务数据模型更新失败：" << m_tableModel->lastError().text();
    }

    while (m_tableModel->canFetchMore())
    {
        m_tableModel->fetchMore();
    }
}

void TaskWidget::updateTableWidget(int beginRow, int endRow)
{
    // //清空样本列表
    // if (ui->tableWidget->rowCount() != 0)
    // {
    //     ui->tableWidget->clearContents();
    //     ui->tableWidget->setRowCount(0);
    // }

    // int total = m_tableModel->rowCount();
    // if (total <= 0)
    // {
    //     qWarning() << "任务数据模型为空";
    //     return;
    // }

    // //获取样本数据
    // QQueue<QSqlRecord> records;
    // QSqlRecord record;
    // while (beginRow != endRow)
    // {
    //     --endRow;
    //     record = m_tableModel->record(endRow);
    //     if (!record.isNull(0))
    //     {
    //         records << record;
    //     }
    // }

    // if (0 == records.size())
    // {
    //     qWarning() << "从任务数据模型获取记录失败";
    //     return;
    // }

    // //填充样本列表
    // ui->tableWidget->setRowCount(records.size());
    // for (int row = 0; row < records.size(); ++row)
    // {
    //     record = records.at(records.size() - row - 1);

    //     ui->tableWidget->setItem(row, 0, new AlignTableWidgetItem(record.value(1).toString()));
    //     ui->tableWidget->setItem(row, 1, new AlignTableWidgetItem(record.value(2).toString()));
    //     ui->tableWidget->setItem(row, 2, new AlignTableWidgetItem(record.value(3).toString()));
    //     ui->tableWidget->setItem(row, 3, new AlignTableWidgetItem(record.value(4).toString()));
    // }
}
