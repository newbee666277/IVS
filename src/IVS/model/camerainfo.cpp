#include "camerainfo.h"

int CameraInfo::getChannel_id() const
{
    return channel_id;
}

void CameraInfo::setChannel_id(int newChannel_id)
{
    channel_id = newChannel_id;
}

QString CameraInfo::getChannel_name() const
{
    return channel_name;
}

void CameraInfo::setChannel_name(const QString &newChannel_name)
{
    channel_name = newChannel_name;
}

QString CameraInfo::getCamera_name() const
{
    return camera_name;
}

void CameraInfo::setCamera_name(const QString &newCamera_name)
{
    camera_name = newCamera_name;
}

CameraInfo::CameraInfo() {}
