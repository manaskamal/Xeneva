/**
 *
 *  Copyright (C) Manas Kamal Choudhury 2020
 *
 *  PSF2 Font -- PSF2 Font library
 *
 *  /PROJECT - Xnldr 2.0
 *  /AUTHOR  - Manas Kamal Choudhury
 *
 */

#ifndef __PSF2_FONT_H__
#define __PSF2_FONT_H__

#include <Include/Video.h>
#include <Include/Console.h>
#include <Include/Xeneva.h>
#include <Uefi.h>

#define PSF_FONT_MAGIC  0x864ab572

typedef struct _PSF_FONT_ 
{
	uint32_t Magic;
	uint32_t Version;
	uint32_t HeaderSize;
	uint32_t Flags;
	uint32_t NumGlyph;
	uint32_t BytesPerGlyph;
	uint32_t Height;
	uint32_t Width;
}PSFFONT;

extern "C" void PSFInit (CHAR16* filename);
extern "C" void PSFPutC (char c, int x, int y, uint32_t bg, uint32_t fg);
#endif