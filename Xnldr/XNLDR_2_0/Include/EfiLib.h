/**
 *
 *   Copyright (C) Manas Kamal Choudhury 2020
 *
 *   EfiLib.h -- UEFI Library Initialiser which initialise 
 *               the UEFI library with given parameters
 *
 *   /PROJECT - Xnldr v2.0
 *   /AUTHOR  - Manas Kamal Choudhury
 *
 * ------------------------------------------------------------
 */


//! define EFI_LIB Here

#ifndef __EFI_LIB_H__
#define __EFI_LIB_H__

#include <Uefi.h>


/**
 * First make it available for
 * whole Xnldr to access some 
 * variables initialised by
 * EfiLib manager
 * --------------------------------------------
 */

extern EFI_HANDLE  gImageHandle;                  /* The Image Handler pointer */
extern EFI_SYSTEM_TABLE *gSystemTable;            /* The System Table pointer */
extern EFI_BOOT_SERVICES *gBootServices;          /* The Boot Services table pointer    */
extern EFI_RUNTIME_SERVICES *gRuntimeServices;    /* The Runtime Services table pointer */


//! FUNCTIONS -----

EFI_STATUS              InitializeEfiLib (EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *gSystemTable);
EFI_BOOT_SERVICES      *GetBootServices ();
EFI_RUNTIME_SERVICES*   GetRuntimeServices ();
void                    Loop ();


#endif

// ==============================
// End Of our EfiLib.h
// ==============================