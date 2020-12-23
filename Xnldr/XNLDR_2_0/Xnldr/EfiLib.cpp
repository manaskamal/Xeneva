/**
 *
 *   Copyright (C) Manas Kamal Choudhury 2020
 *
 *   EfiLib.cpp -- UEFI Library Initialiser 
 *
 *  /PROJECT -- Xnldr v2.0
 *  /AUTHOR  -- Manas Kamal Choudhury
 *
 * -----------------------------------------------------
 */

#include <Include/EfiLib.h>

/**
 *  Initialise some global variables
 *  so that passed information can be
 *  used by entire Xnldr
 *
 */
EFI_HANDLE   gImageHandle;                                /*  Image Handler pointer          */
EFI_SYSTEM_TABLE  *gSystemTable = nullptr;                /*  System Table pointer           */
EFI_BOOT_SERVICES   *gBootServices = nullptr;             /*  Boot Services Table pointer    */
EFI_RUNTIME_SERVICES* gRuntimeServices = nullptr;         /*  Runtime Services Table pointer */


/*
 *  Initialises our entire UEFI Library
 *
 *  @param ImageHandle -- Image Handler pointer passed by UEFI Firmware
 *  @param SystemTable -- System Table pointer passed by UEFI Firmware
 *
 */

EFI_STATUS  InitializeEfiLib ( EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable)
{

	gImageHandle = ImageHandle;
	gSystemTable = SystemTable;
	gBootServices = SystemTable->BootServices;
	gRuntimeServices = SystemTable->RuntimeServices;

	//! return now
	return EFI_SUCCESS;
}

/**
 *  Returns the global Boot Service table pointer
 */
EFI_BOOT_SERVICES *GetBootServices ()
{
	return gBootServices;
}

/**
 * Returns the global Runtime Service table pointer
 */
EFI_RUNTIME_SERVICES *GetRuntimeServices ()
{
	return gRuntimeServices;
}

/**
 * Loop infinite in case to halt the processor
 */
void  Loop()
{
	for (;;);
}
	