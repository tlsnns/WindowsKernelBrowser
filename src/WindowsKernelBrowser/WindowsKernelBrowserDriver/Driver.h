#pragma once
#include <ntddk.h>

#define DefDeviceName L"\\Device\\KernelBrowser"
#define DefDeviceNameSymbolic L"\\DosDevices\\KernelBrowser"

#define IOCTL_QueryIDT CTL_CODE(FILE_DEVICE_UNKNOWN, 0x800, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_QueryGDT CTL_CODE(FILE_DEVICE_UNKNOWN, 0x801, METHOD_BUFFERED, FILE_ANY_ACCESS)

//
// Dispatch Declarations
//
DRIVER_INITIALIZE DriverEntry;
DRIVER_UNLOAD     DriverUnloadHandler;

__drv_dispatchType(IRP_MJ_CREATE)
DRIVER_DISPATCH IrpCreateHandler;

__drv_dispatchType(IRP_MJ_CLOSE)
DRIVER_DISPATCH IrpCloseHandler;

__drv_dispatchType(IRP_MJ_DEVICE_CONTROL)
DRIVER_DISPATCH IrpDeviceIoCtlHandler;

__drv_dispatchType(IRP_MJ_MAXIMUM_FUNCTION)
DRIVER_DISPATCH IrpNotImplementedHandler;

//
// Function Definitions
//
NTSTATUS DriverEntry(_In_ PDRIVER_OBJECT DriverObject, _In_ PUNICODE_STRING RegistryPath);
VOID DriverUnloadHandler(_In_ PDRIVER_OBJECT DriverObject);
NTSTATUS IrpCreateHandler(_In_ PDEVICE_OBJECT DeviceObject, _In_ PIRP Irp);
NTSTATUS IrpCloseHandler(_In_ PDEVICE_OBJECT DeviceObject, _In_ PIRP Irp);
NTSTATUS IrpDeviceIoCtlHandler(_In_ PDEVICE_OBJECT DeviceObject, _In_ PIRP Irp);
NTSTATUS IrpNotImplementedHandler(_In_ PDEVICE_OBJECT DeviceObject, _In_ PIRP Irp);

//
#ifdef ALLOC_PRAGMA
#pragma alloc_text(INIT, DriverEntry)
#pragma alloc_text(PAGE, DriverUnloadHandler)
#pragma alloc_text(PAGE, IrpCreateHandler)
#pragma alloc_text(PAGE, IrpCloseHandler)
#pragma alloc_text(PAGE, IrpDeviceIoCtlHandler)
#pragma alloc_text(PAGE, IrpNotImplementedHandler)
#endif