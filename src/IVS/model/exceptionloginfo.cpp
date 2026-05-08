#include "exceptionloginfo.h"

int ExceptionLogInfo::getLog_id() const
{
    return log_id;
}

void ExceptionLogInfo::setLog_id(int newLog_id)
{
    log_id = newLog_id;
}

QString ExceptionLogInfo::getException_desc() const
{
    return exception_desc;
}

void ExceptionLogInfo::setException_desc(const QString &newException_desc)
{
    exception_desc = newException_desc;
}

QString ExceptionLogInfo::getAdmin_name() const
{
    return admin_name;
}

void ExceptionLogInfo::setAdmin_name(const QString &newAdmin_name)
{
    admin_name = newAdmin_name;
}

QString ExceptionLogInfo::getVideo_path() const
{
    return video_path;
}

void ExceptionLogInfo::setVideo_path(const QString &newVideo_path)
{
    video_path = newVideo_path;
}

QString ExceptionLogInfo::getOperate_time() const
{
    return operate_time;
}

void ExceptionLogInfo::setOperate_time(const QString &newOperate_time)
{
    operate_time = newOperate_time;
}

ExceptionLogInfo::ExceptionLogInfo() {}
