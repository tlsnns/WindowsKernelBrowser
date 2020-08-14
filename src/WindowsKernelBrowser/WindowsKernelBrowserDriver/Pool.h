#pragma once

#include <ntddk.h>

//
// Function Definitions
//

NTSTATUS AllocNonPagedPool();

//
#ifdef ALLOC_PRAGMA
#pragma alloc_text(PAGE, AllocNonPagedPool)
#endif
