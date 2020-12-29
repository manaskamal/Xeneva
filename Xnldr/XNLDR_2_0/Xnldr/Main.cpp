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
#include <Include/Video.h>
#include <Include/PSF2Font.h>
#include <Include/TerminalPSF.h>
#include <Include/VirtualMemory.h>
#include <Include/Pe.h>



/* Xeneva boot info structure */

static XENEVA_INFO *info;

/* ACPI_20_TABLE_GUID 2.0 GUID */
#define ACPI_20_TABLE_GUID  {0x8868e871, 0xe4f1, 0x11d3, 0xbc, 0x22, 0x00, 0x80, 0xc7, 0x3c, 0x88, 0x81}

/**
 * Return the Xeneva Info 
 */
XENEVA_INFO* GetXenevaInfo ()
{
	return info;
}


/** 
 * Match GUID and return the result
 */
bool GUIDMatch(EFI_GUID guid1, EFI_GUID guid2) {
	bool first_part_good = (guid1.Data1 == guid2.Data1 && guid1.Data2 == guid2.Data2 &&
		guid1.Data3 == guid2.Data3);

	if (!first_part_good) return false;

	for (int i = 0; i < 8; ++i) {
		if (guid1.Data4[i] != guid2.Data4[i]) return false;
	}

	return true;
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
	EFI_STATUS Status;
	/* Initialize our UEFI Library */
	InitializeEfiLib (ImageHandle, SystemTable);

	/* Copy the Magic code */
	info->Magic[0] = 'X';
	info->Magic[1] = 'N';
	info->Magic[2] = 'K';
	info->Magic[3] = 'R';
	info->Magic[4] = 'N';
	info->Magic[5] = 'L';

	/*  0x1 is UEFI, 0x2 is BIOS */
	info->Firmware = BOOT_FROM_UEFI;  

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

	/**
	 * Initialize our video subsystem
	 */
	InitializeVideo();



	PSFInit ((CHAR16*)L"\\EFI\\XENEVA\\ConsoleFont.psf");
	
	TerminalPutString_PSF("Xnldr v2.0", 0,0);
	DrawText("\n\nVersion : 2.0 \n");
	DrawText("Initializing..\n");

	/**
	 * store our DrawText function to Xeneva boot information
	 */
	info->ConsoleOut = DrawText;


	/**
	 * ----------------------------------------------------------
	 * Copy ACPI XSDP Address to Boot Info Structure
	 * ----------------------------------------------------------
	 */
	EFI_CONFIGURATION_TABLE *ConfigTable = gSystemTable->ConfigurationTable;
	static EFI_GUID acpi_guid = ACPI_20_TABLE_GUID;
	for (unsigned i = 0; i < gSystemTable->NumberOfTableEntries; ++i){
		if (GUIDMatch(acpi_guid, ConfigTable[i].VendorGuid)){
			info->ACPIXsdp = ConfigTable[i].VendorTable;
		}
	}
	
	
	
	/* Paging Initialize */
	PagingInitialize ();
	

	DrawText("Paging Initialized\n");

	/**
	 * Load the Binary
	 */
	LoadExec ((CHAR16*)L"\\EFI\\XENEVA\\xnkrnl.exe");
	size_t StackSize = GetStackSize();
	KernelEntry EntryPoint = GetEntryPoint ();
	void* StackAddr = GetStackAddress ();

	/* Copy bootloader name */
	info->Bootloader_Name[0] = 'X';
	info->Bootloader_Name[1] = 'N';
	info->Bootloader_Name[2] = 'L';
	info->Bootloader_Name[3] = 'O';
	info->Bootloader_Name[4] = 'A';
	info->Bootloader_Name[5] = 'D';
	info->Bootloader_Name[6] = 'E';
	info->Bootloader_Name[7] = 'R';

	Status = gBootServices->ExitBootServices (ImageHandle,GetMapKey());
	if (Status != EFI_SUCCESS)
		DrawText("Exit Boot Service Failed\n");


	/**
	 * Call The Kernel
	 */
	CallKernel (info, EntryPoint, StackAddr, StackSize);
	/* Go into infinite loop */
	for(;;);

	/* return the EFI_SUCCESS code */
	return EFI_SUCCESS;
}



