#include "tasktablemanager.h"

TaskTableManager::TaskTableManager(QObject *parent)
    : QObject{parent}
{

}

TaskTableManager::~TaskTableManager()
{

}

// 初始化数据库表
void TaskTableManager::initTables()
{
    m_taskTable = new TaskTable(this);
    m_taskTable->initTableParam();
    m_taskTable->initTableModel();
}

TaskTableManager *TaskTableManager::instance()
{
    static TaskTableManager instance;
    return &instance;
}

// 获取最底层的 DataBaseTable::m_tableModel
QSqlTableModel *TaskTableManager::tableModel()
{
    if(!m_taskTable)
    {
        return nullptr;
    }

    QSqlTableModel *tableModel = nullptr;
    tableModel = m_taskTable->tableModel();
    return tableModel;
}
