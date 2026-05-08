#include "videoservice.h"
#include "../view/mainwidget.h"

VideoService::VideoService()
{

}

void VideoService::update_list_normal(QList<NormalVideoInfo> &dates, QString &lab_page, QString selected_date,
                               int &current_page, int &total_pages, int &page_size, int channelid)
{
    dates.clear(); //清空列表
    lab_page = QString("%1/%2").arg(current_page).arg(total_pages); //页数显示 1/2
    int offset = (current_page - 1) * page_size;
    //QString selected_date = dates_com->currentText();
    VideoModel vd;
    vd.fill_list_normal(dates, selected_date, offset, page_size, channelid);

}

void VideoService::update_list_exception(QList<ExceptionVideoInfo> &dates, QString &lab_page, QString selected_date,
                                      int &current_page, int &total_pages, int &page_size, int channelid)
{
    dates.clear(); //清空列表
    lab_page = QString("%1/%2").arg(current_page).arg(total_pages); //页数显示 1/2
    int offset = (current_page - 1) * page_size;
    //QString selected_date = dates_com->currentText();
    VideoModel vd;
    vd.fill_list_exception(dates, selected_date, offset, page_size, channelid);

}

int VideoService::get_total_pages(QString selected_date, int page_size, int channel_id)
{
    //QString selected_date = dates_com->currentText();
    VideoModel vd;
    int total_records = 0;
    if(MainWidget::isNormalReview){
        total_records = vd.query_total_normal(selected_date, channel_id);
    }else{
        total_records = vd.query_total_exception(selected_date, channel_id);
    }
    if(total_records < page_size){
        return 1;
    }else{
        return (total_records%page_size) ==0 ? (total_records/page_size) : (total_records/page_size)+1;
    }

}

void VideoService::init_dates_com(QStringList &dates)
{
    VideoModel vm;
    vm.fill_date_com(dates);
}
