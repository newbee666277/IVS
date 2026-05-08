#include "monitortask.h"

int MonitorTask::getFps() const
{
    return fps;
}

MonitorTask::MonitorTask(QString path, int channel_id, QObject *parent)
    : QObject{parent}
{
    this->filepath = path;
    this->channel_id = channel_id;
    this->isStop = false;

    if(path != nullptr){ //传入视频文件
        video.open(path.toStdString());
    }else{
        video.open(0); //打开摄像头
    }

    if(!video.isOpened()){
        qDebug() << "视频打开失败";
        return;
    }

    this->height = video.get(CAP_PROP_FRAME_HEIGHT);
    this->width = video.get(CAP_PROP_FRAME_WIDTH);
    this->fps = video.get(CAP_PROP_FPS);
    this->framecount = video.get(CAP_PROP_FRAME_COUNT);

}

void MonitorTask::stop()
{
    this->isStop = true;
}

void MonitorTask::startMonitor()
{
    Mat frame;
    while(!isStop){
        if(video.read(frame)){
            //Mat cloneFrame = frame.clone();
            emit sendFrame(frame, this->channel_id);
        }else{
            video.set(CAP_PROP_POS_FRAMES, 0);
        }

        QThread::msleep(30);
    }

    video.release();
}

