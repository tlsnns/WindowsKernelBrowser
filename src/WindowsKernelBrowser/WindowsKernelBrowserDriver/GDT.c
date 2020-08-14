#include "GDT.h"
#include "Worker.h"

NTSTATUS QueryMultiProcessorGDT(PVOID SystemBuffer, ULONG InputBufferLength, ULONG OutputBufferLength, ULONG_PTR* Information)
{
	PSegmentDescriptors gdt = NULL;
	UINT32  itemCount = 0;

	UINT32 dataSize = 0;
	UINT32 needOutputBufferLength = 0;
	UINT32 processorNumber = 0;
	PAGED_CODE();

	if (InputBufferLength != 4)
	{
		return STATUS_PARAMETER_QUOTA_EXCEEDED;
	}

	ULONG* tmp1 = (ULONG*)SystemBuffer;
	processorNumber = *tmp1;
	KeSetSystemAffinityThread((KAFFINITY)1 << processorNumber);
	QueryCurrentGDT(&gdt, &itemCount);
	KeRevertToUserAffinityThread();

	if (!gdt)
	{
		DbgPrintEx(DPFLTR_SYSTEM_ID, DPFLTR_ERROR_LEVEL, "[-]processor %d, not find gdt\n", processorNumber);
		return STATUS_NOT_FOUND;
	}

	DbgPrintEx(DPFLTR_SYSTEM_ID, DPFLTR_ERROR_LEVEL, "[+] processor %d, GDTBase %p\n", processorNumber, gdt);
	DbgPrintEx(DPFLTR_SYSTEM_ID, DPFLTR_ERROR_LEVEL, "[+] processor %d, itemCount %x\n", processorNumber, itemCount);

	dataSize = sizeof(SegmentDescriptorsReadable) * itemCount;
	DbgPrintEx(DPFLTR_SYSTEM_ID, DPFLTR_ERROR_LEVEL, "[+] dataSize %x\n", dataSize);
	needOutputBufferLength = 1 + sizeof(PVOID) + sizeof(UINT32) + dataSize;

	if (OutputBufferLength < 5)
	{
		return STATUS_BUFFER_TOO_SMALL;
	}
	else if (OutputBufferLength < needOutputBufferLength)
	{
		*Information = 5;
		*((BOOLEAN*)SystemBuffer) = 0;//fail
		*((UINT32*)((BOOLEAN*)SystemBuffer + 1)) = needOutputBufferLength;//need buffer length
	}
	else
	{
		*Information = needOutputBufferLength;
		*((BOOLEAN*)SystemBuffer) = 1;//success
		*((PVOID*)((BOOLEAN*)SystemBuffer + 1)) = gdt;//gdtBase
		*((ULONG*)((BOOLEAN*)SystemBuffer + 1 + sizeof(PVOID))) = itemCount;//gdtItemCount

		PSegmentDescriptorsReadable segmentDescriptorsReadableKernelBuffer = (PSegmentDescriptorsReadable)((BOOLEAN*)SystemBuffer + 1 + sizeof(PVOID) + sizeof(UINT32));

		for (UINT8 i = 0, j = 0; i < itemCount; i++, j++, gdt++, segmentDescriptorsReadableKernelBuffer++)
		{
			PVOID Addr = NULL;
			UINT32 limit = 0;

			limit = MakeSegmentLimit(gdt->LimitHigh, gdt->LimitLow);

			segmentDescriptorsReadableKernelBuffer->Index = (UINT8)j;
			segmentDescriptorsReadableKernelBuffer->Limit = limit;
			segmentDescriptorsReadableKernelBuffer->Accessed = gdt->A;
			segmentDescriptorsReadableKernelBuffer->ReadWrite = gdt->RW;
			segmentDescriptorsReadableKernelBuffer->ConformingExpandDown = gdt->CE;
			segmentDescriptorsReadableKernelBuffer->CodeData = gdt->CD;
			segmentDescriptorsReadableKernelBuffer->NonSystem = gdt->S;
			segmentDescriptorsReadableKernelBuffer->DescriptorPrivilegeLevel = gdt->DPL;
			segmentDescriptorsReadableKernelBuffer->Present = gdt->P;
			segmentDescriptorsReadableKernelBuffer->AvailableAndReservedBit = gdt->AVL;
			segmentDescriptorsReadableKernelBuffer->CodeSegment64Bit = gdt->L;
			segmentDescriptorsReadableKernelBuffer->DefaultOperationSizeAndDefaultStackPointerSizeAndUpperBound = gdt->DB;
			segmentDescriptorsReadableKernelBuffer->Granularity = gdt->G;

			if ((gdt->P) && !(gdt->S))
			{
				PSystemSegmentDescriptors SegmentDescriptorsExpand = (PSystemSegmentDescriptors)(gdt + 1);
				Addr = MakeSegmentPointer(SegmentDescriptorsExpand->Address, gdt->AddressHigh, gdt->AddressMiddle, gdt->AddressLow);
				segmentDescriptorsReadableKernelBuffer->Address = Addr;
				gdt++;
				j++;
			}
			else
			{
				Addr = MakeSegmentPointer(0, gdt->AddressHigh, gdt->AddressMiddle, gdt->AddressLow);
				segmentDescriptorsReadableKernelBuffer->Address = Addr;
			}
			DbgPrintEx(DPFLTR_SYSTEM_ID, DPFLTR_ERROR_LEVEL, "[+] processor %d, Index %x\n", processorNumber, segmentDescriptorsReadableKernelBuffer->Index);
			DbgPrintEx(DPFLTR_SYSTEM_ID, DPFLTR_ERROR_LEVEL, "[+] processor %d, Limit %x\n", processorNumber, segmentDescriptorsReadableKernelBuffer->Limit);
			DbgPrintEx(DPFLTR_SYSTEM_ID, DPFLTR_ERROR_LEVEL, "[+] processor %d, Address %p\n", processorNumber, segmentDescriptorsReadableKernelBuffer->Address);
			DbgPrintEx(DPFLTR_SYSTEM_ID, DPFLTR_ERROR_LEVEL, "[+] processor %d, Accessed %x\n", processorNumber, segmentDescriptorsReadableKernelBuffer->Accessed);
			DbgPrintEx(DPFLTR_SYSTEM_ID, DPFLTR_ERROR_LEVEL, "[+] processor %d, ReadWrite %x\n", processorNumber, segmentDescriptorsReadableKernelBuffer->ReadWrite);
			DbgPrintEx(DPFLTR_SYSTEM_ID, DPFLTR_ERROR_LEVEL, "[+] processor %d, ConformingExpandDown %x\n", processorNumber, segmentDescriptorsReadableKernelBuffer->ConformingExpandDown);
			DbgPrintEx(DPFLTR_SYSTEM_ID, DPFLTR_ERROR_LEVEL, "[+] processor %d, CodeData %x\n", processorNumber, segmentDescriptorsReadableKernelBuffer->CodeData);
			DbgPrintEx(DPFLTR_SYSTEM_ID, DPFLTR_ERROR_LEVEL, "[+] processor %d, NonSystem %x\n", processorNumber, segmentDescriptorsReadableKernelBuffer->NonSystem);
			DbgPrintEx(DPFLTR_SYSTEM_ID, DPFLTR_ERROR_LEVEL, "[+] processor %d, DescriptorPrivilegeLevel %x\n", processorNumber, segmentDescriptorsReadableKernelBuffer->DescriptorPrivilegeLevel);
			DbgPrintEx(DPFLTR_SYSTEM_ID, DPFLTR_ERROR_LEVEL, "[+] processor %d, Present %x\n", processorNumber, segmentDescriptorsReadableKernelBuffer->Present);
			DbgPrintEx(DPFLTR_SYSTEM_ID, DPFLTR_ERROR_LEVEL, "[+] processor %d, AvailableAndReservedBit %x\n", processorNumber, segmentDescriptorsReadableKernelBuffer->AvailableAndReservedBit);
			DbgPrintEx(DPFLTR_SYSTEM_ID, DPFLTR_ERROR_LEVEL, "[+] processor %d, CodeSegment64Bit %x\n", processorNumber, segmentDescriptorsReadableKernelBuffer->CodeSegment64Bit);
			DbgPrintEx(DPFLTR_SYSTEM_ID, DPFLTR_ERROR_LEVEL, "[+] processor %d, DefaultOperationSizeAndDefaultStackPointerSizeAndUpperBound %x\n", processorNumber, segmentDescriptorsReadableKernelBuffer->DefaultOperationSizeAndDefaultStackPointerSizeAndUpperBound);
			DbgPrintEx(DPFLTR_SYSTEM_ID, DPFLTR_ERROR_LEVEL, "[+] processor %d, Granularity %x\n", processorNumber, segmentDescriptorsReadableKernelBuffer->Granularity);
		}
	}
	return STATUS_SUCCESS;
}

