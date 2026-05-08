#include "featuremodel.h"

FeatureModel::FeatureModel() {}

void FeatureModel::insertFeatureInfo(FeatureInfo fi)
{
    QSqlDatabase db = DbConn::getInstance()->getDb();

    QSqlQuery query(db);

    QString sql = "INSERT INTO feature_image(image_name, image_path, "
                  "channel_id, feature_type, exception_id) VALUES(:image_name,"
                  ":image_path, :channel_id, :feature_type, :exception_id)";

    query.prepare(sql);
    query.bindValue(":image_name", fi.getName());
    query.bindValue(":image_path", fi.getPath());
    query.bindValue(":channel_id", fi.getChannel_id());
    query.bindValue(":feature_type", fi.getFeature_type());
    //query.bindValue(":capture_time", fi.getCapture_time());
    query.bindValue(":exception_id", fi.getException_id() == 0 ? QVariant() : fi.getException_id());

    if(!query.exec()){
        qDebug() << "图片插入数据库失败" << query.lastError().text();
    }
}

int FeatureModel::query_total_feature(QString &selected_date, int channel_id, int feature_type)
{
    QSqlDatabase db = DbConn::getInstance()->getDb();
    QSqlQuery query(db);
    QString sql_count = "SELECT COUNT(*) FROM feature_image WHERE DATE(capture_time) = :date AND feature_type = :feature_type";
    if(channel_id > 0){
        sql_count += " AND channel_id = :channel_id";
    }
    sql_count += ";";
    query.prepare(sql_count);
    query.bindValue(":date", selected_date);
    query.bindValue(":feature_type", feature_type);
    if(channel_id > 0){
        query.bindValue(":channel_id", channel_id);
    }
    if(!query.exec()){
        qDebug() << "图片数量查询执行失败" << query.lastError().text();
        return -1;
    }
    query.next();
    return query.value(0).toInt();
}

void FeatureModel::fill_list_feature(QList<FeatureInfo> &data, QString &selected_date, int &offset, int &page_size, int channel_id, int feature_type)
{
    QSqlDatabase db = DbConn::getInstance()->getDb();
    QString sql = "SELECT image_name, image_path FROM feature_image WHERE DATE(capture_time) = :date AND feature_type = :feature_type";
    if(channel_id > 0){
        sql += " AND channel_id = :channel_id";
    }
    sql += " LIMIT :offset, :pagesize;";
    QSqlQuery query(db);
    query.prepare(sql);
    query.bindValue(":date", selected_date);
    query.bindValue(":feature_type", feature_type);
    query.bindValue(":offset", offset);
    query.bindValue(":pagesize", page_size);
    if(channel_id > 0){
        query.bindValue(":channel_id", channel_id);
    }

    if(!query.exec()){
        qDebug() << "图片列表查询执行失败" << query.lastError().text();
        return;
    }

    while(query.next()){
        FeatureInfo f;
        f.setName(query.value(0).toString());
        f.setPath(query.value(1).toString());
        data.append(f);
    }
}

QString FeatureModel::query_video_path(QString feature_path)
{
    QSqlDatabase db = DbConn::getInstance()->getDb();
    QSqlQuery query(db);

    // 1. 先查询出该图片的详细属性（类型、通道号、截图时间、异常ID）
    QString sql_info = "SELECT feature_type, exception_id, channel_id, capture_time "
                       "FROM feature_image WHERE image_path = :path;";
    query.prepare(sql_info);
    query.bindValue(":path", feature_path);
    if(!query.exec() || !query.next()){
        qDebug() << "获取图片信息失败" << query.lastError().text();
        return "";
    }

    int feature_type = query.value(0).toInt();
    int exception_id = query.value(1).toInt();
    int channel_id = query.value(2).toInt();
    QString capture_time = query.value(3).toString();

    // 2. 根据类型去不同的表匹配视频
    if (feature_type == 1) {
        // ==== 异常截图 ====
        QSqlQuery query_exc(db);
        query_exc.prepare("SELECT related_video_path FROM exception_log WHERE id = :id;");
        query_exc.bindValue(":id", exception_id);
        if(query_exc.exec() && query_exc.next()){
            return query_exc.value(0).toString();
        }
    } else {
        // ==== 手动截图 ====
        QSqlQuery query_nor(db);
        // 核心逻辑：找同通道下，录制开始时间 <= 截图时间，且离截图时间最近的那个普通视频
        QString sql_nor = "SELECT video_path FROM normal_video_info "
                          "WHERE channel_id = :channel_id AND create_time <= :capture_time "
                          "ORDER BY create_time DESC LIMIT 1;";
        query_nor.prepare(sql_nor);
        query_nor.bindValue(":channel_id", channel_id);
        query_nor.bindValue(":capture_time", capture_time);

        if(query_nor.exec() && query_nor.next()){
            return query_nor.value(0).toString();
        }
    }

    return ""; // 如果没找到则返回空字符串
}