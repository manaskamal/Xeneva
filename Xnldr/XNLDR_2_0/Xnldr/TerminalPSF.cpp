/**
 *
 *  Copyright (C) Manas Kamal Choudhury 2020
 *
 *  Terminal.cpp -- 
 *
 *  /PROJECT - Xnldr v2.0
 *  /AUTHOR  - Manas Kamal Choudhury
 *
 */

#include <Include/TerminalPSF.h>


uint32_t Bg = RGB(0,115,164);
uint32_t Fg = RGB(255,255,255);

	

/**
 * Print a string in PSF Font format
 *
 * @param message -- Message to print
 * @param POS_X   -- X Value for the screen
 * @param POS_Y   -- Y Value for the screen
 */
void TerminalPutString_PSF (const char* string, int POS_X, int POS_Y)
{
	
     int Space = 0;
	 for (unsigned int i=0; i < strlen(string); i++){
	    PSFPutC(string[i], POS_X + Space, POS_Y, Bg, Fg);
		Space += 10;
	 }

}
