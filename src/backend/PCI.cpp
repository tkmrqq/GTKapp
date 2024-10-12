#include "string"
#include <cstdio>
#include <cstring>
#include <iostream>
#include <logger.hpp>
#include <sstream>
#include <windows.h>
#include <iomanip>

#define IOCTL_CODE CTL_CODE(FILE_DEVICE_UNKNOWN, 0x3000, METHOD_BUFFERED, GENERIC_READ | GENERIC_WRITE)

Logger logger(LOG_DIR "/pci_log.txt");

struct DeviceRequest {
  ULONG bus;
  ULONG slot;
  ULONG func;
};

struct DeviceResponse {
  ULONG vendorID;
  ULONG deviceID;
  ULONG func;
};

// Function to send a request to the driver via IOCTL
BOOL GetPciDeviceInfo(HANDLE hDevice, ULONG bus, ULONG slot, ULONG func,
                      ULONG *vendorID, ULONG *deviceID) {
  struct DeviceRequest request = {bus, slot, func};
  struct DeviceResponse response = {0};

  DWORD bytesReturned;
  BOOL result = DeviceIoControl(hDevice,          // Device handle
                                IOCTL_CODE,       // IOCTL code
                                &request,         // Input data
                                sizeof(request),  // Input data size
                                &response,        // Output data
                                sizeof(response), // Output data size
                                &bytesReturned,   // Number of bytes transferred
                                NULL); // Variable for synchronization

  if (result) {
    *vendorID = response.vendorID;
    *deviceID = response.deviceID;
    return TRUE;
  } else {
    DWORD error = GetLastError(); // Log error
    printf("Error in DeviceIoControl call: %lu\n", error);
  }

  return FALSE;
}

// Function to find vendor and device names from pci.ids
void FindDeviceName(ULONG vendorID, ULONG deviceID, std::string* vendorName, std::string *deviceName) {
  std::ifstream file("C:\\GitHub\\GTKapp\\src\\backend\\pci.ids");

  if (!file.is_open()) {
    printf("Error: could not open pci.ids file.\n");
    return;
  }

  std::string line;

  bool flag = false;

  while (std::getline(file, line)) {

    if(!line.empty() && line[0] == '#') {
      continue;
    }

    //find device
    if (!line.empty() && flag && line[0] == '\t') {
      std::string dID = line.substr(1, 4);
      ULONG lineDeviceID;
      std::stringstream deviceSS;
      deviceSS << std::hex << dID;
      deviceSS >> lineDeviceID;

      if (lineDeviceID == deviceID) {
        *deviceName = line.substr(6);
      }
      continue;
    }

    if (flag && (line.empty() || line[0] != '\t')) {
      flag = false;
    }

    std::string vID = line.substr(0,4);
    ULONG lineVendorID;
    std::stringstream ss;
    ss << std::hex << vID;
    ss >> lineVendorID;

    //find vendor
    if (lineVendorID == vendorID) {
      *vendorName = line.substr(5);
      flag = true;
    }
  }

}


int main() {
  // Open the driver
  HANDLE hDevice =
      CreateFileW((L"\\??\\PCI_Habr_Link"), GENERIC_READ | GENERIC_WRITE, 0,
                  NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

  if (hDevice == INVALID_HANDLE_VALUE) {
    DWORD error = GetLastError(); // Get error code
    printf("Error: could not open driver, error code: %lu\n", error);
    return 1;
  }

  // Print table header for device information
  printf("%s|%-4s|%4s|%-9s|%-9s| %-40s | %-60s\n", "Bus",
         "Slot", "Func", "Vendor ID", "Device ID", "Vendor Name",
         "Device Name");
  std::ostringstream logStream;
  for (ULONG bus = 0; bus < 256; bus++) {
    for (ULONG slot = 0; slot < 32; slot++) {
      for (ULONG func = 0; func < 8; func++) {
        ULONG vendorID = 0xFFFF, deviceID = 0xFFFF;

        if (GetPciDeviceInfo(hDevice, bus, slot, func, &vendorID, &deviceID)) {
          if (vendorID != 0xFFFF && deviceID != 0xFFFF) {
            std::string vendorName = "Not found";
            std::string deviceName = "Not found";

            FindDeviceName(vendorID, deviceID, &vendorName, &deviceName);

            std::cout << std::setw(2) << bus << " | "
                      << std::setw(2) << slot << " | "
                      << std::setw(2) << func << " | "
                      << std::setw(7) << vendorID << " | "
                      << std::setw(7) << deviceID << " | "
                      << std::setw(40) << vendorName << " | "
                      << std::setw(60) << deviceName << std::endl;

            logStream << bus << " "
                      << slot << " "
                      << func << " "
                      << vendorID << " "
                      << deviceID << " "
                      << vendorName << "$"
                      << deviceName << "\n";
          }
        }
      }
    }
  }
  std::string logMsg = logStream.str();

  if (!logMsg.empty() && logMsg.back() == '\n') {
    logMsg.pop_back();
  }
  logger.log(logMsg);

  CloseHandle(hDevice);
  return 0;
}
