/* 数据库层次的中间层
 * 对 DataBaseTable::m_tableModel 进行封装
 * 通过 DataBaseTable::m_tableModel 操作数据库
 */

#ifndef TASKTABLE_H
#define TASKTABLE_H

#include "databasetable.h"

#include <QObject>

class TaskTable : public DataBaseTable
{
    Q_OBJECT
public:
    explicit TaskTable(QObject *parent = nullptr);
    ~TaskTable();

    void initTableParam() override;
    void initTableModel() override;
};

#endif // TASKTABLE_H
