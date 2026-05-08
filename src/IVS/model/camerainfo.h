#ifndef CAMERAINFO_H
#define CAMERAINFO_H
#include <QString>

class CameraInfo
{
private:
    int channel_id;
    QString channel_name;  //通道名，用户自己输入的作为摄像头的别名
    QString camera_name; //摄像头自身的名字，默认为vitual camera
public:
    CameraInfo();
    int getChannel_id() const;
    void setChannel_id(int newChannel_id);
    QString getChannel_name() const;
    void setChannel_name(const QString &newChannel_name);
    QString getCamera_name() const;
    void setCamera_name(const QString &newCamera_name);
};

#endif // CAMERAINFO_H
