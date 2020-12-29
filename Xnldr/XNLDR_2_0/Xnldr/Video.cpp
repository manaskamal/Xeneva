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

#include <Include/Video.h>
#include <Include/Xeneva.h>
#include <Uefi.h>
#include <Protocol/GraphicsOutput.h>
#include <Include/EfiLib.h>
#include <Include/Console.h>
#include <Include/ConsoleFont.h>

/**
 * GraphicsOutputProtocol GUID
 */
EFI_GUID GraphicsOutputProtocol = 
{
	0x9042A9dE,
	0x23DC,
	0x4A38,
	{
		0x96, 0xFB, 0x7A, 0xDE, 0xD0, 0x80, 0x51, 0x6A
	}
};

/**
 * Define some global variable
 */
EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop;
EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *CurModeInfo = nullptr;
uint32_t  *CurLFB = nullptr;

/** Maximum Video Modes is 32 */
uint16_t ModeNums [MAX_VIDEO_MODES];

static VideoMode *VideoInformation;
static bool GraphicsInitialised = 0;
/**
 * Conversion Functions 
 */
uint16_t  BitmaskConvert (uint32_t Mask)
{
	int Start = 0, Count = 0;
	int Pos = 0;

	while ((Pos < 32 ) && (! (Mask & (1 << Pos))))
		Pos++;

	if (Pos == 32)
		return 0x0000;

	Start = Pos;

	while ((Pos < 32) && (Mask & (1 << Pos)))
		Pos++, Count++;

	return ((Start << 8) | Count);
}

/**
 * return the highest most bit to given value
 *
 * @param val -- input value
 */
int_fast8_t HighestSetBit (size_t Val)
{
	int_fast8_t Count = -1;
	while (Val)
	{
		Val >>= 1;
		Count += 1;
	}
	return Count;
}

/**
 * return the Lowest most bit from a given value
 *
 * @param Val -- Input Value
 *
 */
int_fast8_t LowestSetBit(size_t Val) {

	int_fast8_t Count = -1;
	while(Val){
		Count += 1;
		if ((Val & 1) != 0)
			break;
		Val >>= 1;
	}
	return Count;
}

/**
 * Get current Pixel Size
 *
 * @param PixelInformation -- Pixel Information
 */
uint16_t GetPixelSize (EFI_PIXEL_BITMASK *PixelInformation)
{
	int HighestPixel = -1;
	int BluePixel = HighestSetBit (PixelInformation->BlueMask);
	int RedPixel = HighestSetBit (PixelInformation->RedMask);
	int GreenPixel = HighestSetBit (PixelInformation->GreenMask);
	int RsvdPixel = HighestSetBit (PixelInformation->ReservedMask);

	HighestPixel = ((BluePixel > RedPixel) ? BluePixel : RedPixel);
	HighestPixel = ((HighestPixel > GreenPixel) ? HighestPixel : GreenPixel);
	HighestPixel = ((HighestPixel > RsvdPixel) ? HighestPixel : RsvdPixel);

	return (uint16_t) (HighestPixel + 1);
}

/**
 * Get Video Info from firmware to Buffer
 *
 * @param ModeInfo -- ModeInfo structure to store
 */
uint16_t GetVideoInfo (VideoMode *ModeInfo)
{
	EFI_STATUS Status;
	EFI_HANDLE *HandleBuffer;
	uint32_t  HandleCount = 0;
	uint32_t ModeNum, SizeOfInfo, Count = 0;

	EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *GopModeInfo;

	Status = gBootServices->LocateHandleBuffer (ByProtocol, &GraphicsOutputProtocol, nullptr, (UINTN*)&HandleCount, &HandleBuffer);

	/** Error found **/
	if (EFI_ERROR (Status))
	{
		Printf (L"No Video Output will be available 1\r\n");
		return 0;
	}

	Status = gBootServices->HandleProtocol (HandleBuffer[0] , &GraphicsOutputProtocol, (void **) &Gop);

	/** Error found **/
	if (EFI_ERROR (Status))
	{
		Printf (L"No Video Output will be available 2\r\n");
		return 0;
	}


	/**  Iterate through available video modes **/

	for ( ModeNum = 0; (Status = Gop->QueryMode (Gop, ModeNum, (UINTN*)SizeOfInfo, &GopModeInfo)) == EFI_SUCCESS; 
		  ModeNum++)
	{
		Printf (L"\r\n Hello \r\n");
		ModeInfo[Count].Lfb = Gop->Mode->FrameBufferBase;
		ModeInfo[Count].XRes = GopModeInfo->HorizontalResolution;
		ModeInfo[Count].YRes = GopModeInfo->VerticalResolution;

		switch (GopModeInfo->PixelFormat)
		{
		case PixelRedGreenBlueReserved8BitPerColor:
			ModeInfo[Count].Red = 0x0008;
			ModeInfo[Count].Blue = 0x0808;
			ModeInfo[Count].Green = 0x1008;
			ModeInfo[Count].BitsPerPixel = sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL) * 8;
			ModeInfo[Count].BytesPerScanline = (GopModeInfo->PixelsPerScanLine * sizeof (EFI_GRAPHICS_OUTPUT_BLT_PIXEL));
			ModeInfo[Count].MemoryModel = 6;
			break;
		case PixelBlueGreenRedReserved8BitPerColor:
			ModeInfo[Count].Red = 0x1008;
			ModeInfo[Count].Blue = 0x0808;
			ModeInfo[Count].Green = 0x0008;
			ModeInfo[Count].BitsPerPixel = sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL) * 8;
			ModeInfo[Count].BytesPerScanline = (GopModeInfo->PixelsPerScanLine * sizeof (EFI_GRAPHICS_OUTPUT_BLT_PIXEL));
			ModeInfo[Count].MemoryModel = 4;
			break;
		case PixelBitMask:
			ModeInfo[Count].Red = BitmaskConvert (GopModeInfo->PixelInformation.RedMask);
			ModeInfo[Count].Blue = BitmaskConvert (GopModeInfo->PixelInformation.BlueMask);
			ModeInfo[Count].Green = BitmaskConvert (GopModeInfo->PixelInformation.GreenMask);
			ModeInfo[Count].BitsPerPixel = GetPixelSize (&GopModeInfo->PixelInformation);
			ModeInfo[Count].BytesPerScanline = (GopModeInfo->PixelsPerScanLine * (ModeInfo[Count].BitsPerPixel / 8));
			ModeInfo[Count].MemoryModel = 6;
			break;
		case PixelBltOnly:
			continue;
		}

		ModeNum[&Count] = ModeNum;
		
		if (++Count  == MAX_VIDEO_MODES)
			break;

	}

	return Count;
}


