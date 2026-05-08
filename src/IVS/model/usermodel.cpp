#include "usermodel.h"

UserModel::UserModel()
{

}

bool UserModel::queryUserNameAndPwd(QString &user_name, QString &pwd, int &id)
{
    //检查用户名或密码
    DbConn* db_instance = DbConn::getInstance();
    QSqlDatabase db = db_instance->getDb();
    QString sql = "SELECT * FROM admin_user WHERE user_name = :name AND password = :pwd;";

    QSqlQuery query(db);

    //预编译处理
    query.prepare(sql);
    query.bindValue(":name", user_name);
    query.bindValue(":pwd", db_instance->getMD5(pwd));

    //执行
    if(query.exec()){
        if(query.next()){
            id = query.value(0).toInt();
            return true;
        }

    }

    return false;

}

int UserModel::updateLoginTime(int &id)
{

        QSqlDatabase db = DbConn::getInstance()->getDb();

        //打开连接
        if(!db.open()){
            qDebug() << "数据库打开失败" << db.lastError().text();
            return -1;
        }

        qDebug() << "数据库打开成功";

        QString sql = "UPDATE user_info SET last_login_time = :cur_time WHERE id = :id";

        QSqlQuery query(db);

        //预编译处理
        query.prepare(sql);
        query.bindValue(":cur_time", QDateTime::currentDateTime());
        query.bindValue(":id", id);

        //执行
        if(query.exec()){
            return query.numRowsAffected();
        }
        qDebug() << "更新用户登录时间执行失败" << query.lastError().text();
        return -1;
}

bool UserModel::reg(QString user_name, QString password)
{
    QSqlDatabase db = DbConn::getInstance()->getDb();
    QSqlQuery query(db);
    QString sql = "INSERT INTO admin_user(user_name, password, status) VALUES(:user_name, MD5(:password), :status);";
    query.prepare(sql);
    query.bindValue(":user_name", user_name);
    query.bindValue(":password", password);
    query.bindValue(":status", 1);

    if(!query.exec()){
        qDebug() << "用户注册数据插入数据库失败" << query.lastError().text();
        return false;
    }

    return true;
}

bool UserModel::query_repeat_name(QString user_name)
{
    QSqlDatabase db = DbConn::getInstance()->getDb();
    QSqlQuery query(db);
    QString sql = "SELECT * FROM admin_user WHERE user_name = :name;";
    query.prepare(sql);
    query.bindValue(":name", user_name);

    if(!query.exec()){
        qDebug() << "查询重复用户名操作失败" << query.lastError().text();
        return false;
    }

    if(query.next()){
        return true;  //有重复用户名
    }else{
        return false;
    }
}
