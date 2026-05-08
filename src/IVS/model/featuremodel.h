#ifndef FEATUREMODEL_H
#define FEATUREMODEL_H

#include "../model/featureinfo.h"
#include "util/dbconn.h"
#include <QVariant>

class FeatureModel
{
public:
    FeatureModel();
    void insertFeatureInfo(FeatureInfo fi);
    int query_total_feature(QString &selected_date, int channel_id, int feature_type);
    void fill_list_feature(QList<FeatureInfo> &data, QString &selected_date, int &offset, int &page_size, int channel_id, int feature_type);
    QString query_video_path(QString feature_path);
};

#endif // FEATUREMODEL_H
