#include "operationlogtable.h"
#include "dbmanager.h"

OperationLogTable::OperationLogTable(QObject *parent)
    : DataBaseTable{parent}
{

}

OperationLogTable::~OperationLogTable()
{
    deleteLater();
}

void OperationLogTable::initTableParam()
{
    m_tableName = QString("operation_log");
    m_addRecordCmdHead = QString(
        "INSERT INTO " + m_tableName +
        " (module, type, operator, date)"
        " VALUES(:module, :type, :operator, :date)");
}

void OperationLogTable::initTableModel()
{
    if(m_tableModel != nullptr)
    {
        m_tableModel->clear();
    }
    m_tableModel = new QSqlTableModel(this, DbManager->getDatabaseConnection());
    m_tableModel->setTable(m_tableName);
    //设置模型提交策略，手动提交
    m_tableModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    //填充模型数据
    if (!m_tableModel->select())
    {
        qDebug() << QString("从 %1 表中获取数据失败！").arg(m_tableName);
        qDebug() << "日志数据模型更新失败：" << m_tableModel->lastError().text();
    }
    //模型数据排序
    m_tableModel->setSort(m_tableModel->fieldIndex("date"), Qt::DescendingOrder);
}
