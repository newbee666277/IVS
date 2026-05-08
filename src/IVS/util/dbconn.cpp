#include "dbconn.h"
DbConn* DbConn::instance = nullptr;
QMutex mutex;
DbConn *DbConn::getInstance()
{
    if(DbConn::instance == nullptr){
        QMutexLocker locker(&mutex);
        if(DbConn::instance == nullptr){
            DbConn::instance = new DbConn;
        }
    }

    return DbConn::instance;
}

void DbConn::releaseInstance()
{
    if(DbConn::instance != nullptr){
        delete DbConn::instance;
        DbConn::instance = nullptr;
    }
}

DbConn::DbConn()
{
    //注册MYSQL驱动
    db = QSqlDatabase::addDatabase("QMYSQL");

    //设置连接参数
    db.setHostName("localhost");
    db.setPort(3306);
    db.setUserName("root");
    db.setPassword("123456");
    db.setDatabaseName("monitor_db");

    //打开连接
    if(!db.open()){
        qDebug() << "数据库打开失败" << db.lastError().text();
    }

    qDebug() << "数据库打开成功";
}

DbConn::~DbConn()
{
    qDebug() << "释放数据库连接";
    if(this->db.isOpen()){
        db.close();
        QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
    }
}

QSqlDatabase DbConn::getDb() const
{
    return db;
}

QString DbConn::getMD5(const QString &pwd){
    QByteArray byte_arr = pwd.toUtf8();
    QByteArray byteMD5 = QCryptographicHash::hash(byte_arr, QCryptographicHash::Md5);
    QString md5 = byteMD5.toHex(); //转换成16进制
    return md5;
}


