#ifndef BATTERY_INFO_HPP
#define BATTERY_INFO_HPP

#include <windows.h>
#include <string>
#include <iostream>
#include <powrprof.h>
#include <winioctl.h>
#include <logger.hpp>

#include <batclass.h>
#include <devguid.h>
#include <setupapi.h>
#include <cstring> 
#include <cstdlib>

#pragma comment(lib, "PowrProf.lib")

class BatteryInfo {
public:
    BatteryInfo();

    void updateBatteryInfo();
    void displayBatteryInfo();
    void getUpTime();
    void logBatteryInfo();

    void sleepMode();
    void hibernateMode();
    void checkBatteryStatus(SYSTEM_POWER_STATUS sps);

private:
    std::string powerSource;
    std::string batteryType;
    std::string batteryLifePercent;
    std::string batteryPowerState;
    int batteryLevel;
    std::string powerSavingMode;
    std::string batteryLifeRemaining;
    std::string upTime;
    int batteryTime = 0;

    std::string getPowerSource(BYTE acLineStatus);
    std::string getBatteryType();
    std::string getPowerSavingMode(BYTE systemStatusFlag);
    std::string getBatteryLifeRemaining(DWORD batteryLifeTime, bool isCharging);
};

#endif
