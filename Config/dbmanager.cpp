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
    QSqlDatabase db = getDatabaseConnection("database");
    if(db.isValid() && db.open())
    {
        qDebug() << "数据库连接成功";
        return true;
    }

    qDebug() << "数据库连接失败：" << db.lastError();

    return false;
}

// 根据 模块+线程 获取连接名
QSqlDatabase DBManager::getDatabaseConnection(const QString &name)
{
    QString connectionName = QString("%1_DBConnection_%2").arg(name).arg((quintptr)QThread::currentThreadId());

    if(!QSqlDatabase::contains(connectionName))
    {
        if(!SettingFileCreate::instance()->sysValue(name, "db_connected").toBool())
        {
            return QSqlDatabase();
        }

        QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL", connectionName);
        db.setHostName(SettingFileCreate::instance()->sysValue(name, "db_Host").toString());
        db.setPort(SettingFileCreate::instance()->sysValue(name, "db_Port").toInt());
        db.setDatabaseName(SettingFileCreate::instance()->sysValue(name, "db_Name").toString());
        db.setUserName(SettingFileCreate::instance()->sysValue(name, "db_User").toString());
        db.setPassword(SettingFileCreate::instance()->sysValue(name, "db_PWD").toString());
        return db;
    }

    return QSqlDatabase::database(connectionName);
}
