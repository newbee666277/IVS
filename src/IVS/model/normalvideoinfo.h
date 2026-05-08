#ifndef NORMALVIDEOINFO_H
#define NORMALVIDEOINFO_H

#include <QString>


class NormalVideoInfo
{
private:
    QString video_path;
    QString video_name;
    int channel_id;
    QString create_time;
    int video_type;
    int video_duration;

public:
    NormalVideoInfo();
    QString getVideo_name() const;
    void setVideo_name(const QString &newVideo_name);
    int getChannel_id() const;
    void setChannel_id(int newChannel_id);
    QString getCreate_time() const;
    void setCreate_time(const QString &newCreate_time);
    int getVideo_type() const;
    QString getVideo_path() const;
    void setVideo_path(const QString &newVideo_path);
    int getVideo_duration() const;
    void setVideo_duration(int newVideo_duration);
};

#endif // NORMALVIDEOINFO_H
