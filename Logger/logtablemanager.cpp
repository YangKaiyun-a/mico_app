#include "logtablemanager.h"

LogTableManager::LogTableManager(QObject *parent) : QObject(parent)
{

}

LogTableManager::~LogTableManager()
{

}

// 初始化数据库表
void LogTableManager::initTables()
{
    // 操作日志
    m_operationTable = new OperationLogTable(this);
    m_operationTable->initTableParam();
    m_operationTable->initTableModel();

    // 系统日志
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
        break;
    case ENUM_CLASS::LOG_INDEX::SYSTEM:
        break;
    default:
        break;
    }

    return tableModel;
}
