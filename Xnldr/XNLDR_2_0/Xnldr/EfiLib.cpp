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
#include <Include/Stdlib/Stdint.h>
#include <Protocol/LoadedImage.h>

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

	EFI_STATUS Status;
	EFI_GUID LoadedProtocol = EFI_LOADED_IMAGE_PROTOCOL_GUID;
	EFI_LOADED_IMAGE_PROTOCOL* LoadedImage = nullptr;
	if (Status = gBootServices->HandleProtocol(gImageHandle, &LoadedProtocol, (void**)&LoadedImage))
	{
		return Status;
	}
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
	
/**
 * Wait For Single Step Event
 *
 * @param event -- type of event
 * @param timeout -- timeout
 *
 */
EFI_STATUS  WaitForSingleEvent(EFI_EVENT event, uint32_t timeout)
{
	EFI_STATUS status;
	uint32_t  index;
	EFI_EVENT  timer_event;
	EFI_EVENT  wait_list[2];

	if (timeout)
	{
		status = gBootServices->CreateEvent(EVT_TIMER, 0, nullptr, nullptr, &timer_event);
		if (!EFI_ERROR(status))
		{
			gBootServices->SetTimer(timer_event, TimerRelative, timeout);

			wait_list[0] = event;
			wait_list[1] = timer_event;
			status = gBootServices->WaitForEvent(2, wait_list, (UINTN*)&index);
			gBootServices->CloseEvent(timer_event);

			//! if the timer expired, change the return to timed out
			if (!EFI_ERROR(status) && (index == 1))
				status = EFI_TIMEOUT;
		}
	} else
	{
		status = gBootServices->WaitForEvent(1, &event, (UINTN*)&index);
	}

	return status;
}


/**
 * Wait for a key stroke
 *
 * @param Key -- KeyCode that user recently pressed
 *
 */
EFI_STATUS GetKeyStroke(EFI_INPUT_KEY *Key)
{
	WaitForSingleEvent(gSystemTable->ConIn->WaitForKey, 0);
	return gSystemTable->ConIn->ReadKeyStroke(gSystemTable->ConIn, Key);
}