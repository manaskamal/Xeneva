;; --------------------------------------------------------------
;; 
;;   Copyright (C) Manas Kamal Choudhury 2020
;;
;;   SysX64.asm -- contains system low level source file
;;
;;   /PROJECT -- Xnldr v2.0
;;   /AUTHOR  -- Manas Kamal Choudhury
;;
;; ---------------------------------------------------------------


BITS 64              ;set the bit to 64 for the assembler
section .text        ; .text is the main section where every code resides

global MemoryBarrier
MemoryBarrier:
     mfence
	 ret

;; ===========================================================
;; GET & SET PAGING ROOT, in our case it is PML4 
;; ===========================================================
global GetPagingRoot
GetPagingRoot:
     mov rax, cr3
	 ret

global SetPagingRoot
SetPagingRoot:
     mov cr3, rcx
	 ret


global Pause
Pause:
     pause
	 ret

;;********************************************************
;; CALLS the KERNEL
;;
;;  /INPUT - RCX -- PARAM 1
;;           RDX -- Kernel Entry Point i.e .text section
;;           R8  -- Kernel Stack
;;           R9  -- Stack Size
;; -------------------------------------------------------
global CallKernel
CallKernel:
     add   r8, r9
	 sub   r8, 0x28
	 mov   rsp, r8
	 push  rbp
	 mov   rbp, rsp
	 sub   rsp, 32
	 call  rdx
	 jmp $
	 ret


;; =====================================================
;; CR0 Functions R/W
;; =====================================================

global ReadCR0
ReadCR0:
       mov rax, cr0
	   ret

global WriteCR0
WriteCR0
	   mov cr0, rcx
	   ret

;; ====================================================
;; TLB Flush -- Translation Lookaside Buffer 
;; ====================================================
global TLBFlush
TLBFlush:
       invlpg [rcx]
	   ret