NTSTATUS QueryCurrentGDT(PSegmentDescriptors* gdt, UINT32* itemCount)
{
	NTSTATUS Status = STATUS_SUCCESS;
	GDTRegister gdtr = { 0 };

	PAGED_CODE();

	QueryCurrentGDTR(&gdtr);

	if (gdtr.GDTBase)
	{
		*gdt = gdtr.GDTBase;
		UINT32 count = 0;
		UINT64	overPoint = (UINT64)gdtr.GDTBase + gdtr.Limit + 1;
		DbgPrintEx(DPFLTR_SYSTEM_ID, DPFLTR_ERROR_LEVEL, "[+]overPoint %p\n", (PVOID)overPoint);
		for (UINT64 i = (UINT64)gdtr.GDTBase; i < overPoint; i += 8)
		{
			PSegmentDescriptors p = (PSegmentDescriptors)i;
			DbgPrintEx(DPFLTR_SYSTEM_ID, DPFLTR_ERROR_LEVEL, "[+]p %p\n", p);
			if (p->P)
			{
				if (!(p->S))
				{
					i += 8;
				}
			}
			count++;
		}
		*itemCount = count;
	}
	else
	{
		return STATUS_NOT_FOUND;
	}
	return Status;
}
PVOID MakeSegmentPointer(UINT32 High, UINT8 MiddleHigh, UINT8 MiddleLow, UINT16 Low)
{
	DbgPrintEx(DPFLTR_SYSTEM_ID, DPFLTR_ERROR_LEVEL, " High %x\n", High);
	DbgPrintEx(DPFLTR_SYSTEM_ID, DPFLTR_ERROR_LEVEL, " MiddleHigh %x\n", MiddleHigh);
	DbgPrintEx(DPFLTR_SYSTEM_ID, DPFLTR_ERROR_LEVEL, "MiddleLow %x\n", MiddleLow);
	DbgPrintEx(DPFLTR_SYSTEM_ID, DPFLTR_ERROR_LEVEL, "Low %x\n", Low);

	UINT64 tmp = 0;
	tmp = (((UINT64)High) << 32) | (((UINT64)MiddleHigh) << 24) | (((UINT64)MiddleLow) << 16) | Low;
	return (PVOID)tmp;
}
UINT32 MakeSegmentLimit(UINT8 High, UINT16 Low)
{
	UINT32 tmp = 0;
	tmp = (((UINT32)High) << 16) | Low;
	return tmp;
}