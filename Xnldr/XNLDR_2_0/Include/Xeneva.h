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
	} Video;

	/**
	 * ACPI RSDP Address, from EfiLib
	 */
	size_t ACPIRsdp;

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



extern "C" XENEVA_INFO* GetXenevaInfo ();
#endif