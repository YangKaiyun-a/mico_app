#include "taskfinishedwidget.h"
#include "ui_taskfinishedwidget.h"
#include "tasktablemanager.h"
#include "aligntablewidgetitem.h"

const int PAGE_NUM = 10;    ///< 一页中显示的记录条数

TaskFinishedWidget::TaskFinishedWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TaskFinishedWidget)
{
    ui->setupUi(this);
    init();
}

TaskFinishedWidget::~TaskFinishedWidget()
{
    delete ui;
}

void TaskFinishedWidget::init()
{
    initData();
    initUI();
}

void TaskFinishedWidget::initData()
{
    m_tableModel = TaskTableManager::instance()->tableModel();
}

void TaskFinishedWidget::initUI()
{
    QStringList headers;
    headers << "流程名称" << "创建时间" << "创建者" << "状态" << "执行机器人";

    ui->tableWidget->setColumnCount(headers.size());
    ui->tableWidget->setHorizontalHeaderLabels(headers);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setAlternatingRowColors(true);

    for(int i = 0; i < headers.size(); ++i)
    {
        ui->tableWidget->horizontalHeader()->setSectionResizeMode(i, QHeaderView::Fixed);
    }

    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void TaskFinishedWidget::enterLogBefore()
{
    qDebug() << "刷新任务列表";

    updateTableModel("");
    updateTableWidget(0, PAGE_NUM);
}

void TaskFinishedWidget::updateTableModel(const QString &strFilter)
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

void TaskFinishedWidget::updateTableWidget(int beginRow, int endRow)
{
    //清空样本列表
    if (ui->tableWidget->rowCount() != 0)
    {
        ui->tableWidget->clearContents();
        ui->tableWidget->setRowCount(0);
    }

    int total = m_tableModel->rowCount();
    if (total <= 0)
    {
        qWarning() << "任务数据模型为空";
        return;
    }

    //获取样本数据
    QQueue<QSqlRecord> records;
    QSqlRecord record;
    while (beginRow != endRow)
    {
        --endRow;
        record = m_tableModel->record(endRow);
        if (!record.isNull(0))
        {
            records << record;
        }
    }

    if (0 == records.size())
    {
        qWarning() << "从任务数据模型获取记录失败";
        return;
    }

    //填充样本列表
    ui->tableWidget->setRowCount(records.size());
    for (int row = 0; row < records.size(); ++row)
    {
        record = records.at(records.size() - row - 1);

        ui->tableWidget->setItem(row, 0, new AlignTableWidgetItem(record.value(1).toString()));
        ui->tableWidget->setItem(row, 1, new AlignTableWidgetItem(record.value(2).toString()));
        ui->tableWidget->setItem(row, 2, new AlignTableWidgetItem(record.value(3).toString()));
        ui->tableWidget->setItem(row, 3, new AlignTableWidgetItem(record.value(4).toString()));
        ui->tableWidget->setItem(row, 4, new AlignTableWidgetItem(record.value(5).toString()));
    }
}
