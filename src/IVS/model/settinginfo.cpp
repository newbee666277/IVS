#include "settinginfo.h"
QString SettingInfo::filepath = QString();
QList<CameraInfo> SettingInfo::cameras = QList<CameraInfo>();
int SettingInfo::timeInterval = 0;

SettingInfo::SettingInfo() {}

void SettingInfo::saveToFile()
{
    QString configpath = "./config.json";
    QFile file(configpath);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text)){
        qDebug() << "文件打开失败";
        return;
    }

    QJsonObject rootObj;
    rootObj["filepath"] = SettingInfo::filepath;
    rootObj["timeInterval"] = SettingInfo::timeInterval;

    QJsonArray cameraArray;
    for(auto &cam : SettingInfo::cameras){
        QJsonObject cameraObj;
        cameraObj["camera_name"] = cam.getCamera_name();
        cameraObj["channel_name"] = cam.getChannel_name();
        cameraObj["channel_id"] = cam.getChannel_id();
        cameraArray.append(cameraObj);
    }

    rootObj["cameras"] = cameraArray;

    QJsonDocument doc(rootObj);
    file.write(doc.toJson(QJsonDocument::Indented)); //格式化输出
    file.close();
}

void SettingInfo::loadFromFile()
{
    QString configpath = "./config.json";
    QFile file(configpath);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        SettingInfo::init_camera();
        return; //读取失败直接使用默认值
    }

    QByteArray jsonData = file.readAll();
    QJsonParseError jsonError;
    QJsonDocument doc = QJsonDocument::fromJson(jsonData, &jsonError);

    if(jsonError.error != QJsonParseError::NoError || !doc.isObject()){
        qDebug() << "JSON格式解析失败";
        file.close();

        SettingInfo::init_camera();
        return;
    }

    //解析根对象
    QJsonObject rootObj = doc.object();

    //配置基础信息
    SettingInfo::filepath = rootObj["filepath"].toString();
    SettingInfo::timeInterval = rootObj["timeInterval"].toInt();
    SettingInfo::cameras.clear();

    QJsonArray cameraArray = rootObj["cameras"].toArray();
    for(int i = 0; i < cameraArray.size(); i++){
        QJsonObject cameraObj = cameraArray[i].toObject();
        CameraInfo cam;
        cam.setCamera_name(cameraObj["camera_name"].toString());
        cam.setChannel_name(cameraObj["channel_name"].toString());
        cam.setChannel_id(cameraObj["channel_id"].toInt());
        SettingInfo::cameras.append(cam);
    }

    file.close();
}

void SettingInfo::init_camera()
{
    //获取所有的摄像头并存入列表
    QList<QCameraDevice> camera_list = QMediaDevices::videoInputs();

    if(camera_list.isEmpty()){
        qDebug() << "未检测到任何摄像头设备";
    }

    int i = 0;
    for(i = 0; i < camera_list.size(); i++){
        QCameraDevice camera = camera_list.at(i);
        QString camera_name = camera.description();
        QString camera_id = camera.id();
        bool isDafult = camera.isDefault(); //判断是否为系统默认摄像头

        if(camera_name.contains("IR")){
            continue; //带IR的为红外摄像头，不适用
        }
        CameraInfo cam;
        cam.setCamera_name(camera_name);
        cam.setChannel_name(camera_name);
        cam.setChannel_id(i + 1);

        SettingInfo::cameras.append(cam);
    }

    for(; i < 4; i++){
        CameraInfo cam;
        cam.setCamera_name("virutal_camera");
        cam.setChannel_name("virutal_camera");
        cam.setChannel_id(i + 1);

        SettingInfo::cameras.append(cam);
    }
}
