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



	/* Go into infinite loop */
	Loop ();

	/* return the EFI_SUCCESS code */
	return EFI_SUCCESS;
}



