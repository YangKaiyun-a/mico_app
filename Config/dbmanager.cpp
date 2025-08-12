#include "dbmanager.h"
#include "settingfilecreate.h"

#include <QThread>
#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>


DBManager *DBManager::m_dbManager =  nullptr;

DBManager::DBManager(QObject *parent)
    : QObject{parent}
{
    init();
}

DBManager::~DBManager()
{

}

DBManager *DBManager::instance()
{
    if(nullptr == m_dbManager)
    {
        m_dbManager = new DBManager();
    }

    return m_dbManager;
}

void DBManager::init()
{
    connect();
}

bool DBManager::connect()
{
    QSqlDatabase db = getDatabaseConnection();
    if(db.isValid() && db.open())
    {
        qDebug() << "数据库连接成功";
        return true;
    }

    qDebug() << "数据库连接失败：" << db.lastError();

    return false;
}

// 根据 线程 获取连接名
QSqlDatabase DBManager::getDatabaseConnection()
{
    QString connectionName = QString("DBConnection_%1").arg((quintptr)QThread::currentThreadId());

    if(!QSqlDatabase::contains(connectionName))
    {
        if(!settingManager->sysValue("database", "db_connected").toBool())
        {
            return QSqlDatabase();
        }

        QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL", connectionName);
        db.setHostName(settingManager->sysValue("database", "db_Host").toString());
        db.setPort(settingManager->sysValue("database", "db_Port").toInt());
        db.setDatabaseName(settingManager->sysValue("database", "db_Name").toString());
        db.setUserName(settingManager->sysValue("database", "db_User").toString());
        db.setPassword(settingManager->sysValue("database", "db_PWD").toString());
        return db;
    }

    return QSqlDatabase::database(connectionName);
}

// 获取机器人形状
QString DBManager::getRobotShapeConfig(int id)
{
    QSqlDatabase db = getDatabaseConnection();
    QSqlQuery query(db);

    QString str = QString("SELECT robot_shape_cofig FROM robot WHERE id = 1");

    QString robotShapeConfig;

    if(query.exec(str))
    {
        if (query.next())
        {
            robotShapeConfig = query.value("robot_shape_cofig").toString();
        }
    }
    else
    {
        qDebug() << query.lastError();
    }

    return robotShapeConfig;
}
