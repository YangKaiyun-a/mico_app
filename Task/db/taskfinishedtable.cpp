#include "taskfinishedtable.h"
#include "dbmanager.h"

TaskFinishedTable::TaskFinishedTable(QObject *parent)
    : DataBaseTable{parent}
{

}

TaskFinishedTable::~TaskFinishedTable()
{
    deleteLater();
}

void TaskFinishedTable::initTableParam()
{
    m_tableName = QString("workflow_config");
    m_addRecordCmdHead = QString(
        "INSERT INTO " + m_tableName +
        " (workflow_name, create_time, create_by, status, robot_id)"
        " VALUES(:workflow_name, :create_time, :create_by, :status, :robot_id)");
}

void TaskFinishedTable::initTableModel()
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
        qDebug() << "任务数据模型更新失败：" << m_tableModel->lastError().text();
    }
    //模型数据排序
    m_tableModel->setSort(m_tableModel->fieldIndex("create_time"), Qt::DescendingOrder);
}
