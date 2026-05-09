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

    QSettings settings("db_config.ini", QSettings::IniFormat);
    settings.beginGroup("Database");
    this->hostname = settings.value("hostname").toString();
    this->dbname = settings.value("dbname").toString();
    this->username = settings.value("username").toString();
    this->password = settings.value("password").toString();
    this->port = settings.value("port").toInt();

    //设置连接参数
    db.setHostName(this->hostname);
    db.setDatabaseName(this->dbname);
    db.setUserName(this->username);
    db.setPassword(this->password);
    db.setPort(this->port);

    //打开连接
    if(!db.open()){
        qDebug() << "数据库打开失败" << db.lastError().text();
        return;
    }else{
        qDebug() << "数据库打开成功";
    }


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


