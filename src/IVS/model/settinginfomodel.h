#ifndef SETTINGINFOMODEL_H
#define SETTINGINFOMODEL_H

#include "../util/dbconn.h"
#include "settinginfo.h"
#include "../util/dbconnectionpool.h"


class SettingInfoModel
{
public:
    SettingInfoModel();
    void saveSettingInfo();
};

#endif // SETTINGINFOMODEL_H
