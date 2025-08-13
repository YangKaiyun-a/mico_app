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
        qDebug() << "错误信息：" << m_tableModel->lastError().text();
    }
    //模型数据排序
    m_tableModel->setSort(m_tableModel->fieldIndex("import_time"), Qt::DescendingOrder);
}







/****************************** LogTableManager 类，操作日志、系统日志的对外接口*******************************/

LogTableManager::LogTableManager(QObject *parent) : QObject(parent)
{

}

LogTableManager::~LogTableManager()
{

}

// 初始化数据库表
void LogTableManager::initTables()
{
    m_operationTable = new OperationLogTable(this);
    m_operationTable->initTableParam();
    m_operationTable->initTableModel();
}

LogTableManager *LogTableManager::instance()
{
    static LogTableManager instance;
    return &instance;
}

// 获取最底层的 DataBaseTable::m_tableModel
QSqlTableModel *LogTableManager::tableModel(ENUM_CLASS::LOG_INDEX index)
{
    QSqlTableModel *tableModel = nullptr;

    switch(index)
    {
        case ENUM_CLASS::LOG_INDEX::OPERATION:
            tableModel = m_operationTable->tableModel();
        default:
            break;
    }

    return tableModel;
}
