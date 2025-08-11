#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QObject>
#include <QSqlDatabase>


class DBManager : public QObject
{
    Q_OBJECT
public:
    static DBManager *instance();
    void init();
    bool connect();
    QSqlDatabase getDatabaseConnection(const QString& name);

private:
    explicit DBManager(QObject *parent = nullptr);
    ~DBManager();

private:
    static DBManager* m_dbManager;

signals:
};

#define DbManager DBManager::instance()

#endif // DBMANAGER_H
