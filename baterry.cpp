#include <iostream>
#include <windows.h>

void GetBatteryInfo() {
    SYSTEM_POWER_STATUS sps;
    if (GetSystemPowerStatus(&sps)) {
        std::cout << "AC Line Status: " << (sps.ACLineStatus == 0 ? "Offline" : "Online") << std::endl;
        std::cout << "Battery Life Percent: " << (int)sps.BatteryLifePercent << "%" << std::endl;
        std::cout << "Battery Life Time: " << sps.BatteryLifeTime << " seconds" << std::endl;
    } else {
        std::cout << "Unable to retrieve battery status." << std::endl;
    }
}
