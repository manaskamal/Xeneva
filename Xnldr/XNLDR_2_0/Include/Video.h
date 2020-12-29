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
	/* 
	 * Linear Frame Buffer Address
	 */
	size_t Lfb; 

	/*
	 * Width of the Screen
	 */
	size_t XRes;

	/*
	 * Height of the Screen
	 */
	size_t YRes;

	/**
	 * Bytes Per Scanline
	 */
	size_t PixelsPerLine;

	/**
	 * Bytes Per Scanline
	 */
	size_t BytesPerScanline;

	/**
	 * Bits Per Pixel
	 */
	size_t BitsPerPixel;

	/**
	 * Video Memory Model
	 */
	uint8_t  MemoryModel;

	/**
	 * Color Properties
	 */
	uint32_t Red;
	uint32_t Blue;
	uint32_t Green;
	uint32_t Resv;
}VideoMode;


typedef struct {
	unsigned char data[16];
} bx_fontcharbitmap_t;

#define MAX_VIDEO_MODES  32  /** 32 Maximum Video Modes **/

/*
 * COLOR RGB Model
 */

#define RGB(r, g, b) \
	         ((r & 0xFF) | ((g << 8)&0xFF00) | ((b << 16)&0xFF0000))

#define RED(col)\
	         (col & 0xFF)

#define GREEN(col)\
	((col>>8) & 0xFF)

#define BLUE(col) \
	((col>>16) & 0xFF)

/**
 * Find a specific video mode with given 
 * Screen resolution and bit depth
 *
 * @param ModeInfo -- to store the result informations
 * @param Count  -- Bit Depth of the screen
 * @param XRes -- Width of the Screen
 * @param YRes -- Height of the Screen
 *
 */
extern "C" uint16_t  FindVideoMode(VideoMode *ModeInfo, uint16_t Count, int XRes, int YRes);

/**
 * Set a specific Video mode with configured screen
 * width and height
 *
 * @param ModeNum -- Mode Number for that resolution
 *
 */
extern "C" EFI_STATUS SetVideoMode(uint32_t ModeNum);

/**
 * Get Video Info from UEFI GOP and store it in Video Mode
 * Information Structure
 *
 * @param ModeInfo -- VideoMode structure to store the result info
 * 
 */
extern "C" uint16_t GetVideoInfo(VideoMode *ModeInfo);

/**
 * Initialise the Video Subsystem
 */
extern "C" void InitializeVideo ();

/**
 *
 * Checks if Graphics system is initialised or not
 *
 * @return boolean -- if initialised true or else false
 */
extern "C" bool IsGraphicsInitialised();
/**
 * Draw a pixel on the screen
 *
 * @param X -- X Value on the screen
 * @param Y -- Y Value on the screen
 * @param Color -- Color Value of the pixel
 */
extern "C" void DrawPixel (int X, int Y, uint32_t Color);

extern "C" void DrawText(const char* str);

#endif  /* END OF Video.h */