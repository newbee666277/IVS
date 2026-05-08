#ifndef FEATURESERVICE_H
#define FEATURESERVICE_H

#include <QObject>
#include <QDebug>
#include "../model/featuremodel.h"
#include <opencv2/opencv.hpp>
#include <QImage>
#include <QMessageBox>
#include <QDateTime>
#include <QString>
#include "../model/featuremodel.h"

class FeatureService : public QObject
{
    Q_OBJECT
public:
    explicit FeatureService(QObject *parent = nullptr);
    bool capture(QImage feature, int channel_id, QString& file_path, QString &file_name, bool hastarget, int index = 0);
    QString query_related_video_path(QString feature_path);
signals:
};

#endif // FEATURESERVICE_H
