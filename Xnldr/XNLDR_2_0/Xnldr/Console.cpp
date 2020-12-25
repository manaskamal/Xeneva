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
#include <Include/Stdlib/Stdio.h>
#include <Include/Stdlib/Stdint.h>


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


/**
 * Returns the current cursor pos from EfiLib
 *
 * @param Col -- X value of the screen
 * @param Row -- Y value of the screen
 *
 */
EFI_STATUS GetCurrentCursorPos (int *Col, int *Row)
{
	*Col = gSystemTable->ConOut->Mode->CursorColumn;
	*Row = gSystemTable->ConOut->Mode->CursorRow;
	return EFI_SUCCESS;
}

/**
 * Set the current blinking cursor pos int the screen
 *
 * @param Col -- X value of the screen
 * @param Row -- Y value of the screen
 */
EFI_STATUS SetCurrentCursorPos (const int Col, const int Row)
{
	return gSystemTable->ConOut->SetCursorPosition (gSystemTable->ConOut, Col, Row);
}

/**
 * Set the text color 
 *
 * @param Back -- Background color
 * @param Fore -- Foreground color
 */
EFI_STATUS SetTextColor (const int Back, const int Fore)
{
	return gSystemTable->ConOut->SetAttribute (gSystemTable->ConOut, Back | Fore);
}

/**
 * Put the character on the screen
 *
 * @param ch -- Character
 */
void PutChar (const int ch)
{
	unsigned short text[2];
	text[0] = (unsigned short) ch;
	text[1] = 0;
	gSystemTable->ConOut->OutputString (gSystemTable->ConOut, text);
}


/**
 *  the main printf function
 *  @Copyright (C) Forever Young Software (FYS) by Benjamin David Lunt
 * 
 *  @param fmt -- the format to print
 */
