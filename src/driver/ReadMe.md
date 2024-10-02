# I/O Ports Kernel Driver
Print list of devices, that connected to PCI bus
'DeviceID', 'VendorID'

# Dependencies
- WDK x64
- Windows SDK.CPP x64 
- Windows SDK.CPP
Version: 10.0.26100.1591

# Compile
'use Visual Studio with WDK, SDK packages'

# Install
To install disable Driver Signature enforcement:
1. Use Advanced Boot Menu
2. Enable Test Signing Mode
3. Disable device Driver Signing 

```
bcdedit.exe /set nointegritychecks on 
bcdedit.exe /set nointegritychecks off

bcdedit /set testsigning off 
bcdedit /set testsigning on 
```

>[!NOTE]
>Don't forget run as administrator

