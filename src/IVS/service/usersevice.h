#ifndef USERSEVICE_H
#define USERSEVICE_H
#include <QString>
#include "../model/usermodel.h"

class UserSevice
{
public:
    UserSevice();
    int login(QString &user_name, QString &pwd);
    bool reg(QString &user_name, QString &pwd);
    bool check_repeat_name(QString &user_name);
private:
    UserModel usermodel;
};

#endif // USERSEVICE_H
