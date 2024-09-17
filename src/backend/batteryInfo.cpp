#include "batteryInfo.hpp"

BatteryInfo::BatteryInfo() : batteryLevel(0) {}

std::string getBatteryPowerState(BYTE btFlag)
{
    if (btFlag == 128)
    {
        return "No battery";
    }
    else if (btFlag & 1)
    {
        return "High";
    }
    else if (btFlag & 2)
    {
        return "Low";
    }
    else if (btFlag & 4)
    {
        return "Critical low";
    }
    return "N/A";
}

std::string BatteryInfo::getBatteryType()
{
    return "N/A";
}

std::string BatteryInfo::getPowerSource(BYTE acLineStatus)
{
    if (acLineStatus == 0)
    {
        batteryTime++;
        return "Battery";
    }
    else
    {
        batteryTime = 0;
        return "AC Power";
    }
}

std::string BatteryInfo::getPowerSavingMode(BYTE systemStatusFlag)
{
    return (systemStatusFlag == 0) ? "Disabled" : "Enabled";
}

std::string BatteryInfo::getBatteryLifeRemaining(DWORD batteryLifeTime, bool isCharging)
{
    if (batteryLifeTime == (DWORD)-1)
    {
        return "N/A";
    }
    int hours = batteryLifeTime / 3600;
    int minutes = (batteryLifeTime % 3600) / 60;
    return std::to_string(hours) + "h " + std::to_string(minutes) + "m";
}

void BatteryInfo::updateBatteryInfo()
{
    SYSTEM_POWER_STATUS sps;
    if (GetSystemPowerStatus(&sps))
    {
        powerSource = getPowerSource(sps.ACLineStatus);
        batteryLifePercent = std::to_string(sps.BatteryLifePercent) + "%";
        batteryPowerState = getBatteryPowerState(sps.BatteryFlag);
        powerSavingMode = getPowerSavingMode(sps.SystemStatusFlag);
        batteryLifeRemaining = getBatteryLifeRemaining(sps.BatteryLifeTime, sps.ACLineStatus == 1);
        batteryType = getBatteryType();
        getUpTime();
    }
    else
    {
        std::cerr << "Unable to retrieve battery status." << std::endl;
    }
}

void BatteryInfo::getUpTime()
{
    ULONGLONG uptimeMillis = GetTickCount64();
    ULONGLONG uptimeSeconds = uptimeMillis / 1000;
    int hours = uptimeSeconds / 3600;
    int minutes = (uptimeSeconds % 3600) / 60;
    int seconds = uptimeSeconds % 60;

    upTime = std::to_string(hours) + "h " + std::to_string(minutes) + "m " + std::to_string(seconds) + "s";
}

void BatteryInfo::displayBatteryInfo()
{
    std::cout << "Power Source: " << powerSource << std::endl;
    std::cout << "Battery Type: " << batteryType << std::endl;
    std::cout << "Battery Life Percent: " << batteryLifePercent << std::endl;
    std::cout << "Battery Power State: " << batteryPowerState << std::endl;
    std::cout << "Power Saving Mode: " << powerSavingMode << std::endl;
    std::cout << "Battery Life Remaining: " << batteryLifeRemaining << std::endl;
    std::cout << "System Uptime: " << upTime << std::endl;
    std::cout << "Battery work time: " << batteryTime << std::endl;
}

void BatteryInfo::sleepMode()
{
    if (SetSuspendState(FALSE, FALSE, FALSE))
    {
        std::cout << "System entered sleep mode." << std::endl;
    }
    else
    {
        std::cerr << "Failed to enter sleep mode." << std::endl;
    }
}

void BatteryInfo::hibernateMode()
{
    if (SetSuspendState(TRUE, FALSE, FALSE))
    {
        std::cout << "System entered hibernate mode." << std::endl;
    }
    else
    {
        std::cerr << "Failed to enter hibernate mode." << std::endl;
    }
}

Logger logger("battery_log.txt");

void BatteryInfo::logBatteryInfo()
{
    std::string logMsg;
    logMsg += ("Power Source: " + powerSource + "\n");
    logMsg += ("Battery Type: " + batteryType + "\n");
    logMsg += ("Battery Life Percent: " + batteryLifePercent + "\n");
    logMsg += ("Battery Power State: " + batteryPowerState + "\n");
    logMsg += ("Power Saving Mode: " + powerSavingMode + "\n");
    logMsg += ("Battery Life Remaining: " + batteryLifeRemaining + "\n");
    logMsg += ("System Uptime: " + upTime + "\n");
    logMsg += ("Battery work time: " + std::to_string(batteryTime) + "\n");
    logger.log(logMsg);
}