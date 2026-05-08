#include "dbconnectionpool.h"
DbConnectionPool* DbConnectionPool::instance = nullptr;

DbConnectionPool* DbConnectionPool::getInstance()
{
    static QMutex mutex;
    if(DbConnectionPool::instance == nullptr){
        mutex.lock();
        if(DbConnectionPool::instance == nullptr){
            DbConnectionPool::instance = new DbConnectionPool;
        }
        mutex.unlock();
    }

    return DbConnectionPool::instance;
}

QSqlDatabase DbConnectionPool::getConnection()
{
    QMutexLocker locker(&mutex);
    //获取当前线程ID
    Qt::HANDLE currentId = QThread::currentThreadId();
    if(threadCurrentConn.contains(currentId)){
        QString conn_name = threadCurrentConn[currentId];
        if(isConnectionValid(conn_name)){
            QSqlDatabase db = QSqlDatabase::database(conn_name);
            return db;
        }else{
            threadCurrentConn.remove(currentId);
        }
    }

    QString conn_name = QUuid::createUuid().toString();
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL", conn_name);
    db.setHostName(this->hostname);
    db.setDatabaseName(this->dbname);
    db.setUserName(this->username);
    db.setPassword(this->password);
    db.setPort(this->port);
    if(db.open()){
        qDebug() << "新数据库已创建";
        this->threadCurrentConn[currentId] = conn_name;
        return db;
    }

    return QSqlDatabase();
}

void DbConnectionPool::releaseConnection()
{
    QMutexLocker locker(&mutex);
    Qt::HANDLE currentId = QThread::currentThreadId();
    if(!threadCurrentConn.contains(currentId)){
        return;
    }

    QString conn_name = threadCurrentConn[currentId];
    if(QSqlDatabase::contains(conn_name)){
        QSqlDatabase::removeDatabase(conn_name);
    }
    this->threadCurrentConn.remove(currentId);
}

//构造方法（加载配置，配置文件.ini）
DbConnectionPool::DbConnectionPool() {
    QSettings settings("db_config.ini", QSettings::IniFormat);
    this->hostname = settings.value("hostname").toString();
    this->dbname = settings.value("dbname").toString();
    this->username = settings.value("username").toString();
    this->password = settings.value("password").toString();
    this->port = settings.value("port").toInt();

}

//释放连接池中的所有连接
DbConnectionPool::~DbConnectionPool()
{
    QMutexLocker locker(&mutex);
    //遍历容器的所有键（线程的ID）
    for(auto cid : this->threadCurrentConn.keys()){
        //获取当前的连接名
        QString connName = threadCurrentConn[cid];
        //判断数据库中是否有当前的连接
        if(QSqlDatabase::contains(connName)){
            QSqlDatabase::removeDatabase(connName);
        }

    }

    this->threadCurrentConn.clear();
}

bool DbConnectionPool::isConnectionValid(const QString &connnectName)
{
    if(!QSqlDatabase::contains(connnectName)){
        return false;
    }

    QSqlDatabase db = QSqlDatabase::database(connnectName);
    if(!db.isOpen()){
        return db.open(); //如果没有打开，尝试再打开一次
    }

    //已经打开，执行最简单的sql语句测试
    QSqlQuery query(db);
    return query.exec("SELECT 1");
}


