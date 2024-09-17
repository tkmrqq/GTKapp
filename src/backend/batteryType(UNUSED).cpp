void initStruct() {
    HANDLE hDevice = CreateFile(
        getDeviceName().c_str(),
        GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
    BATTERY_INFORMATION bi;
    BATTERY_QUERY_INFORMATION bqi;
    DWORD bytesReturned;
    bqi.BatteryTag = 0;
    bqi.InformationLevel = BatteryInformation;

    DeviceIoControl(hDevice, IOCTL_BATTERY_QUERY_TAG, NULL, 0, &bqi.BatteryTag, sizeof(bqi.BatteryTag), &bytesReturned, NULL);
    DeviceIoControl(hDevice, IOCTL_BATTERY_QUERY_INFORMATION, &bqi, sizeof(bqi), &bi, sizeof(bi), &bytesReturned, NULL);
    CloseHandle(hDevice);
}

std::wstring getDeviceName()
{
    HDEVINFO deviceInfoSet = SetupDiGetClassDevs(&GUID_DEVCLASS_BATTERY, NULL, NULL, DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);

    if (deviceInfoSet == INVALID_HANDLE_VALUE)
    {
        std::cerr << "Failed to get device info set: " << GetLastError() << std::endl;
        return L"";
    }

    SP_DEVICE_INTERFACE_DATA deviceInterfaceData;
    deviceInterfaceData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);
    std::wstring devicePath = L"";

    for (int i = 0; SetupDiEnumDeviceInterfaces(deviceInfoSet, NULL, &GUID_DEVCLASS_BATTERY, i, &deviceInterfaceData); ++i)
    {
        DWORD requiredSize = 0;
        SetupDiGetDeviceInterfaceDetail(deviceInfoSet, &deviceInterfaceData, NULL, 0, &requiredSize, NULL);

        SP_DEVICE_INTERFACE_DETAIL_DATA *deviceInterfaceDetailData = (SP_DEVICE_INTERFACE_DETAIL_DATA *)malloc(requiredSize);
        deviceInterfaceDetailData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);

        if (SetupDiGetDeviceInterfaceDetail(deviceInfoSet, &deviceInterfaceData, deviceInterfaceDetailData, requiredSize, NULL, NULL))
        {
            devicePath = deviceInterfaceDetailData->DevicePath;
        }

        free(deviceInterfaceDetailData);
    }

    SetupDiDestroyDeviceInfoList(deviceInfoSet);
    return devicePath;
}

