/* 任务列表的对外接口
 *
 *
 */

#ifndef TASKTABLEMANAGER_H
#define TASKTABLEMANAGER_H

#include "tasktable.h"
#include "global.h"

#include <QObject>

class TaskTableManager : public QObject
{
    Q_OBJECT
public:
    ~TaskTableManager();
    void initTables();                              ///< 初始化数据库表
    static TaskTableManager *instance();            ///< 获取单例指
    QSqlTableModel *tableModel();                   ///< 获取最底层的 DataBaseTable::m_tableModel

private:
    explicit TaskTableManager(QObject *parent = nullptr);

private:
    TaskTable* m_taskTable = nullptr;               ///< TaskTable 对象
    bool m_needUpdate;                              ///< 需要更新
};

#endif // TASKTABLEMANAGER_H
