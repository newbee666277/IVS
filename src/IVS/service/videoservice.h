#ifndef VIDEOSERVICE_H
#define VIDEOSERVICE_H
#include "../model/videomodel.h"
#include "../model/normalvideoinfo.h"
#include "../model/exceptionvideoinfo.h"

class VideoService
{
public:
    VideoService();
    //更新列表内容
    void update_list_normal(QList<NormalVideoInfo> &dates, QString &lab_page, QString selected_date,
                     int &current_page, int &total_pages, int &page_size, int channelid);

    void update_list_exception(QList<ExceptionVideoInfo> &dates, QString &lab_page, QString selected_date,
                            int &current_page, int &total_pages, int &page_size, int channelid);
    //获取总页数
    int get_total_pages(QString selected_date, int page_size, int channel_id);
    //初始化日期选择下拉框(旧)
    void init_dates_com(QStringList &dates);
};

#endif // VIDEOSERVICE_H
