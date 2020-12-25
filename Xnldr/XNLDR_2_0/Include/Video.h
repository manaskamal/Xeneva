/**
 *
 *  Copyright (C) Manas Kamal Choudhury 2020
 *
 *  Video.h -- Video Mode Manager
 *
 *  /PROJECT - Xnldr v2.0
 *  /AUTHOR  - Manas Kamal Choudhury
 *
 * ------------------------------------------------------
 */

//! define video_h_ here
#ifndef __VIDEO_H__
#define __VIDEO_H__

#include <Include/Stdlib/Stdint.h>
#include <Uefi.h>

//! Video Mode Structure
typedef struct _Mode_Info_
{
	uint32_t Lfb;
	uint16_t XRes;
	uint16_t YRes;
	uint16_t BytesPerScanline;
	uint16_t BitsPerPixel;
	uint8_t  MemoryModel;
	uint16_t Red;
	uint16_t Blue;
	uint16_t Green;
	uint8_t  Resv[5];
}VideoMode;


#endif