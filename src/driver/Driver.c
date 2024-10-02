#include <ntddk.h>
#include <wdf.h>

#define IOCTL_CODE CTL_CODE(FILE_DEVICE_UNKNOWN, 0x3000, METHOD_BUFFERED, GENERIC_READ | GENERIC_WRITE)

struct DeviceRequest
{
    ULONG a;
    ULONG b;
    ULONG c;
};

struct DeviceResponse
{
    ULONG result1;
    ULONG result2;
    ULONG result3;
};

ULONG ReadPciConfig(ULONG bus, ULONG slot, ULONG func, ULONG offset)
{
    ULONG address;
    ULONG data;

    address = (ULONG)((bus << 16) | (slot << 11) | (func << 8) | (offset & 0xFC) | ((ULONG)0x80000000));

    __outdword(0xCF8, address);

    data = __indword(0xCFC);

    return data;
}

DRIVER_INITIALIZE DriverEntry;
EVT_WDF_DRIVER_DEVICE_ADD KmdfHelloWorldEvtDeviceAdd;

NTSTATUS DriverEntry(
    _In_ PDRIVER_OBJECT DriverObject,
    _In_ PUNICODE_STRING RegistryPath
)
{
    NTSTATUS status = STATUS_SUCCESS;
    WDF_DRIVER_CONFIG config;
    KdPrintEx((DPFLTR_IHVDRIVER_ID, DPFLTR_INFO_LEVEL, "HelloWorld: DriverEntry\n"));

    WDF_DRIVER_CONFIG_INIT(&config, KmdfHelloWorldEvtDeviceAdd);
    status = WdfDriverCreate(DriverObject, RegistryPath, WDF_NO_OBJECT_ATTRIBUTES, &config, WDF_NO_HANDLE);
    return status;
}

VOID HandleIOCTL(
    _In_ WDFQUEUE Queue,
    _In_ WDFREQUEST Request,
    _In_ size_t OutputBufferLength,
    _In_ size_t InputBufferLength,
    _In_ ULONG IoControlCode
)
{
    NTSTATUS status = STATUS_SUCCESS;
    UNREFERENCED_PARAMETER(Queue);
    UNREFERENCED_PARAMETER(InputBufferLength);
    UNREFERENCED_PARAMETER(OutputBufferLength);

    size_t returnBytes = 0;

    switch (IoControlCode)
    {
    case IOCTL_CODE:
    {
        struct DeviceRequest* request_data = NULL;
        struct DeviceResponse* response_data = NULL;
        PVOID inputBuffer = NULL;
        PVOID outputBuffer = NULL;
        size_t length = 0;

        status = WdfRequestRetrieveInputBuffer(Request, sizeof(struct DeviceRequest), &inputBuffer, &length);
        if (!NT_SUCCESS(status) || length != sizeof(struct DeviceRequest) || !inputBuffer) {
            status = STATUS_INVALID_DEVICE_REQUEST;
            break;
        }
        request_data = (struct DeviceRequest*)inputBuffer;

        status = WdfRequestRetrieveOutputBuffer(Request, sizeof(struct DeviceResponse), &outputBuffer, &length);
        if (!NT_SUCCESS(status) || length != sizeof(struct DeviceResponse) || !outputBuffer) {
            status = STATUS_INVALID_DEVICE_REQUEST;
            break;
        }
        response_data = (struct DeviceResponse*)outputBuffer;

        ULONG configData = ReadPciConfig(request_data->a, request_data->b, request_data->c, 0);
        ULONG vendorID = (ULONG)(configData & 0xFFFF);
        ULONG deviceID = (ULONG)((configData >> 16) & 0xFFFF);
        ULONG func = request_data->c;

        response_data->result1 = vendorID;
        response_data->result2 = deviceID;
        response_data->result3 = func;
        returnBytes = sizeof(struct DeviceResponse);
        break;
    }
    default:
        status = STATUS_INVALID_DEVICE_REQUEST;
        break;
    }

    WdfRequestCompleteWithInformation(Request, status, returnBytes);
}

NTSTATUS KmdfHelloWorldEvtDeviceAdd(
    _In_    WDFDRIVER       Driver,
    _Inout_ PWDFDEVICE_INIT DeviceInit
)
{
    UNREFERENCED_PARAMETER(Driver);
    NTSTATUS status;
    UNICODE_STRING symLinkName = { 0 };
    UNICODE_STRING deviceFileName = { 0 };
    UNICODE_STRING securitySetting = { 0 };

    RtlInitUnicodeString(&symLinkName, L"\\DosDevices\\PCI_Habr_Link");
    RtlInitUnicodeString(&deviceFileName, L"\\Device\\PCI_Habr_Dev");
    RtlInitUnicodeString(&securitySetting, L"D:P(A;;GA;;;SY)(A;;GA;;;BA)");

    WdfDeviceInitAssignSDDLString(DeviceInit, &securitySetting);
    WdfDeviceInitSetCharacteristics(DeviceInit, FILE_DEVICE_SECURE_OPEN, FALSE);
    status = WdfDeviceInitAssignName(DeviceInit, &deviceFileName);

    if (!NT_SUCCESS(status)) {
        WdfDeviceInitFree(DeviceInit);
        return status;
    }

    WDFDEVICE hDevice;
    status = WdfDeviceCreate(&DeviceInit, WDF_NO_OBJECT_ATTRIBUTES, &hDevice);
    if (!NT_SUCCESS(status)) {
        return status;
    }

    status = WdfDeviceCreateSymbolicLink(hDevice, &symLinkName);
    if (!NT_SUCCESS(status)) {
        return status;
    }

    WDF_IO_QUEUE_CONFIG ioQueueConfig;
    WDFQUEUE hQueue;
    WDF_IO_QUEUE_CONFIG_INIT_DEFAULT_QUEUE(&ioQueueConfig, WdfIoQueueDispatchSequential);
    ioQueueConfig.EvtIoDeviceControl = HandleIOCTL;

    status = WdfIoQueueCreate(hDevice, &ioQueueConfig, WDF_NO_OBJECT_ATTRIBUTES, &hQueue);
    return status;
}