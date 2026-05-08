#ifndef RECORDTASK_H
#define RECORDTASK_H

#include <QObject>
#include <QDebug>
#include <opencv2/opencv.hpp>
#include <QThread>
#include <QDateTime>
#include <QString>
#include <QMap>
#include "../model/normalvideoinfo.h"
#include "../model/exceptionvideoinfo.h"
#include "../model/videoinfomodel.h"
#include "../service/featureservice.h"
#include "../model/featureinfo.h"
#include "../model/featuremodel.h"

using namespace cv;

enum RecordMode{
    IDLE, //空闲状态
    NORMAL, //普通录制
    EXCEPTION //移动检测
};

class RecordTask : public QObject
{
    Q_OBJECT
private:
    VideoWriter writer;
    int fps;
    int framecount;
    int lose_time; //目标消失的时间
    int current_channel_id;
    int feature_counts; //截图数量
    QDateTime current_time;
    QString fileName;
    QMap<QString, QString> map_feature_info; //存储当次录制的三个异常图片的路径以及名字
    RecordMode cur_mode; //记录当前类的状态
    QString current_path; //记录当前路径
    bool isDetectMode; //记录当前全局的状态
public:
    explicit RecordTask(int fps, QObject *parent = nullptr);
    void stopRecord();
    QImage MatToQImage(Mat& frame);
public slots:
    void startRecord(Mat frame, int channel_id, bool hastarget);
signals:
};

#endif // RECORDTASK_H
