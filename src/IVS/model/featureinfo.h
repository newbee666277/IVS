#ifndef FEATUREINFO_H
#define FEATUREINFO_H

#include <QString>

class FeatureInfo
{
private:
    QString name;
    QString path;
    int channel_id;
    int feature_type;
    QString capture_time;
    int exception_id;
public:
    FeatureInfo();
    QString getName() const;
    void setName(const QString &newName);
    QString getPath() const;
    void setPath(const QString &newPath);
    int getChannel_id() const;
    void setChannel_id(int newChannel_id);
    int getFeature_type() const;
    void setFeature_type(int newFeature_type);
    QString getCapture_time() const;
    void setCapture_time(const QString &newCapture_time);
    int getException_id() const;
    void setException_id(int newException_id);
};

#endif // FEATUREINFO_H
