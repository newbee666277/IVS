#include "videomodel.h"

VideoModel::VideoModel()
{

}

void VideoModel::fill_list_normal(QList<NormalVideoInfo> &data, QString &selected_date, int &offset, int &page_size, int channelid)
{
    QSqlDatabase db = DbConn::getInstance()->getDb();
    QString sql = "SELECT video_name, video_path FROM normal_video_info WHERE DATE(create_time) = :date";
    if(channelid > 0){
        sql += " AND channel_id = :channel_id";
    }
    sql += " LIMIT :offset, :pagesize;";
    QSqlQuery query(db);
    query.prepare(sql);
    query.bindValue(":date", selected_date);
    query.bindValue(":offset", offset);
    query.bindValue(":pagesize", page_size);
    if(channelid > 0){
        query.bindValue(":channel_id", channelid);
    }


    if(!query.exec()){
        qDebug() << "初始化普通回放的sql执行失败" << query.lastError().text();
        return;
    }

    while(query.next()){
        NormalVideoInfo v;
        v.setVideo_name(query.value(0).toString());
        v.setVideo_path(query.value(1).toString());
        data.append(v);
//        QListWidgetItem* item_day = new QListWidgetItem(QIcon(":/images/device.png"),query.value(0).toString());
//        list_days->addItem(item_day);
    }



}

void VideoModel::fill_list_exception(QList<ExceptionVideoInfo> &dates, QString &selected_date, int &offset, int &page_size, int channelid)
{
    QSqlDatabase db = DbConn::getInstance()->getDb();
    QString sql = "SELECT video_name, related_video_path FROM exception_log WHERE DATE(event_time) = :date";
    if(channelid > 0){
        sql += " AND channel_id = :channel_id";
    }
    sql += " LIMIT :offset, :pagesize;";
    QSqlQuery query(db);
    query.prepare(sql);
    query.bindValue(":date", selected_date);
    query.bindValue(":offset", offset);
    query.bindValue(":pagesize", page_size);
    if(channelid > 0){
        query.bindValue(":channel_id", channelid);
    }


    if(!query.exec()){
        qDebug() << "初始化异常回放的sql执行失败" << query.lastError().text();
        return;
    }

    while(query.next()){
        ExceptionVideoInfo v;
        v.setVideo_name(query.value(0).toString());
        v.setRelated_video_path(query.value(1).toString());
        dates.append(v);
        //        QListWidgetItem* item_day = new QListWidgetItem(QIcon(":/images/device.png"),query.value(0).toString());
        //        list_days->addItem(item_day);
    }



}

void VideoModel::fill_date_com(QStringList &dates)
{
    QSqlDatabase db = DbConn::getInstance()->getDb();
    QSqlQuery query(db);
    QString sql_select_dates = "SELECT DISTINCT DATE(monitor_time) FROM monitor_record;";
    query.prepare(sql_select_dates);
    if(!query.exec()){
        qDebug() << "sql执行失败";
        return;
    }
    while(query.next()){
        dates.append(query.value(0).toString());
        //dates_com->addItem(query.value(0).toString());
    }
}

int VideoModel::query_total_normal(QString &selected_date, int channel_id)
{
    QSqlDatabase db = DbConn::getInstance()->getDb();
    QSqlQuery query(db);
    QString sql_count = "SELECT COUNT(*) FROM normal_video_info WHERE DATE(create_time) = :date";
    if(channel_id > 0){
        sql_count += " AND channel_id = :channel_id";
    }
    sql_count += ";";
    query.prepare(sql_count);
    query.bindValue(":date", selected_date);
    if(channel_id > 0){
        query.bindValue(":channel_id", channel_id);
    }
    if(!query.exec()){
        qDebug() << "sql执行失败" << query.lastError().text();
        return -1;
    }
    query.next();
    return query.value(0).toInt();
}

int VideoModel::query_total_exception(QString &selected_date, int channel_id)
{
    QSqlDatabase db = DbConn::getInstance()->getDb();
    QSqlQuery query(db);
    QString sql_count = "SELECT COUNT(*) FROM exception_log WHERE DATE(event_time) = :date";
    if(channel_id > 0){
        sql_count += " AND channel_id = :channel_id";
    }
    sql_count += ";";
    query.prepare(sql_count);
    query.bindValue(":date", selected_date);
    if(channel_id > 0){
        query.bindValue(":channel_id", channel_id);
    }
    if(!query.exec()){
        qDebug() << "sql执行失败" << query.lastError().text();
        return -1;
    }
    query.next();
    return query.value(0).toInt();
}
