#ifndef VIDEOINFOMODEL_H
#define VIDEOINFOMODEL_H

#include "../util/dbconn.h"
#include "normalvideoinfo.h"
#include "exceptionvideoinfo.h"
#include "util/dbconnectionpool.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

class VideoInfoModel
{
public:
    VideoInfoModel();
    int insertNormalVideoInfo(NormalVideoInfo &video_info);
    int insertExceptionlVideoInfo(ExceptionVideoInfo &video_info);
};

#endif // VIDEOINFOMODEL_H
