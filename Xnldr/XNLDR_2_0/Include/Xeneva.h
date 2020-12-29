/**
 *
 *  Copyright (C) Manas Kamal Choudhury 2020
 *
 *  Xeneva.h -- the main Kernel header...
 * 
 *  /PROJECT -- Xnldr v2.0
 *  /AUTHOR  -- Manas Kamal Choudhury
 *
 * ------------------------------------------------------
 */

#ifndef __XENEVA_H__
#define __XENEVA_H__

#include <Include/Stdlib/Stdint.h>

/**
 *  
 *  Xnldr is mainly prepared for Xeneva kernel
 *  environment. Xnldr contains its own
 *  kernel boot information so that the kernel
 *  get its required informations
 */

#define XNKRNL_MAGIC   'XNKRNL'

//! Xnldr will set it. 
#define BOOT_FROM_UEFI   0x1
#define BOOT_FROM_BIOS   0x2

typedef struct __BOOT_INFO__
{
	/**
	 * The magic always contains XNKRNL
	 * if it is not XNRKNL, Xnldr is compiled
	 * for any other kernel with specific name
	 */
	char Magic[6];  

	/**
	 * Firmware info, it is boot from, 
	 * Xnldr will set it automatically
	 */

    uint8_t Firmware;  

	/**
	 * Memorry Information, Xnldr will collect available
	 * Free physical memory and store it here as an array
	 */
	struct _FreeMemory_
	{
		/* Physical start of memory, i.e address */
		size_t PhysicalStart;

		/* Number of pages it contains, i.e size */
		uint64_t Size;

		/* Type of memory, i.e EfiConventionalMemory or ? */
		size_t Type;

		/* Attribute */
		size_t Attribute;

		/* Virtual Start, for every Physical Memory there
		   is a virtual memory mapped */
		size_t VirtualStart;

	}FreeMemory[32];

	/**
	 * Video information, Xnldr will choose the best 
	 * screen resolution for user and store it here
	 * NOTE THAT : For smooth performance, Xnldr will
	 * automatically for Xeneva kernel
	 */
	struct _Video_
	{
		/**
		 * Width of the Screen
		 */
		size_t Width;

		/**
		 * Height of the Screen
		 */
		size_t Height;

		/**
		 * Bit Per Pixel of the Screen
		 */
		size_t BitPerPixel;

		/**
		 * Pixels Per Line 
		 */
		size_t PixelsPerLine;

		/**
		 * Physical Address of Video Screen
		 */
		size_t PhysicalAddress;

		/**
	     * Color Properties
	     */
	    uint32_t Red;
	    uint32_t Blue;
	    uint32_t Green;
	    uint32_t  Resv;
	} Video;

	void (*ConsoleOut)(const char *text);

	/**
	 * ACPI RSDP Address, from EfiLib
	 */
	void* ACPIXsdp;

	/**
	 * Virtual Memory PML4 Address
	 * Xnldr use 64 Bit Virtual Memory or 
	 * PAE enable in x86 systems
	 */
	void* PML4;

	/**
	 * At last bootloader name, for Xeneva 
	 * it is 'XNLOADER'
	 */
	char Bootloader_Name[8];
}XENEVA_INFO;

/**
 * Return the Xeneva Boot Information Structure
 */
extern "C" XENEVA_INFO* GetXenevaInfo ();

/**
 * Kernel Entry point extraction
 *
 * @param Info -- Xeneva Boot Information Structure to pass to
 */
typedef void(*KernelEntry)(XENEVA_INFO *Info);

/** Define Some RAW function @Reference XNLDR 1.0 **/
#ifdef __cplusplus
#define EXTERN extern "C"
#else
#define EXTERN
#endif
/* Template class */
#ifdef __cplusplus
extern "C++" {
	template <class T, class U> intptr_t raw_diff(T* p1, U* p2)
	{
		return (intptr_t)p1 - (intptr_t)p2;
	}
	template <class T, class U> T raw_offset(U p1, const intptr_t offset)
	{
		return (T) ((size_t)p1 + offset);
	}
	template <class T, class U> T mem_after(U* p1)
	{
		return (T) (&p1[1]);
	}
}
#else
#define RAW_OFFSET(type, x, offset)  (type)((size_t)x + offset)
#endif
// ----------------------------------------------------------------------

#define SIZE_MAX UINTPTR_MAX
/**
 * Get Paging Root i.e PML4
 */
extern "C" void* GetPagingRoot ();

/**
 * Memory Barrier
 */
extern "C" void MemoryBarrier ();

/**
 * SetPagingRoot i.e PML4
 *
 * @param Root -- PML4 address
 */
extern "C" void SetPagingRoot (void* Root);

/**
 * Call the Kernel
 *
 * @param Param -- Boot Information Structure
 * @param Entry -- Kernel Entry Point
 * @param Stack -- Stack address
 * @param StackSize -- Size of the Stack for the Kernel
 */
extern "C" void CallKernel (void* Param, void *Entry, void* Stack, size_t StackSize);

/**
 * Read the value from CR0 Register
 */
extern "C" size_t ReadCR0();

/**
 * Write the value to CR0 Register
 *
 * @param Value -- Value to write
 *
 */
extern "C" void   WriteCR0(size_t Value);

/**
 * Write the new page to TLB, so that we don't miss the page
 * next time we need it. else Page Fault
 *
 * @param Addr -- Address of the Page
 */
extern "C" void TLBFlush(void* Addr);

#endif