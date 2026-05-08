#ifndef DETECTTASK_H
#define DETECTTASK_H

#include <QObject>
#include "../yolo/Detect.h"


class DetectTask : public QObject
{
    Q_OBJECT
private:
    Detect detect;
    std::vector<YOLO_OUT> yoloOut;
    int framecount;
    bool hastarget = false; //是否检测到目标
public:
    explicit DetectTask(QObject *parent = nullptr);

signals:
    void sendDetectFrame(cv::Mat, int, bool);

public slots:
    void beginDectect(cv::Mat frame, int channel_id);
};

#endif // DETECTTASK_H
