#ifndef NORMALLOGINFO_H
#define NORMALLOGINFO_H

#include <QString>

class NormalLogInfo
{
private:
    int log_id;
    QString admin_name;
    QString operate_time;
    QString operate_func;
public:
    NormalLogInfo();
    QString getAdmin_name() const;
    void setAdmin_name(const QString &newAdmin_name);
    QString getOperate_time() const;
    void setOperate_time(const QString &newOperate_time);
    QString getOperate_func() const;
    void setOperate_func(const QString &newOperate_func);
    int getLog_id() const;
    void setLog_id(int newLog_id);
};

#endif // NORMALLOGINFO_H
