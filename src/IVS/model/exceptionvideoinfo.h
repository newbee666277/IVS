#ifndef EXCEPTIONVIDEOINFO_H
#define EXCEPTIONVIDEOINFO_H

#include <QString>
#include <QDateTime>

class ExceptionVideoInfo
{
private:
    QString admin_name;
    int channel_id;
    QString event_time;
    QString event_desc = "检测到目标";
    QString related_video_path;
    QString video_name;
    int video_duration;
public:
    ExceptionVideoInfo();
    QString getAdmin_name() const;
    void setAdmin_name(const QString &newAdmin_name);
    int getChannel_id() const;
    void setChannel_id(int newChannel_id);
    QString getEvent_time() const;
    void setEvent_time(const QString &newEvent_time);
    QString getEvent_desc() const;
    void setEvent_desc(const QString &newEvent_desc);
    QString getRelated_video_path() const;
    void setRelated_video_path(const QString &newRelated_video_path);
    QString getVideo_name() const;
    void setVideo_name(const QString &newVideo_name);
    int getVideo_duration() const;
    void setVideo_duration(int newVideo_duration);
};

#endif // EXCEPTIONVIDEOINFO_H
