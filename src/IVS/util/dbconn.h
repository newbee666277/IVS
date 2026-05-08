#ifndef DBCONN_H
#define DBCONN_H
#include <QSqlDatabase>
#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>
#include <QCryptographicHash>
#include <QMutex>

class DbConn
{
public:
    DbConn(const DbConn&) = delete;
    DbConn& operator=(const DbConn&) = delete;

    static DbConn* getInstance();
    static void releaseInstance();
    QSqlDatabase getDb() const;
    QString getMD5(const QString &pwd);

private:
    DbConn();
    ~DbConn();
    static DbConn* instance;
    QSqlDatabase db;
};

#endif // DBCONN_H
