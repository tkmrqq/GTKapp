#include <cstdio>
#include <cstring>
#include <windows.h>
#include <logger.hpp>

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
void FindDeviceName(ULONG vendorID, ULONG deviceID, char *vendorName,
                    char *deviceName) {
  FILE *file;
  fopen_s(&file, "C:\\GitHub\\GTKapp\\src\\backend\\pci.ids", "r");
  if (file == NULL) {
    printf("Error: could not open pci.ids file.\n");
    return;
  }

  char line[512];
  int foundVendor = 0;

  // First pass: find vendor name
  while (fgets(line, sizeof(line), file)) {
    ULONG vid;
    // Check for vendor line
    if (sscanf_s(line, "%4lX %[^\n]", &vid, vendorName, 512) == 2) {
      if (vid == vendorID) {
        foundVendor = 1; // Mark that the vendor has been found
        break;           // Exit loop as we've found the vendor
      }
    }
  }

  // If vendor found, proceed to find device name
  if (foundVendor) {
    // Rewind file to search for device ID
    rewind(file);

    while (fgets(line, sizeof(line), file)) {
      ULONG did;
      // Check for device line
      if (sscanf_s(line, " %4lX %[^\n]", &did, deviceName, 512) == 2) {
        if (did == deviceID) {
          // Found the device name for the matched device ID
          break; // Exit loop since we've found both names
        }
      }
    }
  }

  fclose(file);
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
  printf("| %-4s | %-4s | %-9s | %-11s | %-11s | %-40s | %-60s |\n", "Bus",
         "Slot", "Func", "Vendor ID", "Device ID", "Vendor Name",
         "Device Name");
  std::string logMsg;
  // Iterate through all possible devices on the PCI bus
  for (ULONG bus = 0; bus < 256; bus++) {
    for (ULONG slot = 0; slot < 32; slot++) {
      for (ULONG func = 0; func < 8; func++) {
        ULONG vendorID = 0xFFFF, deviceID = 0xFFFF;

        // Request information from the driver
        if (GetPciDeviceInfo(hDevice, bus, slot, func, &vendorID, &deviceID)) {
          // If device is found
          if (vendorID != 0xFFFF && deviceID != 0xFFFF) {
            // Variables to store names
            char vendorName[512] = "Not found";
            char deviceName[512] = "Not found";

            // Find and print the device name
            FindDeviceName(vendorID, deviceID, vendorName, deviceName);

            // Print device information in columns
            printf("| %-4lu | %-4lu | %-9lu | %04lX        | %04lX        | "
                   "%-40.39s | %-60.59s |\n",
                   bus, slot, func, vendorID, deviceID, vendorName, deviceName);
            logMsg += std::to_string(bus) + " ";
            logMsg += std::to_string(slot) + " ";
            logMsg += std::to_string(func) + " ";
            logMsg += std::to_string(vendorID) + " ";
            logMsg += std::to_string(deviceID) + " ";
            logMsg += vendorName;
            logMsg += deviceName;
            logMsg += "\n";
          }
        }
      }
    }
  }
  logger.log(logMsg);

  CloseHandle(hDevice);
  return 0;
}
