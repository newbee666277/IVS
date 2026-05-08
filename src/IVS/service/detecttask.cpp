#include "detecttask.h"
#include "../view/monitorwidget.h"

DetectTask::DetectTask(QObject *parent)
    : QObject{parent}, detect("data/model/yolov8n.onnx")
{
    //加载模型文件
    detect.loadModel();
    //获取模型尺寸
    detect.model_input_size = cv::Size(320, 320);
}

void DetectTask::beginDectect(cv::Mat frame, int channel_id)
{
    if(!MonitorWidget::isDetect){
        emit sendDetectFrame(frame, channel_id, this->hastarget);
        return;
    }
    //获取图像原始大小
    detect.org_Size = frame.size();
    //图像预处理，将原始图像转换成640*640
    cv::Mat out_frame = detect.letterbox(frame, detect.model_input_size.width, detect.model_input_size.height, cv::Scalar(114, 114, 114));
    //模型推理(每10帧推理一次，避免卡顿)，获取检测到的目标数据
    if(this->framecount % 10 == 0){
        yoloOut.clear();
        detect.detect(out_frame, yoloOut);
    }

    //绘制图像，标记检测到的图像
    detect.draw(frame, yoloOut);
    this->framecount++;

    this->hastarget = !yoloOut.empty(); //判断数组是否为空，判断是否有目标

    emit sendDetectFrame(frame, channel_id, this->hastarget);
}
