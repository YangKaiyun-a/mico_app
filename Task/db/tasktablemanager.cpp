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
    m_taskListTable = new TaskListTable(this);
    m_taskListTable->initTableParam();
    m_taskListTable->initTableModel();

    m_taskFinishedTable = new TaskFinishedTable(this);
    m_taskFinishedTable->initTableParam();
    m_taskFinishedTable->initTableModel();
}

TaskTableManager *TaskTableManager::instance()
{
    static TaskTableManager instance;
    return &instance;
}

// 获取最底层的 DataBaseTable::m_tableModel
QSqlTableModel *TaskTableManager::tableModel(ENUM_CLASS::TASK_INDEX index)
{
    if(!m_taskListTable || !m_taskFinishedTable)
    {
        return nullptr;
    }

    QSqlTableModel *tableModel = nullptr;

    switch(index)
    {
    case ENUM_CLASS::TASK_INDEX::TASK_LIST:
        tableModel = m_taskListTable->tableModel();
        break;
    case ENUM_CLASS::TASK_INDEX::TASK_FINISHED:
        tableModel = m_taskFinishedTable->tableModel();
        break;
    default:
        break;
    }

    return tableModel;
}
