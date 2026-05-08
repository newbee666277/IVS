#ifndef VIDEOMODEL_H
#define VIDEOMODEL_H

#include <QListWidget>
#include <QLabel>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QString>
#include <QDebug>
#include <QComboBox>
#include <QStringList>
#include <QList>
#include "../util/dbconn.h"
#include "../model/normalvideoinfo.h"
#include "../model/exceptionvideoinfo.h"

class VideoModel
{
public:
    VideoModel();
    //填充日期列表具体内容
    void fill_list_normal(QList<NormalVideoInfo> &data, QString &selected_date, int &offset, int &page_size, int channelid);

    void fill_list_exception(QList<ExceptionVideoInfo> &data, QString &selected_date, int &offset, int &page_size,  int channelid);
    //填充日期选择下拉框(旧)
    void fill_date_com(QStringList &dates);
    //计算数据库中的总记录数
    int query_total_normal(QString &selected_date, int channel_id);
    int query_total_exception(QString &selected_date, int channel_id);
};

#endif // VIDEOMODEL_H