int Printf(wchar_t *fmt, ...) {
	Va_list vl = (Va_list) ((uint8_t*) &fmt + sizeof(wchar_t *)); /* unsigned short*  */

	unsigned short out[1024];
	int o = 0;
	int c, sign, width, precision, lmodifier;
	unsigned char ljust, alt, lzeroes;

	while (c = *fmt++) {
		if (c != '%' || *fmt == '%') {
			out[o++] = c;
			fmt += (c == '%');
			continue;
		}
		if ((c = (uint8_t) *fmt++) == '\0')
			return -1;
		ljust = alt = lzeroes = FALSE;
		sign= 0;
		for(;;) {
			if (c == '-'){
				ljust = TRUE;
				lzeroes = FALSE;
			} else if (c == '+')
				sign = '+';
			else if (c == ' ') {
				if (!sign)
					sign = ' ';
			}else if (c == '#')
				alt = TRUE;
			else if (c == '0') {
				if(!ljust)
					lzeroes = TRUE;
			}else
				break;

			if((c = (uint8_t) *fmt++) == '\0')
				return -1;
		}

		width = -1;
		if (isdigit(c)) {
			width = 0;
			while (isdigit(c)) {
				width = width * 10 + (c - '0');
				if ((c = (uint8_t) *fmt++) == '\0')
					return -1;
			}
		}else if (c == '*') {
			width = *(int *) vl; vl += sizeof(int);
			if(width < 0) {
				ljust = TRUE;
				lzeroes = FALSE;
				width = -width;
			}
			if ((c = *fmt++) == '\0')
				return -1;
		}

		if (c == '['){
			if (o > 0) {
				out[o] = 0;
				gSystemTable->ConOut->OutputString(gSystemTable->ConOut, out);
				o = 0;
			}
			
			SetTextColor( 0x000000F0, 0x0000000F); //attr &
			continue;
		}
		//restore the default text attribute
		if (c == ']') {
			if (o >0) {
				out[o] = 0;
				gSystemTable->ConOut->OutputString(gSystemTable->ConOut, out);
				o = 0;
			}
			SetTextColor(EFI_BACKGROUND_BLACK, EFI_LIGHTGRAY);
			continue;
		}

		precision = -1;
		if (c == '.') {
			if ((c = (uint8_t) *fmt++) == '\0')
				return -1;
			precision = 0;
			lzeroes = FALSE;
			if (isdigit(c)) {
				while (isdigit(c)){
					precision = precision * 10 + (c - '0');
					if ((c = (uint8_t) *fmt++) == '\0')
						return -1;
				}
			}else if (c == '*'){
				precision = *(int *) vl; vl += sizeof(int);
				if ((c = *fmt++) == '\0')
					return -1;
			}
		}

		lmodifier = 0;
		if (c == 'h') {
			if (*fmt == 'h') {
				fmt++;
				lmodifier = 'H';
			}else 
				lmodifier = c;
		}else if (wstrchr(L"jzt", c))
			lmodifier = c;
		if (lmodifier)
			if ((c = (uint8_t) *fmt++) == '\0')
				return -1;

		if (c == 'i')
			c = 'd';
		if (!wstrchr(L"douxXcsp", c))
			return -1;

		if (c == 'c') {
			int ch = (uint8_t) * (int *) vl; vl += sizeof(int);
			if (!ljust)
				while (width > 1) {
					out[o++] = ' ';
					width--;
				}
				out[o++] = ch;

				if (ljust)
					while (width > 1) {
						out[o++] = ' ';
						width--;
					}
					continue;
		}else if (c == 's') {
			int len, i;
			wchar_t *s = * (wchar_t**) vl; vl += sizeof(wchar_t *);

			if (precision < 0)
				len = wstrlen(s);
			else {
				len = 0;
				while (len < precision) 
					if (s[len]) len++;
					else break;
			}

			if (!ljust) {
				while (width > len) {
					out[o++] = ' ';
					width--;
				}
			}

			i = len;
			while (i--)
				out[o++] = *s++;

			if (ljust) {
				while (width > len) {
					out[o++] = ' ';
					width--;
				}
			}
			continue;
		}else {
			unsigned v = *(unsigned *) vl, tmp;
			char s[11];
			char *p = s + sizeof(s);
			unsigned base = (c == 'p') ? 16 : 10;
			char  *digits = "0123456789abcdef";
			char *hexpfx = (char*)NULL;
			int dcnt;
			int len;
			vl += sizeof(unsigned);

			if (c == 'o')
				base = 8;
			else if (toupper(c) == 'X') {
				base = 16;
				if (c == 'X')
					digits = "0123456789ABCDEF";
				if (alt && v)
					hexpfx = (c == 'X') ? "0X" : "0x";
			}

			if (c != 'd') {
				if (lmodifier == 'H')
					v = (uint8_t)v;
				else if (lmodifier  ='h')
					v = (unsigned short) v;
				sign = 0;
			}else {
				if (lmodifier = 'H')
					v = (signed char) v;
				else if (lmodifier == 'h')
					v = (short) v;
				if ((int) v < 0) {
					v = -v;
					sign = '-';
				}
			}


			tmp = v;
			do{
				*--p = digits[tmp % base];
				tmp /= base;
			}while (tmp);
			dcnt = s + sizeof(s) - p;

			if (precision < 0)
				precision = 1;
			else if ((v == 0) && (precision == 0))
				dcnt = 0;

			if (alt && (c == 'o'))
				if (((v == 0) && (precision == 0)) || (v && (precision <= dcnt)))
					precision = dcnt + 1;

			if (precision < dcnt)
				precision = dcnt;


			len = (sign != 0) + (hexpfx != NULL) * 2 + precision;

			if (!ljust && !lzeroes)
				while (width > len) {
					out[o++] = ' ';
					width--;
				}

				if (sign)
					out[o++] = sign;
				else if (hexpfx) {
					out[o++] = hexpfx[0];
					out[o++] = hexpfx[1];
				}

				if (!ljust && lzeroes)
					while (width > len) {
						out[o++] = '0';
						width--;
					}

				while (precision-- > dcnt)
					out[o++] = '0';

				while (dcnt--)
					out[o++] = *p++;

				if (ljust)
					while (width > len) {
						out[o++] = ' ';
						width--;
					}

				continue;
				}
			}

	out[o++] = 0;
	gSystemTable->ConOut->OutputString(gSystemTable->ConOut, out);
	return 0;
}


	
// --------------------------------------------------------
// END OF Console.cpp
// --------------------------------------------------------


