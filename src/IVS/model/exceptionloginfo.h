#ifndef EXCEPTIONLOGINFO_H
#define EXCEPTIONLOGINFO_H

#include <QString>

class ExceptionLogInfo
{
private:
    int log_id;
    QString exception_desc;
    QString admin_name;
    QString video_path;
    QString operate_time;

public:
    ExceptionLogInfo();
    int getLog_id() const;
    void setLog_id(int newLog_id);
    QString getException_desc() const;
    void setException_desc(const QString &newException_desc);
    QString getAdmin_name() const;
    void setAdmin_name(const QString &newAdmin_name);
    QString getVideo_path() const;
    void setVideo_path(const QString &newVideo_path);
    QString getOperate_time() const;
    void setOperate_time(const QString &newOperate_time);
};

#endif // EXCEPTIONLOGINFO_H
