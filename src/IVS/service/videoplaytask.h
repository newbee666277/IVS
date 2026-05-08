#ifndef VIDEOPLAYTASK_H
#define VIDEOPLAYTASK_H

#include <QObject>
#include <opencv2/opencv.hpp>
#include <QDebug>
#include <QThread>
using namespace cv;

class VideoPlayTask : public QObject
{
    Q_OBJECT
private:
    cv::VideoCapture video;
    int height;
    int width;
    int fps;
    int framecount;
    int cur_frame;
    volatile bool isStop = false;
    volatile bool isPause = false; //判断视频是否暂停
    double speed;
    int target_frame = -1; //防止UI线程与视频播放线程冲突
public:
    explicit VideoPlayTask(QString path, QObject *parent = nullptr);
    void stop(); //彻底停掉视频
    void start();
    void pause(); //暂停播放
    void go();//继续播放
    void setPlayFrame(int frameIndex);

    int getFramecount() const;

    int getCur_frame() const;

    double getSpeed() const;
    void setSpeed(double newSpeed);

    volatile bool getIsPause() const;
    void setIsPause(volatile bool newIsPause);



signals:
    void sendFrame(Mat, int);
public slots:
    void startPlay();
};

#endif // VIDEOPLAYTASK_H
