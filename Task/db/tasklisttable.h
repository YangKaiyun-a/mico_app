/* 数据库层次的中间层
 * 对 DataBaseTable::m_tableModel 进行封装
 * 通过 DataBaseTable::m_tableModel 操作数据库
 */

#ifndef TASKLISTTABLE_H
#define TASKLISTTABLE_H

#include "databasetable.h"

#include <QObject>

class TaskListTable : public DataBaseTable
{
    Q_OBJECT
public:
    explicit TaskListTable(QObject *parent = nullptr);
    ~TaskListTable();

    void initTableParam() override;
    void initTableModel() override;
};

#endif // TASKLISTTABLE_H