/**
 * Find a video, with given XRes, YRes, and bitdepth
 *
 * @param ModeInfo -- Mode Information Structure to store to
 * @param Count -- Bit Depth count
 * @param XRes -- Width of the Screen
 * @param YRes -- Height of the Screen
 *
 */
uint16_t FindVideoMode (VideoMode *ModeInfo, uint16_t Count, int XRes, int YRes)
{
	for (uint16_t i=0; i < Count; i++)
		if ((ModeInfo[i].XRes == XRes) &&  (ModeInfo[i].YRes == YRes))
			return i;

	return 0xFFFF;
}


/**
 * Set a video mode with given mode number
 *
 * @param ModeNum -- Mode Number to set
 *
 */
EFI_STATUS SetVideoMode (uint32_t ModeNum)
{
	 Gop->SetMode (Gop, ModeNum);
	 EFI_GRAPHICS_OUTPUT_MODE_INFORMATION* ModeInfo = Gop->Mode->Info;

	 XENEVA_INFO *info = GetXenevaInfo ();

	 VideoInformation->Lfb = Gop->Mode->FrameBufferBase;
	 VideoInformation->XRes = ModeInfo->HorizontalResolution;
	 VideoInformation->YRes = ModeInfo->VerticalResolution;
	 VideoInformation->PixelsPerLine = ModeInfo->PixelsPerScanLine;
	
	 switch (Gop->Mode->Info->PixelFormat)
	 {
		case PixelRedGreenBlueReserved8BitPerColor:
			VideoInformation->Red = 0xFF;
			VideoInformation->Green = 0xFF00;
			VideoInformation->Blue = 0xFF0000;
			VideoInformation->Resv = 0xFF000000;
			break;
		case PixelBlueGreenRedReserved8BitPerColor:
			VideoInformation->Red = 0xFF0000;
			VideoInformation->Green = 0xFF00;
			VideoInformation->Blue = 0xFF;
			VideoInformation->Resv = 0xFF000000;
			break;
		case PixelBitMask:
			VideoInformation->Red = Gop->Mode->Info->PixelInformation.RedMask;
			VideoInformation->Green = Gop->Mode->Info->PixelInformation.GreenMask;
			VideoInformation->Blue = Gop->Mode->Info->PixelInformation.BlueMask;
			VideoInformation->Resv = Gop->Mode->Info->PixelInformation.ReservedMask;
			break;
	 }

	 size_t MergeMask = VideoInformation->Red | VideoInformation->Green | VideoInformation->Blue | VideoInformation->Resv;
	 VideoInformation->BitsPerPixel = HighestSetBit ( MergeMask + 1);

	 EFI_GRAPHICS_OUTPUT_BLT_PIXEL pixel;
	 pixel.Blue = 164;
	 pixel.Green = 115;
	 pixel.Red = 0;

	 Gop->Blt(Gop, &pixel, EfiBltVideoFill, 0, 0, 0, 0, VideoInformation->XRes, VideoInformation->YRes, 0);
	
	 /**
	  * Copy the information to Xeneva Boot Info Structure
	  */
	 info->Video.PhysicalAddress = VideoInformation->Lfb;
	 info->Video.PixelsPerLine = VideoInformation->PixelsPerLine;
	 info->Video.BitPerPixel = VideoInformation->BitsPerPixel;
	 info->Video.Width = VideoInformation->XRes;
	 info->Video.Height = VideoInformation->YRes;
	 info->Video.Red = VideoInformation->Red;
	 info->Video.Green = VideoInformation->Green;
	 info->Video.Blue = VideoInformation->Blue;
	 info->Video.Resv = VideoInformation->Resv;
	
	 /* Now set bit 1 to Graphics initialized boolean value */
	 GraphicsInitialised = true;
	 return EFI_SUCCESS;
}


