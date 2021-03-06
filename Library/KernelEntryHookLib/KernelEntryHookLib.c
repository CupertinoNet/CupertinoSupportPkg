/** @file
  Copyright (C) 2017, CupertinoNet.  All rights reserved.<BR>
  Portions Copyright (C) 2012 - 2014, Damir Ma�ar.  All rights reserved.<BR>

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

      http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

**/

#include <Uefi.h>

#include <XnuHibernate.h>

#include <Library/BaseMemoryLib.h>
#include <Library/CupertinoXnuLib.h>
#include <Library/DebugLib.h>
#include <Library/KernelEntryNotifyLib.h>
#include <Library/KernelEntryHookLib.h>
#include <Library/KernelEntryNotifyImageLib.h>
#include <Library/XnuSupportMemoryAllocationLib.h>

#include "KernelEntryHookInternal.h"

VOID
KernelEntryHook (
  IN OUT UINTN  KernelEntryAddress
  )
{
  //
  // XNU Kernel transition always happens in protected mode.  This is an
  // assertion made by this function.
  //

  UINTN                     NotifyEntry;
  UINTN                     MemorySize;
  KERNEL_ENTRY_HOOK_CONTEXT *Context;
  VOID                      *HandlerEntry;

  ASSERT (KernelEntryAddress != 0);
  ASSERT (KERNEL_ENTRY_HOOK_32_SIZE > 0);
  ASSERT (KERNEL_ENTRY_HANDLER_32_SIZE > 0);

  NotifyEntry = LoadKernelEntryNotifyImage (
                  gKernelEntryNotifyImageData
                  );

  if (NotifyEntry != 0) {
    MemorySize = (sizeof (*Context) + KERNEL_ENTRY_HOOK_32_SIZE);

    Context = (KERNEL_ENTRY_HOOK_CONTEXT *)AllocateXnuSupportData (MemorySize);

    if (Context != NULL) {
      HandlerEntry = AllocateXnuSupportCode (KERNEL_ENTRY_HANDLER_32_SIZE);

      if (HandlerEntry != NULL) {
        Context->Hdr.Signature       = KERNEL_ENTRY_HOOK_CONTEXT_SIGNATURE;
        Context->Hdr.KernelEntry     = (UINT32)KernelEntryAddress;
        Context->Hdr.KernelEntrySize = (UINT32)KERNEL_ENTRY_HOOK_32_SIZE;

        PrepareKernelEntryHook32 (
          Context->Hdr.KernelEntry,
          (UINT32)(UINTN)HandlerEntry,
          (UINT32)NotifyEntry,
          (UINT32)(UINTN)Context
          );

        CopyMem (
          (VOID *)&Context->KernelEntryBackup[0],
          (VOID *)(UINTN)Context->Hdr.KernelEntry,
          (UINTN)Context->Hdr.KernelEntrySize
          );

        CopyMem (
          (VOID *)(UINTN)Context->Hdr.KernelEntry,
          (VOID *)(UINTN)KernelEntryHook32,
          (UINTN)Context->Hdr.KernelEntrySize
          );
  
        CopyMem (
          HandlerEntry,
          (VOID *)(UINTN)KernelEntryHandler32,
          KERNEL_ENTRY_HANDLER_32_SIZE
          );
      } else {
        FreeXnuSupportMemory (
          (VOID *)Context,
          MemorySize
          );
      }
    }
  }
}

VOID
KernelHookPatchEntry (
  IN OUT VOID   *KernelHeader,
  IN     UINTN  KernelSlide
  )
{
  UINTN KernelEntry;

  ASSERT (KernelHeader != NULL);

  KernelEntry = XnuGetEntryAddress (KernelHeader);

  ASSERT (KernelEntry != 0);

  if (KernelEntry != 0) {
    KernelEntryHook (KernelEntry + KernelSlide);
  }
}

VOID
KernelHookPatchHibernationEntry (
  IN IO_HIBERNATE_IMAGE_HEADER  *ImageHeader
  )
{
  ASSERT (ImageHeader != NULL);
  ASSERT (ImageHeader->Signature == IO_HIBERNATE_HEADER_SIGNATURE);

  KernelEntryHook (
    IO_HIBERNATE_ENTRY_POINT (ImageHeader)
    );
}
