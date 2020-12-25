/**
 *
 *   Copyright (C) Manas Kamal Choudhury 2020
 *
 *   Main.cpp -- Contains Main Entry Point of Xnldr v2.0
 *
 *   /PROJECT - Xnldr v2.0
 *   /AUTHOR  - Manas Kamal Choudhury
 *
 *  ------------------------------------------------------------
 */

#include <Include/Stdlib/Stdint.h>
#include <Include/Uefi/Uefi.h>
#include <Include/EfiLib.h>
#include <Include/Console.h>
#include <Include/Xeneva.h>
#include <Include/Memory.h>

/* Xeneva boot info structure */

static XENEVA_INFO *info;


/**
 * Return the Xeneva Info 
 */
XENEVA_INFO* GetXenevaInfo ()
{
	return info;
}

/**
 * XnMain -- Main Entry point of Xnldr v2.0
 *
 * @param ImageHandle -- Handle Address passed by UEFI Firmware
 * @param SystemTable -- UEFI System Table passed by UEFI Firmware
 *
 * @return CODE
 */

EFI_STATUS  XnMain (EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable)
{

	/* Initialize our UEFI Library */
	InitializeEfiLib (ImageHandle, SystemTable);

	/* 
	   Clear the screen and enable the cursor
	   for now there is no print function 
	   */
	ClearScreen ();
	EnableCursor (true);

	SetTextColor (EFI_BACKGROUND_BLACK, EFI_LIGHTGRAY);
	Printf (L"Xnldr v2.0 \r\n");
	Printf (L"Version 2.0 \r\n");

	/* Print a message for user to wait */
	Printf (L"Please Wait... \r\n");
   
	//! Scan physical memory for use
	GetMemory ();
	
	/* Go into infinite loop */
	Loop ();

	/* return the EFI_SUCCESS code */
	return EFI_SUCCESS;
}



