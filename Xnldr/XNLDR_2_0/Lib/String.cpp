/**
 *
 *    Copyright (C) Manas Kamal Choudhury 2020
 *
 *    /PROJECT - Xnldr v2.0
 *    /AUTHOR  - Manas Kamal Choudhury
 *
 *  --------------------------------------------------
 */


#include <Include/Stdlib/String.h>

/** ----------------------------------------------------------------
 **  [String.cpp]
 **   == Standard C String routine implimentation
 ** ----------------------------------------------------------------
 **/


/*
 * Compare two strings
 * @param str1 -- String 1
 * @param str2 -- String 2
 * @return res -- result String
 */
int strcmp (const char* str1, const char* str2)
{
     int res = 0;
	 while (!(res = *(unsigned char*)str1 - *(unsigned char*)str2) && *str2)
		 ++str1, ++str2;

	 if (res < 0)
		 res = -1;
	 if (res > 0)
		 res = 1;

	 return res;
}


/*
 * Copies Strings
 *
 * @param s1 -- String value 1 (target)
 * @param s2 -- String value 2 (source)
 */
char *strcpy (char* s1, const char* s2)
{
	char *s1_p = s1;
	while (*s1++ = *s2++);
	return s1_p;
}




/**
 *  Returns the length of String
 *
 *  @param str -- Input String
 *
 */
size_t strlen (const char* str)
{
	size_t  len = 0;
	while (str[len++]);
	return len;
}



/**
 * Copies counted bytes to target from source
 *
 * @param dest -- target location
 * @param src  -- source location
 * @param count -- count of bytes to copy
 */
void *memcpy (void *dest, const void *src, size_t count)
{
	const char *sp = (const char *)src;
	char *dp = (char *)dest;
	for (; count != 0; count--) *dp++ = *sp++;
	return dest;
}


/**
 *  Sets count bytes of dest to value
 *
 *  @param dest -- target location
 *  @param val  -- value 
 *  @param count -- count of bytes
 *
 */
void *memset (void *dest, char val, size_t count)
{
	unsigned char *temp = (unsigned char *)dest;
	for (; count != 0; count--, temp[count] = val);
	return dest;
}



/**
 *  Set count bytes of dest to value
 *
 *  @param dest -- target location
 *  @param val  -- value to set
 *  @param count -- count of bytes
 *
 */
unsigned short *memsetw (unsigned short *dest, unsigned short val, size_t count)
{
	unsigned short *temp = (unsigned short *)dest;
	for (; count != 0; count--)
		*temp++ = val;
	return dest;
}



/**
 * locates first occurance of character in string
 *
 * @param str -- String 
 * @param character -- Character to check
 *
 */
char* strchr (char *str, int character )
{
	do {
		if (*str == character )
			return (char*)str;
	}
	while (*str++);

	return 0;
}

/*
 * locates first occurance of character in string
 *
 * @param s -- String 
 * @param c -- Character to check
 */
wchar_t *wstrchr(wchar_t *s, int c) {
	while (*s) {
		if(*s == c)
			return s;
		s++;
	}

	return 0;
}

/*
 *  Returns the length of String
 *
 *  @param s -- Input String
 *
 */
int wstrlen(wchar_t *s) {
	int i = 0;
	while (*s)
		i++, s++;
	return i;
}

/**
 * return the size of string
 * 
 * @param s -- input string
 */
uint32_t wstrsize(wchar_t *s) {
	return (wstrlen(s) + 1) * sizeof(wchar_t);
}

/** ================================================================
 **  END OF String.cpp
 ** ================================================================
 */







