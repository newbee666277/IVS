#ifndef MONITORTASK_H
#define MONITORTASK_H

#include <QObject>
#include <QString>
#include <QDebug>
#include <QThread>
#include <opencv2/opencv.hpp>
using namespace cv;
class MonitorTask : public QObject
{
    Q_OBJECT
private:
    int height;
    int width;
    int fps;
    int framecount;
    VideoCapture video;
    QString filepath;
    int channel_id;
    volatile bool isStop;

public:
    explicit MonitorTask(QString path = nullptr, int channel_id = 0, QObject *parent = nullptr);
    void stop();
    int getFps() const;

public slots:
    void startMonitor();

signals:
    void sendFrame(Mat frame, int channel_id);
};

#endif // MONITORTASK_H
