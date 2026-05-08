#include "logmodel.h"

LogModel::LogModel() {}

void LogModel::insertNormalLogInfo(int admin_id, QString admin_name, QString operation_time, QString operation_func, QString operation_desc)
{
    QSqlDatabase db = DbConn::getInstance()->getDb();
    QSqlQuery query(db);
    QString sql = "INSERT INTO operation_log(admin_id, admin_name, operation_time, operation_func, operation_desc)"
                  " VALUES(:admin_id, :admin_name, :operation_time, :operation_func, :operation_desc);";
    query.prepare(sql);
    query.bindValue(":admin_id", admin_id == 0 ? QVariant() : admin_id);
    query.bindValue(":admin_name", admin_name);
    query.bindValue(":operation_time", operation_time);
    query.bindValue(":operation_func", operation_func);
    query.bindValue(":operation_desc", operation_desc);

    if(!query.exec()){
        qDebug() << "操作日志写入数据库失败" << query.lastError().text();
    }
}

std::variant<QList<NormalLogInfo>, QList<ExceptionLogInfo> > LogModel::queryLogInfo(bool isNormal, int offset, int page_size)
{
    QSqlDatabase db = DbConn::getInstance()->getDb();
    QSqlQuery query(db);
    QString sql = "";
    if(isNormal){ //查询操作日志
        QList<NormalLogInfo> normal_list;
        sql = "SELECT id, admin_name, operation_func, operation_time FROM operation_log "
              "ORDER BY operation_time DESC LIMIT :offset, :page_size;";
        query.prepare(sql);
        query.bindValue(":offset", offset);
        query.bindValue(":page_size", page_size);

        if(!query.exec()){
            qDebug() << "查询操作日志失败" << query.lastError().text();
        }

        while(query.next()){
            NormalLogInfo normal_info;
            normal_info.setLog_id(query.value(0).toInt());
            normal_info.setAdmin_name(query.value(1).toString());
            normal_info.setOperate_func(query.value(2).toString());
            normal_info.setOperate_time(query.value(3).toDateTime().toString("yyyy-MM-dd HH:mm:ss"));
            normal_list.append(normal_info);
        }

        return normal_list;
    }else{ //查询异常日志
        QList<ExceptionLogInfo> exception_list;
        sql = "SELECT id, event_desc, admin_name, related_video_path, event_time "
              "FROM exception_log ORDER BY event_time DESC LIMIT :offset, :page_size;";
        query.prepare(sql);
        query.bindValue(":offset", offset);
        query.bindValue(":page_size", page_size);

        if(!query.exec()){
            qDebug() << "查询异常日志失败" << query.lastError().text();
        }

        while(query.next()){
            ExceptionLogInfo exception_info;
            exception_info.setLog_id(query.value(0).toInt());
            exception_info.setException_desc(query.value(1).toString());
            exception_info.setAdmin_name(query.value(2).toString());
            exception_info.setVideo_path(query.value(3).toString());
            exception_info.setOperate_time(query.value(4).toDateTime().toString("yyyy-MM-dd HH:mm:ss"));
            exception_list.append(exception_info);
        }

        return exception_list;
    }
}

int LogModel::query_total_records(bool isNormal)
{
    QSqlDatabase db = DbConn::getInstance()->getDb();
    QSqlQuery query(db);
    QString sql = "SELECT COUNT(*) FROM ";
    sql += isNormal ? "operation_log;" : "exception_log;";
    query.prepare(sql);
    if(!query.exec()){
        qDebug() << "查询日志总记录数失败" << query.lastError().text();
    }
    query.next();
    return query.value(0).toInt();

}


