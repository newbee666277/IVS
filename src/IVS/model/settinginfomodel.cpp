#include "settinginfomodel.h"


SettingInfoModel::SettingInfoModel() {}

void SettingInfoModel::saveSettingInfo()
{
    //QSqlDatabase db = DbConnectionPool::getInstance()->getConnection();
    QSqlDatabase db = DbConn::getInstance()->getDb();

    if(!db.isValid() || !db.isOpen()){
        qDebug() << "数据库连接失败！";
        return;
    }

    QSqlQuery query(db);

    QString sql_config = "INSERT INTO sys_config(save_path, interval_time, "
                  "channel_name_1, channel_name_2, channel_name_3, channel_name_4) "
                  "VALUES (:save_path, :interval_time, :channel_name_1, "
                  ":channel_name_2, :channel_name_3, :channel_name_4);";
    query.prepare(sql_config);
    query.bindValue(":save_path", SettingInfo::filepath);
    query.bindValue(":interval_time", SettingInfo::timeInterval);
    //channel_id一定对应channel_name 不能瞎对应
    for(int i = 0; i < SettingInfo::cameras.size(); i++){
        int cur_channel_id = SettingInfo::cameras[i].getChannel_id();
        switch(cur_channel_id){
            case 1:{
                query.bindValue(":channel_name_1", SettingInfo::cameras[i].getChannel_name());
                break;
            }
            case 2:{
                query.bindValue(":channel_name_2", SettingInfo::cameras[i].getChannel_name());
                break;
            }
            case 3:{
                query.bindValue(":channel_name_3", SettingInfo::cameras[i].getChannel_name());
                break;
            }
            case 4:{
                query.bindValue(":channel_name_4", SettingInfo::cameras[i].getChannel_name());
                break;
            }
        }
    }

    if(!query.exec()){
        qDebug() << "系统设置数据插入数据失败" << query.lastError().text();
        return;
    }

    //同时更新camera_channel表的信息
    for(int i = 0; i < SettingInfo::cameras.size(); i++){
        QSqlQuery query_channel(db);
        //使用 ON DUPLICATE KEY UPDATE 进行平滑更新，绝不触发删除
        QString sql_camera_channel_info = "INSERT INTO camera_channel(id, channel_name, camera_name, is_online) "
                                          "VALUES(:id, :channel_name, :camera_name, :is_online) "
                                          "ON DUPLICATE KEY UPDATE "
                                          "channel_name = VALUES(channel_name), "
                                          "camera_name = VALUES(camera_name), "
                                          "is_online = VALUES(is_online);";
        query_channel.prepare(sql_camera_channel_info);
        query_channel.bindValue(":id", SettingInfo::cameras[i].getChannel_id());
        query_channel.bindValue(":channel_name", SettingInfo::cameras[i].getChannel_name());
        query_channel.bindValue(":camera_name", SettingInfo::cameras[i].getCamera_name());
        query_channel.bindValue(":is_online", SettingInfo::cameras[i].getCamera_name() == "virutal_camera" ? 0 : 1);

        if(!query_channel.exec()){
            qDebug() << "camera_channel表通道" << SettingInfo::cameras[i].getChannel_id() << "更新失败" << "错误信息: " << query_channel.lastError().text();
            return;
        }
    }

}
