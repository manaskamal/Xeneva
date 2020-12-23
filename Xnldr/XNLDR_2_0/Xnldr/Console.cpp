/**
 *   
 *   Copyright (C) Manas Kamal Choudhury 2020
 *
 *
 *   /PROJECT - Xnldr v2.0
 *   /AUTHOR  - Manas Kamal Choudhury
 *
 * ------------------------------------------------
 */


#include <Include/Console.h>
#include <Include/EfiLib.h>

/**
 * Clear the current screen
 */
void ClearScreen ()
{
	gSystemTable->ConOut->ClearScreen (gSystemTable->ConOut);
}

/**
 * Enables the text cursor
 *
 * @param enable -- enable cursor?
 */
EFI_STATUS EnableCursor (bool enable)
{
	return gSystemTable->ConOut->EnableCursor (gSystemTable->ConOut, enable);
}


/////// WILL BE IMPLEMENTED////////