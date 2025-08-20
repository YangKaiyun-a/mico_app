/* 操作日志、系统日志的对外接口
 *
 *
 */

#ifndef LOGTABLEMANAGER_H
#define LOGTABLEMANAGER_H

#include "operationlogtable.h"
#include "global.h"

#include <QObject>

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
    OperationLogTable* m_operationTable = nullptr;          ///< OperationLogTable 对象
    bool m_needUpdate;                                      ///< 需要更新
};

#define LOGTableManager LogTableManager::instance()

#endif // LOGTABLEMANAGER_H
