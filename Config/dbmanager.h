#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QObject>

class DBManager : public QObject
{
    Q_OBJECT
public:
    static DBManager  *instance();

private:
    explicit DBManager(QObject *parent = nullptr);
    ~DBManager();

private:
    static DBManager* m_dbManager;

signals:
};

#endif // DBMANAGER_H
