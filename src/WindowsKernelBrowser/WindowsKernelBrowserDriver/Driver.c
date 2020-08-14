#include "Driver.h"
#include "IDT.h"
#include "GDT.h"

NTSTATUS DriverEntry(_In_ PDRIVER_OBJECT DriverObject, _In_ PUNICODE_STRING RegistryPath)
{
	UINT32 i = 0;
	PDEVICE_OBJECT DeviceObject = NULL;
	NTSTATUS Status = STATUS_UNSUCCESSFUL;
	UNICODE_STRING DeviceName, DeviceNameSymbolic = { 0 };

	UNREFERENCED_PARAMETER(RegistryPath);
	PAGED_CODE();

	RtlInitUnicodeString(&DeviceName, DefDeviceName);
	RtlInitUnicodeString(&DeviceNameSymbolic, DefDeviceNameSymbolic);

	Status = IoCreateDevice(DriverObject, 0, &DeviceName, FILE_DEVICE_UNKNOWN, FILE_DEVICE_SECURE_OPEN, FALSE, &DeviceObject);

	if (!NT_SUCCESS(Status))
	{
		if (DeviceObject)
		{
			IoDeleteDevice(DeviceObject);
		}

		DbgPrintEx(DPFLTR_SYSTEM_ID, DPFLTR_ERROR_LEVEL, "[-] Error Initializing Driver\n");
		return Status;
	}

	DriverObject->DriverUnload = DriverUnloadHandler;

	for (i = 0; i <= IRP_MJ_MAXIMUM_FUNCTION; i++)
	{
		DriverObject->MajorFunction[i] = IrpNotImplementedHandler;
	}

	DriverObject->MajorFunction[IRP_MJ_CREATE] = IrpCreateHandler;
	DriverObject->MajorFunction[IRP_MJ_CLOSE] = IrpCloseHandler;
	DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = IrpDeviceIoCtlHandler;

	DeviceObject->Flags |= DO_BUFFERED_IO;
	DeviceObject->Flags &= ~DO_DEVICE_INITIALIZING;

	Status = IoCreateSymbolicLink(&DeviceNameSymbolic, &DeviceName);

	DbgPrintEx(DPFLTR_SYSTEM_ID, DPFLTR_ERROR_LEVEL, "[+] Driver Loaded\n");

	return Status;
}

VOID DriverUnloadHandler(_In_ PDRIVER_OBJECT DriverObject)
{
	UNICODE_STRING DeviceNameSymbolic = { 0 };
	PAGED_CODE();

	RtlInitUnicodeString(&DeviceNameSymbolic, DefDeviceNameSymbolic);

	IoDeleteSymbolicLink(&DeviceNameSymbolic);

	IoDeleteDevice(DriverObject->DeviceObject);
	DbgPrintEx(DPFLTR_SYSTEM_ID, DPFLTR_ERROR_LEVEL, "[-] Driver Unloaded\n");
}

NTSTATUS IrpCreateHandler(_In_ PDEVICE_OBJECT DeviceObject, _In_ PIRP Irp)
{
	PIO_STACK_LOCATION IrpSp = NULL;
	NTSTATUS Status = STATUS_NOT_SUPPORTED;

	UNREFERENCED_PARAMETER(DeviceObject);
	PAGED_CODE();

	IrpSp = IoGetCurrentIrpStackLocation(Irp);

	PVOID pPoolAdd = ExAllocatePoolWithTag(NonPagedPoolNx, 0x100, '1gaT');
	if (pPoolAdd == NULL)
	{
		Status = STATUS_NO_MEMORY;
	}
	else
	{
		IrpSp->FileObject->FsContext = pPoolAdd;
		Status = STATUS_SUCCESS;
		DbgPrintEx(DPFLTR_SYSTEM_ID, DPFLTR_ERROR_LEVEL, "[+] File Object %p\n", IrpSp->FileObject);
		DbgPrintEx(DPFLTR_SYSTEM_ID, DPFLTR_ERROR_LEVEL, "[+] Pool Addr %p\n", pPoolAdd);
	}

	Irp->IoStatus.Information = 0;
	Irp->IoStatus.Status = Status;

	IoCompleteRequest(Irp, IO_NO_INCREMENT);

	return Status;
}
NTSTATUS IrpCloseHandler(_In_ PDEVICE_OBJECT DeviceObject, _In_ PIRP Irp)
{
	PIO_STACK_LOCATION IrpSp = NULL;
	NTSTATUS Status = STATUS_SUCCESS;

	UNREFERENCED_PARAMETER(DeviceObject);
	PAGED_CODE();

	IrpSp = IoGetCurrentIrpStackLocation(Irp);

	PVOID pPoolAdd = IrpSp->FileObject->FsContext;
	ExFreePoolWithTag(pPoolAdd, 'firs');

	Irp->IoStatus.Information = 0;
	Irp->IoStatus.Status = Status;

	IoCompleteRequest(Irp, IO_NO_INCREMENT);

	return Status;
}

