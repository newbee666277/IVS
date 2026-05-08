#include "exceptionvideoinfo.h"

QString ExceptionVideoInfo::getAdmin_name() const
{
    return admin_name;
}

void ExceptionVideoInfo::setAdmin_name(const QString &newAdmin_name)
{
    admin_name = newAdmin_name;
}

int ExceptionVideoInfo::getChannel_id() const
{
    return channel_id;
}

void ExceptionVideoInfo::setChannel_id(int newChannel_id)
{
    channel_id = newChannel_id;
}

QString ExceptionVideoInfo::getEvent_time() const
{
    return event_time;
}

void ExceptionVideoInfo::setEvent_time(const QString &newEvent_time)
{
    event_time = newEvent_time;
}

QString ExceptionVideoInfo::getEvent_desc() const
{
    return event_desc;
}

void ExceptionVideoInfo::setEvent_desc(const QString &newEvent_desc)
{
    event_desc = newEvent_desc;
}

QString ExceptionVideoInfo::getRelated_video_path() const
{
    return related_video_path;
}

void ExceptionVideoInfo::setRelated_video_path(const QString &newRelated_video_path)
{
    related_video_path = newRelated_video_path;
}

QString ExceptionVideoInfo::getVideo_name() const
{
    return video_name;
}

void ExceptionVideoInfo::setVideo_name(const QString &newVideo_name)
{
    video_name = newVideo_name;
}

int ExceptionVideoInfo::getVideo_duration() const
{
    return video_duration;
}

void ExceptionVideoInfo::setVideo_duration(int newVideo_duration)
{
    video_duration = newVideo_duration;
}

ExceptionVideoInfo::ExceptionVideoInfo() {}
