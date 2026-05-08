#include "logtask.h"

LogTask::LogTask(QObject *parent)
    : QObject{parent}
{}

void LogTask::recordLog(int admin_id, QString admin_name, QString operate_time, QString operate_func)
{
    LogModel lm;
    lm.insertNormalLogInfo(admin_id, admin_name, operate_time, operate_func);
}

std::variant<QList<NormalLogInfo>, QList<ExceptionLogInfo>> LogTask::fill_table(bool isNormal, QString &lab_page, int &cur_page, int &total_pages, int page_size)
{
    LogModel lm;
    lab_page = QString("%1/%2").arg(cur_page).arg(total_pages);
    int offset = (cur_page - 1) * page_size;
    return lm.queryLogInfo(isNormal, offset, page_size);
}

int LogTask::query_total_pages(bool isNormal, int page_size)
{
    LogModel lm;
    int total_records = 0;
    total_records = lm.query_total_records(isNormal);
    if(total_records == 0){
        return 1;
    }

    return (total_records % page_size) == 0 ? (total_records / page_size) : (total_records / page_size) + 1;

}
