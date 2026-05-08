#ifndef LOGTASK_H
#define LOGTASK_H

#include <QObject>
#include "../model/logmodel.h"
#include "../model/exceptionloginfo.h"
#include "../model/normalloginfo.h"
#include <variant>

class LogTask : public QObject
{
    Q_OBJECT
public:
    explicit LogTask(QObject *parent = nullptr);
    void recordLog(int admin_id, QString admin_name, QString operate_time, QString operate_func);
    std::variant<QList<NormalLogInfo>, QList<ExceptionLogInfo>> fill_table(bool isNormal, QString &lab_page, int &cur_page, int &total_pages, int page_size);
    int query_total_pages(bool isNormal, int page_size);
signals:

};

#endif // LOGTASK_H
