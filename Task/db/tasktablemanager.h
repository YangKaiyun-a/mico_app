/* 任务表的对外接口
 *
 *
 */

#ifndef TASKTABLEMANAGER_H
#define TASKTABLEMANAGER_H

#include "tasklisttable.h"
#include "taskfinishedtable.h"
#include "global.h"

#include <QObject>

class TaskTableManager : public QObject
{
    Q_OBJECT
public:
    ~TaskTableManager();
    void initTables();                                              ///< 初始化数据库表
    static TaskTableManager *instance();                            ///< 获取单例指
    QSqlTableModel *tableModel(ENUM_CLASS::TASK_INDEX index);       ///< 获取最底层的 DataBaseTable::m_tableModel

private:
    explicit TaskTableManager(QObject *parent = nullptr);

private:
    TaskListTable* m_taskListTable = nullptr;           ///< TaskTable 对象
    TaskFinishedTable *m_taskFinishedTable = nullptr;   ///< TaskFinishedTable 对象
    bool m_needUpdate;                                  ///< 需要更新
};

#endif // TASKTABLEMANAGER_H
