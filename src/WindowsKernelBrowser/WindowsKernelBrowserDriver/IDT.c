#include "IDT.h"
#include "Worker.h"


NTSTATUS QueryMultiProcessorIDT(PVOID SystemBuffer, ULONG InputBufferLength, ULONG OutputBufferLength, ULONG_PTR* Information)
{
	NTSTATUS Status = STATUS_SUCCESS;

	PGateDescriptors idt = NULL;
	ULONG  itemCount = 0;

	ULONG dataSize = 0;
	ULONG needOutputBufferLength = 0;
	ULONG processorNumber = 0;
	PAGED_CODE();

	if (InputBufferLength != 4)
	{
		return STATUS_PARAMETER_QUOTA_EXCEEDED;
	}

	ULONG* tmp1 = (ULONG*)SystemBuffer;
	processorNumber = *tmp1;

	KeSetSystemAffinityThread((KAFFINITY)1 << processorNumber);
	Status = QueryCurrentIDT(&idt, &itemCount);
	KeRevertToUserAffinityThread();

	if (!NT_SUCCESS(Status))
	{
		DbgPrintEx(DPFLTR_SYSTEM_ID, DPFLTR_ERROR_LEVEL, "[-]processor %d, not find idt\n", processorNumber);
		return Status;
	}
	DbgPrintEx(DPFLTR_SYSTEM_ID, DPFLTR_ERROR_LEVEL, "[+] processor %d, idtBase %p\n", processorNumber, idt);
	DbgPrintEx(DPFLTR_SYSTEM_ID, DPFLTR_ERROR_LEVEL, "[+] processor %d, itemCount %x\n", processorNumber, itemCount);

	dataSize = sizeof(GateDescriptorsReadable) * itemCount;
	DbgPrintEx(DPFLTR_SYSTEM_ID, DPFLTR_ERROR_LEVEL, "[+] dataSize %x\n", dataSize);
	needOutputBufferLength = 1 + sizeof(PVOID) + sizeof(ULONG) + dataSize;

	if (OutputBufferLength < 5)
	{
		return STATUS_BUFFER_TOO_SMALL;
	}
	else if (OutputBufferLength < needOutputBufferLength)
	{
		*Information = 5;
		*((BOOLEAN*)SystemBuffer) = 0;//fail
		*((ULONG*)((BOOLEAN*)SystemBuffer + 1)) = needOutputBufferLength;//need buffer length
	}
	else
	{
		*Information = needOutputBufferLength;
		*((BOOLEAN*)SystemBuffer) = 1;//success
		*((PVOID*)((BOOLEAN*)SystemBuffer + 1)) = idt;//idtBase
		*((ULONG*)((BOOLEAN*)SystemBuffer + 1 + sizeof(PVOID))) = itemCount;//idtItemCount

		PGateDescriptorsReadable gateDescriptorsReadableKernelBuffer = (PGateDescriptorsReadable)((BOOLEAN*)SystemBuffer + 1 + sizeof(PVOID) + sizeof(ULONG));

		for (ULONG i = 0; i < itemCount; i++, idt++, gateDescriptorsReadableKernelBuffer++)
		{
			PVOID isrAddr = MakeISRPointer(idt->OffsetHigh, idt->OffsetMiddle, idt->OffsetLow);

			gateDescriptorsReadableKernelBuffer->InterruptServiceRoutineSegmentSelector = idt->Selector;
			gateDescriptorsReadableKernelBuffer->InterruptServiceRoutineOffset = isrAddr;
			gateDescriptorsReadableKernelBuffer->InterruptStackTableIndex = idt->IST;
			gateDescriptorsReadableKernelBuffer->TYPE = idt->TYPE;
			gateDescriptorsReadableKernelBuffer->DescriptorPrivilegeLevel = idt->DPL;
			gateDescriptorsReadableKernelBuffer->Present = idt->Present;
		}
	}
	return Status;
}

NTSTATUS QueryCurrentIDT(PGateDescriptors* idt, ULONG* itemCount)
{
	NTSTATUS Status = STATUS_SUCCESS;
	IDTRegister idtr = { 0 };

	PAGED_CODE();

	QueryCurrentIDTR(&idtr);

	if (idtr.IDTBase)
	{
		*idt = idtr.IDTBase;
		*itemCount = (idtr.Limit + 1) / 16;
	}
	else
	{
		return STATUS_NOT_FOUND;
	}
	return Status;
}
PVOID MakeISRPointer(UINT32 OffsetHigh, USHORT OffsetMiddle, USHORT OffsetLow)
{
	UINT64 tmp = 0;
	tmp = (((UINT64)OffsetHigh) << 32) | (((UINT64)OffsetMiddle) << 16) | OffsetLow;
	return (PVOID)tmp;
}