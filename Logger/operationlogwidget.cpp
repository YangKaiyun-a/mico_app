#include "operationlogwidget.h"
#include "ui_operationlogwidget.h"
#include "db/logtablemanager.h"
#include "aligntablewidgetitem.h"


const int PAGE_NUM = 10;    ///< 一页中显示的记录条数

OperationLogWidget::OperationLogWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::OperationLogWidget)
{
    ui->setupUi(this);
    init();
}

OperationLogWidget::~OperationLogWidget()
{
    delete ui;
}

void OperationLogWidget::init()
{
    initData();
    initUI();
}

void OperationLogWidget::initData()
{
    m_tableModel = LOGTableManager->tableModel(ENUM_CLASS::LOG_INDEX::OPERATION);
}

void OperationLogWidget::initUI()
{
    QStringList headers;
    headers << "操作模块" << "操作类型" << "操作者" << "日期";

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

void OperationLogWidget::enterLogBefore()
{
    qDebug() << "刷新操作日志";

    updateTableModel("");
    updateTableWidget(0, PAGE_NUM);
}

void OperationLogWidget::updateTableModel(const QString &strFilter)
{
    if(!m_tableModel)
    {
        return;
    }

    if (!strFilter.isEmpty())
    {
        m_tableModel->setFilter(strFilter);
    }

    if (!m_tableModel->select())
    {
        qWarning() << "日志数据模型更新失败：" << m_tableModel->lastError().text();
    }

    while (m_tableModel->canFetchMore())
    {
        m_tableModel->fetchMore();
    }
}

void OperationLogWidget::updateTableWidget(int beginRow, int endRow)
{
    if(!m_tableModel)
    {
        return;
    }

    //清空样本列表
    if (ui->tableWidget->rowCount() != 0)
    {
        ui->tableWidget->clearContents();
        ui->tableWidget->setRowCount(0);
    }

    int total = m_tableModel->rowCount();
    if (total <= 0)
    {
        qWarning() << "日志数据模型为空";
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
        qWarning() << "从日志数据模型获取记录失败";
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
    }
}









