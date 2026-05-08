#include "view/mainwidget.h"
#include "util/windowmanager.h"
#include "model/settinginfo.h"
#include "yolo/Detect.h"
#include <QApplication>
#include <iostream>

void test01(){
    std::string imgpath = "data/images/game.png";
    std::string modelpath = "data/model/yolov8n.onnx";
    cv::Mat image = cv::imread(imgpath);
    Detect detect(modelpath);
    //加载模型文件
    detect.loadModel();
    if (image.empty())
    {
        std::cerr << "No image loaded to run the test." << std::endl;
        return;
    }
    //获取模型尺寸
    detect.model_input_size = cv::Size(640, 640);
    //定义输出检测数据容器
    std::vector<YOLO_OUT> yoloOut;
    //获取图像原始大小
    detect.org_Size = image.size();
    //图像预处理，将原始图像转换成640*640
    cv::Mat out_frame = detect.letterbox(image, detect.model_input_size.width, detect.model_input_size.height, cv::Scalar(114, 114, 114));
    //模型推理，获取检测到的目标数据
    detect.detect(out_frame, yoloOut);
    //绘制图像，标记检测到的图像
    detect.draw(image, yoloOut);
    cv::imshow("Detected Image", image);
    cv::waitKey(0);
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //加载配置文件
    SettingInfo::loadFromFile();

    WindowManager::getInstance()->settingwidget.show();

    return a.exec();
}
