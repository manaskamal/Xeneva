/**
 *
 *   Copyright (C) Manas Kamal Choudhury 2020
 *
 *   Terminal.h -- Main Terminal for Entire Kernel Output
 *                 and XNLDR Output
 *
 *  /PROJECT - Xnldr v2.0
 *  /AUTHOR  - Manas Kamal Choudhury
 *
 * --------------------------------------------------------------
 */

#ifndef __TERMINAL_H__
#define __TERMINAL_H__

#include <Include/Console.h>
#include <Include/PSF2Font.h>
#include <Include/Video.h>
#include <Include/Xeneva.h>
#include <Include/Stdlib/Stdint.h>
#include <Include/Stdlib/String.h>
#include <Include/Memory.h>

/**
 * Print a string in PSF Font format
 *
 * @param message -- Message to print
 * @param POS_X   -- X Value for the screen
 * @param POS_Y   -- Y Value for the screen
 */
extern "C" void TerminalPutString_PSF (const char* message, int POS_X, int POS_Y);

#endif