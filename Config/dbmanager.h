#ifndef DBMANAGER_H
#define DBMANAGER_H

#include "global.h"

#include <QObject>
#include <QSqlDatabase>


class DBManager : public QObject
{
    Q_OBJECT
public:
    static DBManager *instance();
    void init();
    bool connect();
    QSqlDatabase getDatabaseConnection();               ///< 根据 线程 获取连接名
    bool getRobotShapeConfig(int id);                   ///< 获取机器人形状
    bool getMotorStatus();                              ///< 获取电机信息

private:
    explicit DBManager(QObject *parent = nullptr);
    ~DBManager();

private:
    static DBManager* m_dbManager;

signals:
};

#define DbManager DBManager::instance()

#endif // DBMANAGER_H
