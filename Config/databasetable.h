#ifndef DATABASETABLE_H
#define DATABASETABLE_H

#include <QObject>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QSqlField>
#include <QSqlError>
#include <QSqlTableModel>
#include <QQueue>
#include <QMutex>
#include <QDebug>

class DataBaseTable : public QObject
{
    Q_OBJECT

public:
    explicit DataBaseTable(QObject *parent = nullptr);
    virtual ~DataBaseTable();

public:
    QString tableName() const;                                              ///< 获取表名
    QSqlTableModel *tableModel() const;                                     ///< 获取表的模型
    QSqlRecord emptyRecord();                                               ///< 获取表的一条空记录
    int counts();                                                           ///< 获取全部记录条数
    int specifiedCounts(const QString strConstraint);                       ///< 获取指定条件的记录条数
    bool addSingleRecord(const QSqlRecord& tRecord);                        ///< 增加单条记录
    bool addManyRecords(const QQueue<QSqlRecord>& tRecords);                ///< 增加多条记录
    bool clear();                                                           ///< 清除表的全部记录
    bool deleteSpecifiedRecord(const QString strConstraint);                ///< 删除指定条件的记录
    bool updateField(const QString strSet, const QString strConstraint);    ///< 更改记录某字段
    bool existField(const QSqlField tField);
    bool queryAllRecords(QList<QSqlRecord>& RecordQueue);                   ///< 查询全部记录
    bool querySpecifiedRecords(QList<QSqlRecord>& RecordQueue, const QString strConstraint);    ///< 查询指定记录

    virtual void initTableParam() = 0;                                      ///< 初始化表的参数
    virtual void initTableModel() = 0;                                      ///< 初始化表的数据模型
    virtual void initTableRecords();                                        ///< 初始化表的记录

public:
    QString m_tableName;                        ///< 表名
    QString m_addRecordCmdHead;                 ///< 添加记录的命令头
    volatile bool m_modelNeedUpdate;            ///< 是否需要更新模型
    QSqlTableModel *m_tableModel = nullptr;     ///< 表模型
    QMutex m_mutex;
};

#endif // DATABASETABLE_H
