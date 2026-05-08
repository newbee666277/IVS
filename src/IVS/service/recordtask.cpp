#include "recordtask.h"
#include "../view/monitorwidget.h"
#include "../model/settinginfo.h"
#include "../view/mainwidget.h"

RecordTask::RecordTask(int fps, QObject *parent)
    : QObject{parent}
{
    this->fps = fps;
    this->framecount = 0;
    this->cur_mode = IDLE;
    this->lose_time = 0;
    this->feature_counts = 0;
}

void RecordTask::stopRecord()
{
    if(writer.isOpened()){ //说明还在写入，强制停止
        writer.release();
        qDebug() << "停止记录";

        int exception_id = 0;
        VideoInfoModel vm;
        //当前为普通录制
        if(cur_mode == NORMAL){
            NormalVideoInfo v;
            v.setVideo_name(fileName);
            v.setVideo_path(current_path);
            v.setChannel_id(current_channel_id+1);
            v.setVideo_duration(framecount/30);
            v.setCreate_time(current_time.toString("yyyy-MM-dd hh:mm:ss"));

            exception_id = vm.insertNormalVideoInfo(v);
        }else if(cur_mode == EXCEPTION){
            ExceptionVideoInfo v;
            v.setAdmin_name(MainWidget::admin_name);
            v.setVideo_name(fileName);
            v.setRelated_video_path(current_path);
            v.setChannel_id(current_channel_id+1);
            v.setVideo_duration(framecount/30);
            v.setEvent_time(current_time.toString("yyyy-MM-dd hh:mm:ss"));

            exception_id = vm.insertExceptionlVideoInfo(v);

            FeatureInfo f;
            FeatureModel fm;

            for(auto it = map_feature_info.begin(); it != map_feature_info.end(); it++){
                f.setName(it.value());
                f.setPath(it.key());
                f.setChannel_id(current_channel_id + 1);
                f.setException_id(exception_id);
                f.setFeature_type(1);
                fm.insertFeatureInfo(f);
            }
        }

        if(exception_id > 0){
            qDebug() << "视频写入数据库成功";
        }else{
            qDebug() << "视频写入数据库失败";
        }

        this->framecount = 0;
        this->cur_mode = IDLE;
        this->lose_time = 0;
        this->feature_counts = 0;
        this->map_feature_info.clear();
    }
}

void RecordTask::startRecord(Mat frame, int channel_id, bool hastarget)
{
    this->current_channel_id = channel_id;

    isDetectMode = MonitorWidget::isDetect; //记录当前全局状态，是否勾选目标检测

    //判断当前类状态与全局状态是否一致
    if(isDetectMode && cur_mode == NORMAL){
        stopRecord(); //停止普通检测
    }else if(!isDetectMode && cur_mode == EXCEPTION){
        stopRecord(); //停止目标检测
    }

    if(!isDetectMode){ //未开启目标检测，普通录制
        if(!writer.isOpened()){ //文件写入器没有被创建
            current_time = QDateTime::currentDateTime();
            fileName = "Record_" + current_time.toString("yyyyMMdd_hhmmss") + "_" +
                       QString::number(channel_id+1) + ".mp4";
            Size frameSize(frame.cols, frame.rows);
            int fourcc = VideoWriter::fourcc('H','2','6','4');
            //保存到normal_records文件夹
            current_path = "records/normal_records/videos/" + fileName;
            writer = VideoWriter("../" + current_path.toStdString(), fourcc, 30, frameSize, true);
            cur_mode = NORMAL;
        }

        writer.write(frame);
        this->framecount++;

        if(this->framecount > 30 * SettingInfo::timeInterval){
            stopRecord();
        }
    }else{
        if(hastarget){
            if(!writer.isOpened()){ //文件写入器没有被创建
                current_time = QDateTime::currentDateTime();
                fileName = "Record_" + current_time.toString("yyyyMMdd_hhmmss") + "_" +
                           QString::number(channel_id+1) + ".mp4";
                Size frameSize(frame.cols, frame.rows);
                int fourcc = VideoWriter::fourcc('H','2','6','4');
                //保存到exception_records文件夹
                current_path = "records/exception_records/videos/" + fileName;
                writer = VideoWriter("../" + current_path.toStdString(), fourcc, 30, frameSize, true);
                cur_mode = EXCEPTION;
            }

            writer.write(frame);
            this->framecount++;

            //同时自动截图照片并保存
            if(this->feature_counts < 3){
                if(this->framecount % 15 == 1){
                    FeatureService fs;
                    QString feature_path = "";
                    QString feature_name = "";
                    fs.capture(MatToQImage(frame), channel_id, feature_path, feature_name, hastarget, this->feature_counts + 1);
                    this->feature_counts++;
                    map_feature_info[feature_path] = feature_name;
                }
            }


            if(this->framecount > 30 * SettingInfo::timeInterval){
                stopRecord();

            }
        }else{
            if(writer.isOpened()){ //写入器开着，说明还在录，但是目标消失
                writer.write(frame);
                this->lose_time++;

                if(lose_time > 30*3){ //目标消失超过3秒停止
                    stopRecord();
                }
            }
        }
    }

}

QImage RecordTask::MatToQImage(Mat& frame)
{
    if(frame.empty()){
        return QImage();
    }

    switch(frame.type()){
    case CV_8UC1:{
        return QImage(frame.data, frame.cols, frame.rows, frame.step, QImage::Format_Grayscale8).copy();
    }
    case CV_8UC3:{
        cv::Mat rgb_frame; // 创建一个临时的 Mat
        // 把转换后的结果存到 rgb_frame里
        cv::cvtColor(frame, rgb_frame, cv::COLOR_BGR2RGB);
        return QImage(rgb_frame.data, rgb_frame.cols, rgb_frame.rows, rgb_frame.step, QImage::Format_RGB888).copy();
    }
    default:{
        return QImage();
    }
    }
}
