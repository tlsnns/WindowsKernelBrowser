#pragma once
#include <ntddk.h>

#pragma pack(1)
typedef struct _GDTRegister
{
	USHORT Limit;
	PVOID GDTBase;
}GDTRegister, * PGDTRegister;
#pragma warning(push)
#pragma warning(disable : 4214)
typedef struct _SegmentDescriptors
{
	UINT16 LimitLow;
	UINT16 AddressLow;
	UINT8 AddressMiddle;

	UINT8 A : 1;
	UINT8 RW : 1;
	UINT8 CE : 1;
	UINT8 CD : 1;
	UINT8 S : 1;
	UINT8 DPL : 2;
	UINT8 P : 1;

	UINT8 LimitHigh : 4;
	UINT8 AVL : 1;
	UINT8 L : 1;
	UINT8 DB : 1;
	UINT8 G : 1;

	UINT8 AddressHigh;
}SegmentDescriptors, * PSegmentDescriptors;
#pragma warning(pop)
typedef struct _SystemSegmentDescriptors
{
	UINT32 Address;
	UINT32 Reserved;
}SystemSegmentDescriptors, * PSystemSegmentDescriptors;
typedef struct _SegmentDescriptorsReadable
{
	UINT8 Index;//0-1

	UINT32  Limit;//1-4
	PVOID Address;//5-8

	BOOLEAN Accessed;//13-1
	BOOLEAN ReadWrite;//14-1
	BOOLEAN ConformingExpandDown;//15-1
	BOOLEAN CodeData;//16-1
	BOOLEAN NonSystem;//17-1
	UINT8  DescriptorPrivilegeLevel;//18-1
	BOOLEAN Present;//19-1

	UINT8 AvailableAndReservedBit;//20-1
	BOOLEAN CodeSegment64Bit;//21-1
	BOOLEAN DefaultOperationSizeAndDefaultStackPointerSizeAndUpperBound;//22-1
	BOOLEAN Granularity;//23-1
}SegmentDescriptorsReadable, * PSegmentDescriptorsReadable;
#pragma pack()

NTSTATUS QueryMultiProcessorGDT(PVOID SystemBuffer, ULONG InputBufferLength, ULONG OutputBufferLength, ULONG_PTR* Information);
NTSTATUS QueryCurrentGDT(PSegmentDescriptors* gdt, UINT32* itemCount);
PVOID MakeSegmentPointer(UINT32 High, UINT8 MiddleHigh, UINT8 MiddleLow, UINT16 Low);
UINT32 MakeSegmentLimit(UINT8 High, UINT16 Low);

#ifdef ALLOC_PRAGMA
#pragma alloc_text(PAGE, QueryMultiProcessorGDT)
#pragma alloc_text(PAGE, QueryCurrentGDT)
#pragma alloc_text(PAGE, MakeSegmentPointer)
#pragma alloc_text(PAGE, MakeSegmentLimit)
#endif

