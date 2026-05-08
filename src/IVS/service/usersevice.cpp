#include "usersevice.h"

UserSevice::UserSevice()
{

}

int UserSevice::login(QString &user_name, QString &pwd)
{
    int id = 0;
    bool result = usermodel.queryUserNameAndPwd(user_name, pwd, id);
    if(result){
        int row = usermodel.updateLoginTime(id);
        if(row > 0){
            return id;
        }
    }

    return id;
}

bool UserSevice::reg(QString &user_name, QString &pwd)
{
    return usermodel.reg(user_name, pwd);
}

bool UserSevice::check_repeat_name(QString &user_name)
{
    return usermodel.query_repeat_name(user_name);
}
