#ifndef LOGMODEL_H
#define LOGMODEL_H

#include <QString>
#include <QList>
#include "../util/dbconn.h"
#include "../model/exceptionloginfo.h"
#include "../model/normalloginfo.h"
#include <variant>
#include <QDebug>
#include <QVariant>
#include <QDateTime>

class LogModel
{
public:
    LogModel();
    void insertNormalLogInfo(int admin_id, QString admin_name, QString operate_time, QString operate_func, QString operate_desc = "正常操作");
    std::variant<QList<NormalLogInfo>, QList<ExceptionLogInfo>> queryLogInfo(bool isNormal, int offset, int page_size);
    int query_total_records(bool isNormal);
};

#endif // LOGMODEL_H
