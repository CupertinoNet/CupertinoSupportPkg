/** @file
  Copyright (C) 2012 - 2014 Damir Ma�ar.  All rights reserved.<BR>
  Portions Copyright (C) 2015 - 2017, CupertinoNet.  All rights reserved.<BR>

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions
  are met:

  1. Redistributions of source code must retain the above copyright
  notice, this list of conditions and the following disclaimer.
  2. Redistributions in binary form must reproduce the above copyright
  notice, this list of conditions and the following disclaimer in
  the documentation and/or other materials provided with the
  distribution.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
  FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE
  COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
  POSSIBILITY OF SUCH DAMAGE.
**/

#include <Uefi.h>

#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/MiscMemoryLib.h>
#include <Library/VirtualMemoryLib.h>

#include "VirtualMemoryInternal.h"

// VM_MEMORY_POOL_SIZE
#define VM_MEMORY_POOL_SIZE  SIZE_2MB

// mVmMemoryBottom
STATIC VOID *mVmMemory = NULL;

// mVmMemoryAvailablePages
STATIC UINTN mVmMemoryPages = 0;

// mVmMemoryBottom
STATIC UINTN mVmMemoryBottom = 0;

// mVmMemoryAvailablePages
STATIC UINTN mVmMemoryAvailablePages = 0;

// VirtualMemoryConstructor
BOOLEAN
VirtualMemoryConstructor (
  VOID
  )
{
  BOOLEAN Result;

  Result = FALSE;

  if (mVmMemoryBottom == 0) {
    mVmMemoryAvailablePages = EFI_SIZE_TO_PAGES (VM_MEMORY_POOL_SIZE);

    mVmMemory = AllocatePagesFromTop (
                  EfiBootServicesData,
                  mVmMemoryAvailablePages,
                  BASE_4GB
                  );

    mVmMemoryBottom = (UINTN)mVmMemory;
  }

  Result = (BOOLEAN)(mVmMemoryBottom != 0);

  return Result;
}

// VirtualMemoryConstructor
VOID
VirtualMemoryDestructor (
  VOID
  )
{
  if ((mVmMemory != NULL) && (mVmMemoryPages == mVmMemoryAvailablePages)) {
    FreePages (mVmMemory, mVmMemoryPages);

    mVmMemory = NULL;

    DEBUG_CODE (
      mVmMemoryPages          = 0;
      mVmMemoryBottom         = 0;
      mVmMemoryAvailablePages = 0;
      );
  }
}

// VmInternalAllocatePages
VOID *
VmInternalAllocatePages (
  IN UINTN  NumberOfPages
  )
{
  VOID *Memory;

  ASSERT (mVmMemoryAvailablePages >= NumberOfPages);

  Memory = NULL;

  if (mVmMemoryAvailablePages >= NumberOfPages) {
    Memory = (VOID *)mVmMemoryBottom;

    mVmMemoryBottom         += EFI_PAGES_TO_SIZE (NumberOfPages);
    mVmMemoryAvailablePages -= NumberOfPages;
  }

  return Memory;
}

// VirtualMemoryMapVirtualPages
BOOLEAN
VirtualMemoryMapVirtualPages (
  IN VOID                  *PageTable,
  IN EFI_VIRTUAL_ADDRESS   VirtualAddress,
  IN UINT64                NumberOfPages,
  IN EFI_PHYSICAL_ADDRESS  PhysicalAddress
  )
{
  BOOLEAN Result;

  Result = TRUE;

  while ((NumberOfPages > 0) && Result) {
    Result = VmInternalMapVirtualPage (PageTable, VirtualAddress, PhysicalAddress);

    VirtualAddress  += EFI_PAGE_SIZE;
    PhysicalAddress += EFI_PAGE_SIZE;

    --NumberOfPages;
  }

  return Result;
}
