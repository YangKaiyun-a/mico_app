#include "dbmanager.h"


DBManager *DBManager::m_dbManager =  nullptr;

DBManager::DBManager(QObject *parent)
    : QObject{parent}
{

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
