#ifndef OPERATIONLOGTABLE_H
#define OPERATIONLOGTABLE_H

#include "databasetable.h"

#include <QObject>

// 数据库层次的中间层，对 DataBaseTable::m_tableModel 进行封装
class OperationLogTable : public DataBaseTable
{
    Q_OBJECT
public:
    explicit OperationLogTable(QObject *parent = nullptr);
    ~OperationLogTable();

    void initTableParam() override;
    void initTableModel() override;
};


#endif // OPERATIONLOGTABLE_H
