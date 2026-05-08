#ifndef USERMODEL_H
#define USERMODEL_H
#include <QString>
#include <QDateTime>
#include "../util/dbconn.h"

class UserModel
{
public:
    UserModel();
    bool queryUserNameAndPwd(QString &user_name, QString &pwd, int &id);
    int updateLoginTime(int &id);
    bool reg(QString user_name, QString password);
    bool query_repeat_name(QString user_name);
};

#endif // USERMODEL_H
