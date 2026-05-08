#include "settinginfoservice.h"

SettingInfoService::SettingInfoService() {}

void SettingInfoService::saveSettingToDb()
{
    SettingInfoModel sim;
    sim.saveSettingInfo();
}
