#include "featureservice.h"

FeatureService::FeatureService(QObject *parent)
    : QObject{parent}
{}

bool FeatureService::capture(QImage feature, int channel_id, QString& file_path, QString &file_name, bool hastarget, int index)
{
    if(!feature.isNull()){
        QString cur_time = QDateTime::currentDateTime().toString("yyyyMMddHHmmss");
        file_name = "";
        if(!hastarget){ //无目标，手动截图
            file_name = "手动_通道" + QString::number(channel_id) + cur_time + ".jpg";
            file_path = "../records/normal_records/features/" + file_name;
            if(feature.save(file_path, "JPG", 100)){
                qDebug() << "手动截图成功保存至文件中";
                return true;
            }
        }else{ //异常截图
            file_name = "异常_通道" + QString::number(channel_id) + cur_time + "_" + QString::number(index) +".jpg";
            file_path = "../records/exception_records/features/" + file_name;
            if(feature.save(file_path, "JPG", 100)){
                qDebug() << "异常截图成功保存至文件中";
                return true;
            }

        }



    }

    return false;
}

QString FeatureService::query_related_video_path(QString feature_path)
{
    FeatureModel fm;
    return fm.query_video_path(feature_path);
}
