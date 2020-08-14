#include "Pool.h"

NTSTATUS AllocNonPagedPool()
{
	PVOID KernelBuffer = NULL;
	NTSTATUS Status = STATUS_SUCCESS;

	PAGED_CODE();

	__try
	{
		DbgPrint("[+] Allocating Pool chunk\n");
		for (int i = 0; i < 10000; i++)
		{
			KernelBuffer = ExAllocatePoolWithTag(NonPagedPool, (SIZE_T)2, (ULONG)'hack');
			if (!KernelBuffer)
			{
				DbgPrint("[-] Unable to allocate Pool chunk\n");

				Status = STATUS_NO_MEMORY;
				return Status;
			}
			RtlFillMemory(KernelBuffer, 2, 'A');
		}
		DbgPrint("[+] Allocating Pool chunk 2\n");
		KernelBuffer = ExAllocatePoolWithTag(NonPagedPool, (SIZE_T)2, (ULONG)'9999');
		RtlFillMemory(KernelBuffer, 2, 'A');
		ExFreePoolWithTag(KernelBuffer, (ULONG)'9999');
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		Status = GetExceptionCode();
		DbgPrint("[-] Exception Code: 0x%X\n", Status);
	}

	return Status;
}
