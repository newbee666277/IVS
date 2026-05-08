#ifndef SETTINGINFO_H
#define SETTINGINFO_H

#include <QString>
#include "camerainfo.h"
#include <QList>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QCameraDevice>
#include <QMediaDevices>

class SettingInfo
{
public:
    SettingInfo();
    static QString filepath;
    static QList<CameraInfo> cameras;
    static int timeInterval;
    static void saveToFile();
    static void loadFromFile();
    static void init_camera();

};

#endif // SETTINGINFO_H
