/**
 *
 *  Copyright (C) Manas Kamal Choudhury 2020
 *
 *  VirtualMemory.h -- Virtual Memory Manager for Xeneva and Xnldr
 *
 *  /PROJECT - Xnldr v2.0
 *  /AUTHOR  - Manas Kamal Choudhury
 *
 * ------------------------------------------------------------------
 */

/** Virtual Memory header **/

#ifndef __VIRTUAL_MEMORY_H__
#define __VIRTUAL_MEMORY_H__

#include <Include/Stdlib/Stdint.h>
#include <Include/Stdlib/String.h>
#include <Uefi.h>

/* Physical addr type definition */
typedef EFI_PHYSICAL_ADDRESS Paddr_t;

/** Derived from Xnldr v1.0, paging attribute **/
#define PAGE_ATTRIBUTE_WRITABLE     0x2
#define PAGE_ATTRIBUTE_NO_EXEC      0x80000
#define PAGE_ATTRIBUTE_NO_PAGING    0x100000
#define PAGE_ATTRIBUTE_NO_CACHING   0x200000

#define PADDR_T_MAX  UINT64_MAX

/**
 * Initialize paging system 
 */
extern "C" void PagingInitialize ();

/**
 * Map a physical page to virtual memory
 *
 * @param VirtualAddr -- 64 bit Virtual address input
 * @param PhysicalAddress -- Physical Memory address
 * @param Length -- Length , i.e count of address to map to virtual memory
 * @param Attrib -- Attributes of Newly mapped Page
 */
extern "C" bool MapPage (void* VirtualAddr, Paddr_t PhysicalAddress, size_t Length, size_t Attrib);

extern "C" bool CheckFree (void* VirtualAddr, size_t Length);
extern "C" void SetAttributes (void* VirtualAddr, size_t Length, size_t Attr, size_t AttrClear);

#endif