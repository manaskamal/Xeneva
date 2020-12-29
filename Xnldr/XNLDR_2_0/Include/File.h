/**
 *
 *  Copyright (C) Manas Kamal Choudhury 2020
 *
 *  File.h -- UEFI File Handling Library
 *
 *  /PROJECT - Xnldr v2.0
 *  /AUTHOR  - Manas Kamal Choudhury
 * ----------------------------------------------------
 */

//! file_h  define it here
#ifndef __FILE_H__
#define __FILE_H__

#include <Include/EfiLib.h>
#include <Include/Stdlib/Stdint.h>
#include <Uefi.h>

/**
 * Opens a file for read
 *
 * @param filename -- Filename with fill path
 *
 */
extern "C" uint8_t* OpenFile (CHAR16* filename);

//! END OF FILE.H
#endif