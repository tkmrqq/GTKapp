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

    static void sleepMode();
    static void hibernateMode();

private:
    std::string powerSource;
    std::string batteryType;
    std::string batteryLifePercent;
    std::string batteryPowerState;
    std::string powerSavingMode;
    std::string batteryLifeRemaining;
    std::string upTime;
    int designCapacity;
    int fullChargedCapacity;
    int btHealth = 0;
    int batteryLevel;

    int batteryTime = 0;

    std::string getPowerSource(BYTE acLineStatus);
    std::string getBatteryType();
    static std::string getPowerSavingMode(BYTE systemStatusFlag);
    static std::string getBatteryLifeRemaining(DWORD batteryLifeTime);
};

#endif