NTSTATUS IrpNotImplementedHandler(_In_ PDEVICE_OBJECT DeviceObject, _In_ PIRP Irp)
{
	Irp->IoStatus.Information = 0;
	Irp->IoStatus.Status = STATUS_NOT_SUPPORTED;

	UNREFERENCED_PARAMETER(DeviceObject);
	PAGED_CODE();

	IoCompleteRequest(Irp, IO_NO_INCREMENT);

	return STATUS_NOT_SUPPORTED;
}

NTSTATUS IrpDeviceIoCtlHandler(_In_ PDEVICE_OBJECT DeviceObject, _In_ PIRP Irp)
{
	ULONG IoControlCode = 0;
	PIO_STACK_LOCATION IrpSp = NULL;

	NTSTATUS Status = STATUS_NOT_SUPPORTED;
	ULONG_PTR Information = 0;

	PVOID  SystemBuffer = NULL;
	ULONG  InputBufferLength = 0;
	ULONG  OutputBufferLength = 0;

	UNREFERENCED_PARAMETER(DeviceObject);
	PAGED_CODE();

	__try
	{
		IrpSp = IoGetCurrentIrpStackLocation(Irp);

		IoControlCode = IrpSp->Parameters.DeviceIoControl.IoControlCode;
		InputBufferLength = IrpSp->Parameters.DeviceIoControl.InputBufferLength;
		OutputBufferLength = IrpSp->Parameters.DeviceIoControl.OutputBufferLength;

		//need arg check

		DbgPrintEx(DPFLTR_SYSTEM_ID, DPFLTR_ERROR_LEVEL, "[+] IoControlCode: %d\n", IoControlCode);
		DbgPrintEx(DPFLTR_SYSTEM_ID, DPFLTR_ERROR_LEVEL, "[+] InputBufferLength: %d\n", InputBufferLength);
		DbgPrintEx(DPFLTR_SYSTEM_ID, DPFLTR_ERROR_LEVEL, "[+] OutputBufferLength: %d\n", OutputBufferLength);

		SystemBuffer = Irp->AssociatedIrp.SystemBuffer;
		DbgPrintEx(DPFLTR_SYSTEM_ID, DPFLTR_ERROR_LEVEL, "[+] SystemBuffer Address: %p\n", SystemBuffer);

		if (SystemBuffer)
		{
			switch (IoControlCode)
			{
			case IOCTL_QueryIDT:
				Status = QueryMultiProcessorIDT(SystemBuffer, InputBufferLength, OutputBufferLength, &Information);
				break;
			case IOCTL_QueryGDT:
				Status = QueryMultiProcessorGDT(SystemBuffer, InputBufferLength, OutputBufferLength, &Information);
				break;
			default:
				Status = STATUS_INVALID_DEVICE_REQUEST;
				break;
			}
		}
		else
		{
			Status = STATUS_INVALID_USER_BUFFER;
		}
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		Status = GetExceptionCode();
		DbgPrintEx(DPFLTR_SYSTEM_ID, DPFLTR_ERROR_LEVEL, "[-] Exception Code: 0x%X\n", Status);
	}

	//
	// Update the IoStatus information
	//

	Irp->IoStatus.Status = Status;
	Irp->IoStatus.Information = Information;

	//
	// Complete the request
	//

	IoCompleteRequest(Irp, IO_NO_INCREMENT);

	return Status;
}