/**
 *  Copyright (C) Manas Kamal Choudhury 2020
 *
 *  Memory.h -- Physical Memory arrangement library
 *
 *  /PROJECT - Xnldr v2.0
 *  /AUTHOR  - Manas Kamal Choudhury
 *
 * ----------------------------------------------------------
 */

#ifndef __MEMORY_H__
#define __MEMORY_H__

/**
 * Memory manager simply checks the available physical memory
 * for use and then setup virtual memory and provides the access
 * to memory functions
 */

#include  <Include/Stdlib/String.h>
#include  <Include/Stdlib/Stdint.h>
#include  <Uefi.h>
#include  <Include/Xeneva.h>


/**
 * Check available memories for use
 * and return to Xeneva memory block
 *
 * @param info -- Xeneva info return to
 * @param mem_map_key -- Memory map key for EFILib
 *
 */
extern "C" EFI_STATUS  GetMemory ();

/**
 * Allocate memory from pool
 * 
 * @param MemSize -- Size of buffer to be allocated
 */
extern "C" void *AllocatePool (const size_t MemSize);

/**
 * Free Memory from pool
 *
 * @param buffer -- Address of the buffer to deallocate
 */
extern "C" void FreePool (void *buffer);

#endif