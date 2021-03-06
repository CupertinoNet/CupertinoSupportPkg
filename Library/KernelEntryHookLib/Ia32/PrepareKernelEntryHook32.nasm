;; @file
; Helper procedures to pass data to the XNU kernel hook procedures.
;
; Copyright (C) 2017, CupertinoNet.  All rights reserved.<BR>
; Portions Copyright (C) 2012 - 2014, Damir Ma�ar.  All rights reserved.<BR>
;
; Licensed under the Apache License, Version 2.0 (the "License");
; you may not use this file except in compliance with the License.
; You may obtain a copy of the License at
;
;     http://www.apache.org/licenses/LICENSE-2.0
;
; Unless required by applicable law or agreed to in writing, software
; distributed under the License is distributed on an "AS IS" BASIS,
; WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
; See the License for the specific language governing permissions and
; limitations under the License.
;
;;

extern ASM_PFX (KernelEntryHook32).ASM_PFX (KernelEntryHandlerAddress)
extern ASM_PFX (KernelEntryHandler32).ASM_PFX (KernelEntryAddress)
extern ASM_PFX (KernelEntryHandler32).ASM_PFX (KernelHookHandoffAddress)
extern ASM_PFX (KernelEntryHandler32).ASM_PFX (KernelEntryNotifyAddress)

    BITS    32
    CPU     386
    SECTION .text

;------------------------------------------------------------------------------
; VOID
; EFIAPI
; PrepareKernelEntryHook32 (
;   IN UINT32  KernelEntry,
;   IN UINT32  HandlerEntry,
;   IN UINT32  NotifyEntry,
;   IN UINT32  Context
;   );
;------------------------------------------------------------------------------
global ASM_PFX (PrepareKernelEntryHook32)
ASM_PFX (PrepareKernelEntryHook32):
    ; The caller eip is on the top of the stack: 4 bytes
    push    ebx                     ; 4 bytes
    push    edi                     ; 4 bytes
    ; Total size: 12 bytes

    mov     ebx, dword [esp + 12]   ; KernelEntry
    mov     ecx, dword [esp + 16]   ; HandlerEntry
    mov     edx, dword [esp + 20]   ; NotifyEntry
    mov     edi, dword [esp + 24]   ; Context

    ; Patch KernelEntryHandler32() with the address of the kernel entry point.
    mov     dword [ASM_PFX (KernelEntryHandler32).ASM_PFX (KernelEntryAddress)], ebx

    ; Patch KernelEntryHook32() with the handler address.
    mov     dword [ASM_PFX (KernelEntryHook32).ASM_PFX (KernelEntryHandlerAddress)], ecx

    ; Patch KernelEntryHandler32() with the notify entry point.
    mov     dword [ASM_PFX (KernelEntryHandler32).ASM_PFX (KernelEntryNotifyAddress)], edx

    ; Patch KernelEntryHandler32() with the context data address.
    mov     dword [ASM_PFX (KernelEntryHandler32).ASM_PFX (KernelHookHandoffAddress)], edi

    pop     edi
    pop     ebx

    retn
