#include <iostream>
#include <windows.h>
#include <powrprof.h>
#include <logger.hpp>
#pragma comment(lib, "PowrProf.lib") // Линковка с powrprof.lib

// variant 1
// TODO:
// test add to reg
// how to cls work in bg
//

// BatteryFlag
// 1 - charging
// 2 - charged
// 4 - critical low level
// 8 - power saving
// 128 - no battery

Logger logger("battery_log.txt");

bool isInStartUp()
{
    HKEY hkey;
    const char *keyPath = "Software\\Microsoft\\Windows\\CurrentVersion\\Run";
    const char *valueName = "BatteryMonitor";
    char value[MAX_PATH] = {0};
    DWORD value_length = MAX_PATH;

    if (RegOpenKey(HKEY_CURRENT_USER, keyPath, &hkey) == ERROR_SUCCESS)
    {
        if (RegQueryValueEx(hkey, valueName, NULL, NULL, (LPBYTE)value, &value_length) == ERROR_SUCCESS)
        {
            RegCloseKey(hkey);
            return true;
        }
        RegCloseKey(hkey);
    }
    return false;
}

void toStartUp()
{
    char path[MAX_PATH];
    GetModuleFileName(NULL, path, MAX_PATH);
    HKEY hkey;
    const char *keyPath = "Software\\Microsoft\\Windows\\CurrentVersion\\Run";
    const char *valueName = "BatteryMonitor";

    if (RegOpenKey(HKEY_CURRENT_USER, keyPath, &hkey) == ERROR_SUCCESS)
    {
        if (RegSetValueEx(hkey, valueName, 0, REG_SZ, (BYTE *)path, strlen(path) + 1) == ERROR_SUCCESS)
        {
            std::cout << "Added to autoload!" << std::endl;
        }
        else
        {
            std::cerr << "Error to add." << std::endl;
        }
        RegCloseKey(hkey);
    }
    else
    {
        std::cerr << "Unable to open reg." << std::endl;
    }
}

std::string getBatteryPowerState(BYTE btFlag)
{
    if (btFlag == 128) {
        return "No battery";
    }
    else if (btFlag & 1) {
        return "High";
    }
    else if (btFlag & 2) {
        return "Low";
    }
    else if (btFlag & 4) {
        return "Critical low";
    }
    return "N/A";
}

void getUpTime(std::string &logMsg) {
    ULONGLONG uptimeMillis = GetTickCount64();
    ULONGLONG uptimeSeconds = uptimeMillis / 1000;
    int hours = uptimeSeconds / 3600;
    int minutes = (uptimeSeconds % 3600) / 60;
    int seconds = uptimeSeconds % 60;

    logMsg += ("System uptime: " + std::to_string(hours) + "h " + std::to_string(minutes) + "m " + std::to_string(seconds) + "s\n");
}

void GetBatteryInfo()
{
    SYSTEM_POWER_STATUS sps;
    if (GetSystemPowerStatus(&sps)) {
        std::string logMsg;
        logMsg += ("AC Line Status: " + std::string((sps.ACLineStatus == 0 ? "Offline" : "Online")) + "\n");
        logMsg += ("Battery Life Percent: " + std::to_string((int)sps.BatteryLifePercent) + "%\n");
        logMsg += ("Battery power state: " + getBatteryPowerState(sps.BatteryFlag) + "\n"); 
        logMsg += ("Power saving mode: " + std::string(sps.SystemStatusFlag == 0 ? "disabled" : "enabled") + "\n");
        getUpTime(logMsg);
        DWORD btTime;
        std::string timeMessage;
        if (sps.ACLineStatus == 1) {  //charging
            btTime = sps.BatteryFullLifeTime;
            timeMessage = "Battery time to full charge: ";
        } else {
            btTime = sps.BatteryLifeTime;
            timeMessage = "Battery life remaining: ";
        }
        if (btTime == (DWORD)-1) {
            logMsg += (timeMessage + "N/A");
        } else {
            int hours = btTime / 3600;
            int minutes = (btTime % 3600) / 60;
            logMsg += (timeMessage + std::to_string(hours) + "h " + std::to_string(minutes) + "m");
        }
        logger.log(logMsg);
    } else {
        logger.log("Unable to retrieve battery status.\n");
    }
}

void sleepMode()
{
    if (SetSuspendState(FALSE, FALSE, FALSE))
    {
        std::cout << "Enter sleep mode" << std::endl;
    }
    else
    {
        std::cout << "Error to enter sleep mode" << std::endl;
    }
}

void hibernateMode()
{
    if (SetSuspendState(TRUE, FALSE, FALSE))
    {
        std::cout << "Enter hibernate mode" << std::endl;
    }
    else
    {
        std::cout << "Error to enter hibernate" << std::endl;
    }
}

int main(int argc, char *argv[])
{
    if (argc > 1)
    {
        std::string arg = argv[1];
        if (arg == "--sleep")
        {
            Sleep(5000);
            sleepMode();
            return 1;
        }
        else if (arg == "--hibernate")
        {
            Sleep(5000);
            hibernateMode();
            return 1;
        }
        else if (arg == "--add-to-startup") {
            if (!isInStartUp())
            {
                toStartUp();
            }
            else{
                std::cout << "Already in startup";
                Sleep(2000);
            }
        }
        else
        {
            std::cout << "Unknow argument." << std::endl;
        }
    }
    while (true)
    {
        system("cls");
        GetBatteryInfo();
        Sleep(1000);
    }
    return 0;
}
