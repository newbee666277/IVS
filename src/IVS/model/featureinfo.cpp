#include "featureinfo.h"

QString FeatureInfo::getName() const
{
    return name;
}

void FeatureInfo::setName(const QString &newName)
{
    name = newName;
}

QString FeatureInfo::getPath() const
{
    return path;
}

void FeatureInfo::setPath(const QString &newPath)
{
    path = newPath;
}

int FeatureInfo::getChannel_id() const
{
    return channel_id;
}

void FeatureInfo::setChannel_id(int newChannel_id)
{
    channel_id = newChannel_id;
}

int FeatureInfo::getFeature_type() const
{
    return feature_type;
}

void FeatureInfo::setFeature_type(int newFeature_type)
{
    feature_type = newFeature_type;
}

QString FeatureInfo::getCapture_time() const
{
    return capture_time;
}

void FeatureInfo::setCapture_time(const QString &newCapture_time)
{
    capture_time = newCapture_time;
}

int FeatureInfo::getException_id() const
{
    return exception_id;
}

void FeatureInfo::setException_id(int newException_id)
{
    exception_id = newException_id;
}

FeatureInfo::FeatureInfo() {}
