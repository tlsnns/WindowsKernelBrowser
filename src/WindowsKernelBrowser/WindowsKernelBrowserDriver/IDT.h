#pragma once
#include <ntddk.h>

#pragma pack(1)
typedef struct _IDTRegister
{
	USHORT Limit;
	PVOID IDTBase;
}IDTRegister, * PIDTRegister;

#pragma warning(push)
#pragma warning(disable : 4214)
typedef struct _GateDescriptors
{
	USHORT OffsetLow;
	USHORT Selector;
	UCHAR IST : 3;
	UCHAR Reserved0 : 5;
	UCHAR TYPE : 4;
	UCHAR Reserved1 : 1;
	UCHAR DPL : 2;
	UCHAR Present : 1;
	USHORT OffsetMiddle;
	UINT32 OffsetHigh;
	UINT32 Reserved2;
}GateDescriptors, * PGateDescriptors;
#pragma warning(pop)

typedef struct _GateDescriptorsReadable
{
	USHORT InterruptServiceRoutineSegmentSelector;
	PVOID  InterruptServiceRoutineOffset;
	UCHAR InterruptStackTableIndex;
	UCHAR TYPE;
	UCHAR DescriptorPrivilegeLevel;
	BOOLEAN Present;
}GateDescriptorsReadable, * PGateDescriptorsReadable;
#pragma pack()


NTSTATUS QueryMultiProcessorIDT(PVOID SystemBuffer, ULONG InputBufferLength, ULONG OutputBufferLength, ULONG_PTR* Information);
NTSTATUS QueryCurrentIDT(PGateDescriptors* idt, ULONG* itemCount);
PVOID MakeISRPointer(UINT32 OffsetHigh, USHORT OffsetMiddle, USHORT OffsetLow);


#ifdef ALLOC_PRAGMA
#pragma alloc_text(PAGE, QueryMultiProcessorIDT)
#pragma alloc_text(PAGE, QueryCurrentIDT)
#pragma alloc_text(PAGE, MakeISRPointer)
#endif

