#include "normalloginfo.h"

QString NormalLogInfo::getAdmin_name() const
{
    return admin_name;
}

void NormalLogInfo::setAdmin_name(const QString &newAdmin_name)
{
    admin_name = newAdmin_name;
}

QString NormalLogInfo::getOperate_time() const
{
    return operate_time;
}

void NormalLogInfo::setOperate_time(const QString &newOperate_time)
{
    operate_time = newOperate_time;
}

QString NormalLogInfo::getOperate_func() const
{
    return operate_func;
}

void NormalLogInfo::setOperate_func(const QString &newOperate_func)
{
    operate_func = newOperate_func;
}

int NormalLogInfo::getLog_id() const
{
    return log_id;
}

void NormalLogInfo::setLog_id(int newLog_id)
{
    log_id = newLog_id;
}

NormalLogInfo::NormalLogInfo() {}
