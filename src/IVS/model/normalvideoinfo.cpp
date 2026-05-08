#include "normalvideoinfo.h"


QString NormalVideoInfo::getVideo_name() const
{
    return video_name;
}

void NormalVideoInfo::setVideo_name(const QString &newVideo_name)
{
    video_name = newVideo_name;
}

int NormalVideoInfo::getChannel_id() const
{
    return channel_id;
}

void NormalVideoInfo::setChannel_id(int newChannel_id)
{
    channel_id = newChannel_id;
}

QString NormalVideoInfo::getCreate_time() const
{
    return create_time;
}

void NormalVideoInfo::setCreate_time(const QString &newCreate_time)
{
    create_time = newCreate_time;
}


QString NormalVideoInfo::getVideo_path() const
{
    return video_path;
}

void NormalVideoInfo::setVideo_path(const QString &newVideo_path)
{
    video_path = newVideo_path;
}

int NormalVideoInfo::getVideo_duration() const
{
    return video_duration;
}

void NormalVideoInfo::setVideo_duration(int newVideo_duration)
{
    video_duration = newVideo_duration;
}

NormalVideoInfo::NormalVideoInfo() {}
