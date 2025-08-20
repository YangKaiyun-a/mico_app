/* 数据库层次的中间层
 * 对 DataBaseTable::m_tableModel 进行封装
 * 通过 DataBaseTable::m_tableModel 操作数据库
 */

#ifndef TASKFINISHEDTABLE_H
#define TASKFINISHEDTABLE_H

#include "databasetable.h"

#include <QObject>

class TaskFinishedTable : public DataBaseTable
{
    Q_OBJECT
public:
    explicit TaskFinishedTable(QObject *parent = nullptr);
    ~TaskFinishedTable();

    void initTableParam() override;
    void initTableModel() override;

signals:
};

#endif // TASKFINISHEDTABLE_H
