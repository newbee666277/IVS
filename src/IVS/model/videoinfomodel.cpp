#include "videoinfomodel.h"

VideoInfoModel::VideoInfoModel() {}

int VideoInfoModel::insertExceptionlVideoInfo(ExceptionVideoInfo &video_info)
{
    QSqlDatabase db = DbConn::getInstance()->getDb();
    QSqlQuery query(db);
    QString sql = "INSERT INTO exception_log(admin_name, video_name, related_video_path, channel_id, event_time, video_duration, event_desc) "
                  "VALUES(:admin_name, :video_name, :related_video_path, :channel_id, :event_time, :video_duration, :event_desc);";
    query.prepare(sql);
    query.bindValue(":admin_name", video_info.getAdmin_name());
    query.bindValue(":video_name", video_info.getVideo_name());
    query.bindValue(":related_video_path", video_info.getRelated_video_path());
    query.bindValue(":channel_id", video_info.getChannel_id());
    query.bindValue(":event_time", video_info.getEvent_time());
    query.bindValue(":video_duration", video_info.getVideo_duration());
    query.bindValue(":event_desc", video_info.getEvent_desc());
    if(query.exec()){
        qDebug() << "插入成功";
        return query.lastInsertId().toInt();
    }else{
        qDebug() << "异常视频插入数据库错误" << query.lastError().text();
        return -1;
    }

    return -1;
}

int VideoInfoModel::insertNormalVideoInfo(NormalVideoInfo &video_info)
{
    QSqlDatabase db = DbConn::getInstance()->getDb();
    QSqlQuery query(db);
    QString sql = "INSERT INTO normal_video_info(video_name, video_path, channel_id, create_time, video_duration) "
                  "VALUES(:video_name, :video_path, :channel_id, :create_time, :video_duration);";
    query.prepare(sql);
    query.bindValue(":video_name", video_info.getVideo_name());
    query.bindValue(":video_path", video_info.getVideo_path());
    query.bindValue(":channel_id", video_info.getChannel_id());
    query.bindValue(":create_time", video_info.getCreate_time());
    query.bindValue(":video_duration", video_info.getVideo_duration());
    if(query.exec()){
        qDebug() << "插入成功";
        return query.lastInsertId().toInt();
    }else{
        qDebug() << "普通视频插入数据库错误" << query.lastError().text();
        return -1;
    }

    return -1;
}
