/** @file
  References:
    1) IA-32 Intel(R) Architecture Software Developer's Manual Volume 1:Basic Architecture, Intel
    2) IA-32 Intel(R) Architecture Software Developer's Manual Volume 2:Instruction Set Reference, Intel
    3) IA-32 Intel(R) Architecture Software Developer's Manual Volume 3:System Programmer's Guide, Intel
    4) AMD64 Architecture Programmer's Manual Volume 2: System Programming

  Copyright (c) 2006 - 2011, Intel Corporation. All rights reserved.
  Copyright (C) 2012, Damir Ma�ar.  All rights reserved.
  Portions Copyright (C) 2015 - 2017, CupertinoNet.  All rights reserved.

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

#ifndef VIRTUAL_MEMORY_LIB_H_
#define VIRTUAL_MEMORY_LIB_H_

// VirtualMemoryConstructor
BOOLEAN
VirtualMemoryConstructor (
  VOID
  );

// VirtualMemoryDestructor
VOID
VirtualMemoryDestructor (
  VOID
  );

// VirtualMemoryGetPageTable
VOID *
VirtualMemoryGetPageTable (
  IN OUT UINTN  *Flags OPTIONAL
  );

// VirtualMemoryGetPhysicalAddress
EFI_PHYSICAL_ADDRESS
VirtualMemoryGetPhysicalAddress (
  IN VOID                 *PageTable,
  IN EFI_VIRTUAL_ADDRESS  VirtualAddress
  );

// VirtualMemoryMapVirtualPages
BOOLEAN
VirtualMemoryMapVirtualPages (
  IN VOID                  *PageTable,
  IN EFI_VIRTUAL_ADDRESS   VirtualAddress,
  IN UINT64                NumberOfPages,
  IN EFI_PHYSICAL_ADDRESS  PhysicalAddress
  );

// VirtualMemoryFlashCaches
VOID
VirtualMemoryFlashCaches (
  VOID
  );

#endif // VIRTUAL_MEMORY_LIB_H_
