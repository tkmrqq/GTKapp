#include <iostream>
#include <batteryInfo.hpp>

// variant 1

// BatteryFlag
// 1 - charging
// 2 - charged
// 4 - critical low level
// 8 - power saving
// 128 - no battery

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


int main(int argc, char *argv[]) {
    BatteryInfo battery;
    if (argc > 1)
    {
        std::string arg = argv[1];
        if (arg == "--sleep")
        {
            Sleep(5000);
            BatteryInfo::sleepMode();
            return 1;
        }
        else if (arg == "--hibernate")
        {
            Sleep(5000);
            BatteryInfo::hibernateMode();
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
            std::cout << "Unknown argument." << std::endl;
        }
    }
    while (true)
    {
        battery.updateBatteryInfo();
        system("cls");
        battery.displayBatteryInfo();
        battery.logBatteryInfo();
        Sleep(1000);
    }
    return 0;
}
