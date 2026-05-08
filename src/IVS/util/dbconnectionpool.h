#ifndef DBCONNECTIONPOOL_H
#define DBCONNECTIONPOOL_H

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QMutex>
#include <QSettings>
#include <QThread>
#include <QUuid>

class DbConnectionPool
{
public:
    static DbConnectionPool* getInstance();
    QSqlDatabase getConnection();
    void releaseConnection();

private:
    static DbConnectionPool* instance;
    DbConnectionPool(); //私有构造

    DbConnectionPool(const DbConnectionPool&) = delete;
    DbConnectionPool& operator=(const DbConnectionPool&) = delete;
    ~DbConnectionPool();

    bool isConnectionValid(const QString& connnectName); //连接是否合法

    QMutex mutex;
    QHash<Qt::HANDLE, QString> threadCurrentConn; //保存连接的容器 （线程ID = 连接名）
    QString hostname;
    QString dbname;
    QString username;
    QString password;
    int port;
};

#endif // DBCONNECTIONPOOL_H
