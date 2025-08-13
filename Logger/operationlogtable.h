#ifndef OPERATIONLOGTABLE_H
#define OPERATIONLOGTABLE_H

#include "databasetable.h"
#include "global.h"

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




// 操作日志、系统日志的对外接口
class LogTableManager : public QObject
{
    Q_OBJECT
public:
    ~LogTableManager();
    void initTables();                                          ///< 初始化数据库表
    static LogTableManager *instance();                         ///< 获取单例指
    QSqlTableModel *tableModel(ENUM_CLASS::LOG_INDEX index);    ///< 获取最底层的 DataBaseTable::m_tableModel

private:
    explicit LogTableManager(QObject *parent = nullptr);

private:
    OperationLogTable* m_operationTable = nullptr;       ///< OperationLogTable 对象
    bool m_needUpdate;                                  ///< 需要更新
};

#define LOGTableManager LogTableManager::instance()

#endif // OPERATIONLOGTABLE_H