/**
 * Initialize our Xnldr Video Subsystem
 *
 */
void InitializeVideo ()
{
	VideoMode ModeInfo[MAX_VIDEO_MODES];
	uint16_t ModeCount = GetVideoInfo (ModeInfo);
	
	int Avail [24];
	int j, i = 0;
	uint16_t ch;

	if (ModeCount == 0)
	{
		/*
		 * There's an error, no video mode found 
		 */
		Printf (L"\r\n ***No video output available! Xnldr will set to desired Mode ***\r\n");
	}
NextEntry:
	for (j = 0; i < ModeCount && j < 23; i++)
	{
		Printf(L"Found Video: Width %i, Height %i", ModeInfo[i].XRes, ModeInfo[i].YRes);

		Avail[j] = i;
		j++;
	}
	
	Printf (L"\r\n Press Any Key ... \r\n");
	EFI_INPUT_KEY Key;
	GetKeyStroke (&Key);
	/** From here Xnldr will set its video mode to 
	 *  Xeneva demanded mode
	 */
	SetVideoMode(8);
}

/**
 *
 * Checks if Graphics system is initialised or not
 *
 * @return boolean -- if initialised true or else false
 */
bool IsGraphicsInitialised()
{
	return IsGraphicsInitialised;
}

/**
 * Draw a pixel on the screen
 *
 * @param X -- X Value on the screen
 * @param Y -- Y Value on the screen
 * @param Color -- Color Value of the pixel
 */
void DrawPixel (int X, int Y, uint32_t Color)
{
	XENEVA_INFO *Info = GetXenevaInfo ();

	uint32_t *LFBAddress = raw_offset<uint32_t*>(VideoInformation->Lfb, (VideoInformation->PixelsPerLine * Y + X ) * (VideoInformation->BitsPerPixel / 8 ));
	size_t  Pixel = ((RED(Color) << LowestSetBit (VideoInformation->Red)&VideoInformation->Red) | 
		((GREEN(Color) << LowestSetBit(VideoInformation->Green)) & VideoInformation->Green) |
		((BLUE(Color) << LowestSetBit(VideoInformation->Blue))&VideoInformation->Blue));
	*LFBAddress = Pixel;
}

static int xpos = 0;
static int ypos = 0;
static int LINES = 20;
static uint32_t foreground = RGB(255,255,255);
static uint32_t background = RGB(0,115,164);

/**
 * Draw a text using GOP Graphics
 *
 * @param str -- Message to Print
 */
void DrawText(const char* str)
{
	// *Info = GetXenevaInfo();

	while (*str)
	{

		if (*str > 0xFF)
		{
			//unicode
		}else if (*str == '\n'){

			++ypos;
			xpos = 0;
		}else if (*str == '\r'){
		}else if(*str == '\b'){
			if (xpos > 0)
				--xpos;
		}
		else {

			const bx_fontcharbitmap_t entry = bx_vgafont[*str];
			for (size_t y = 0; y < 16; ++y)
			{

				for (size_t x = 0; x < 8; ++x)
				{

					if (entry.data[y] & (1 << x))
					{
						DrawPixel(x + xpos * 9, y + ypos * 16, foreground);
					}else {
						DrawPixel(x + xpos * 9, y + ypos * 16, background);
					}
				}
				DrawPixel(8 + xpos * 9, y + ypos * 16, background);
			}
			++xpos;
			if (xpos > VideoInformation->XRes / 9)
			{
				xpos = 0;
				++ypos;
			}
		}

		if (ypos >= VideoInformation->YRes / 16 - 1)
		{
			if (LINES > 0)
			{
				void* dest = raw_offset<void*>(VideoInformation->Lfb, (16 * (ypos - LINES) * VideoInformation->PixelsPerLine)*(VideoInformation->BitsPerPixel / 8));
				void* src = raw_offset<void*>(VideoInformation->Lfb, (16 * (ypos - (LINES - 1)) * VideoInformation->PixelsPerLine)*(VideoInformation->BitsPerPixel / 8));
				memcpy(dest, src, (((LINES - 1) * 16)*VideoInformation->PixelsPerLine)*(VideoInformation->BitsPerPixel / 8));
			}
			--ypos;
			for (size_t n = 0; n < 16; ++n){

				for (size_t x = 0; x < VideoInformation->XRes; ++x){

					DrawPixel(x, n + ypos * 16, background);
				}
			}
			ypos = 0;
		}
		
		++str;
	}
}
				