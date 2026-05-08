#include "videoplaytask.h"


int VideoPlayTask::getFramecount() const
{
    return framecount;
}

int VideoPlayTask::getCur_frame() const
{
    return cur_frame;
}

double VideoPlayTask::getSpeed() const
{
    return speed;
}

void VideoPlayTask::setSpeed(double newSpeed)
{
    speed = newSpeed;
}

volatile bool VideoPlayTask::getIsPause() const
{
    return isPause;
}

void VideoPlayTask::setIsPause(volatile bool newIsPause)
{
    isPause = newIsPause;
}

VideoPlayTask::VideoPlayTask(QString path, QObject *parent)
    : QObject{parent}
{
    this->cur_frame = 0;
    this->speed = 1;

    QString relative_path = "../" + path;
    video.open(relative_path.toStdString());

    if(!video.isOpened()){
        qDebug() << "视频回放功能视频打开失败";
        return;
    }

    this->height = video.get(CAP_PROP_FRAME_HEIGHT);
    this->width = video.get(CAP_PROP_FRAME_WIDTH);
    this->fps = video.get(CAP_PROP_FPS);
    this->framecount = video.get(CAP_PROP_FRAME_COUNT);

}

void VideoPlayTask::stop()
{
    this->isStop = true;
}

void VideoPlayTask::start()
{
    this->isStop = false;
}

void VideoPlayTask::pause()
{
    this->isPause = true;
}

void VideoPlayTask::go()
{
    this->isPause = false;
}

void VideoPlayTask::setPlayFrame(int frameIndex)
{
    if(frameIndex >= 0 && frameIndex < framecount){
        target_frame = frameIndex;
    }
}

void VideoPlayTask::startPlay()
{
    Mat frame;
    while(!isStop){
        if(isPause){
            QThread::msleep(30/speed);
            continue;
        }

        if(target_frame >= 0){
            cur_frame = target_frame;
            video.set(CAP_PROP_POS_FRAMES, cur_frame);
            target_frame = -1;
        }

        if(video.read(frame)){
            cur_frame++;
            emit sendFrame(frame, cur_frame);
        }else{
            //结束后从头开始播放
            video.set(CAP_PROP_POS_FRAMES, 0);
            cur_frame = 0;
        }
        QThread::msleep(30/this->speed);
    }
}
