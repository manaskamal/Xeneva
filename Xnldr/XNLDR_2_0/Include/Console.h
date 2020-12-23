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

//! define CONSOLE_H here

#ifndef __CONSOLE_H__
#define __CONSOLE_H__

#include <Uefi.h>
#include <Include/Stdlib/String.h>
#include <Include/Stdlib/CType.h>
#include <Include/Stdlib/Stdarg.h>

//! FUNCTIONS

extern "C" void  ClearScreen ();
extern "C" EFI_STATUS EnableCursor (bool on);
/**extern "C" EFI_STATUS GetCurrentCursorPos (int *Col, int *Row);
extern "C" EFI_STATUS SetCurrentCursorPos (int Col, int Row);
extern "C" int  Printf (const char* fmt, ...);**/

#endif
/**
 * End Of Console.h
 */