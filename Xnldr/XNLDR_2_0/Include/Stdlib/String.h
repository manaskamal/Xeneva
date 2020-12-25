/**
 *  
 *  Copyright (C) Manas Kamal Choudury 2020
 *
 *  String.h -- Standard String manipulation 
 *
 *  /PROJECT -- Xnldr v2.0
 *  /AUTHOR  -- Manas Kamal Choudhury
 *
 * ----------------------------------------------------
 */

/* define __STRING_H__ here */
#ifndef __STRING_H__
#define __STRING_H__

#include <Include/Stdlib/Stdint.h>

/* maximum string length is 4096 */
#define MAX_STRING_LENGTH   4095

/* define 'NULL' here */
#ifndef NULL
#define NULL 0
#endif

/*
 * Functions
 */

/* String Functions */
extern "C" char* strcpy (char *s1, const char *s2);
extern "C" size_t strlen (const char* str);
extern "C" int strcmp (const char* str1, const char* str2);
extern "C" char* strchr (char *str, int character);

/* Memory Functions */
extern "C" void* memcpy (void *dest, const void *src, size_t count);
extern "C" void* memset (void *dest, char val, size_t count);
extern "C" unsigned short* memsetw (unsigned short *dest, unsigned short val, size_t count);


extern "C" wchar_t *wstrchr(wchar_t *s, int c);
extern "C" int  wstrlen(wchar_t *s);
extern "C" uint32_t wstrsize(wchar_t *s);

/**
 * For now, this much is sufficient
 * for future development it will be updated
 */

/* End of String.h */
#endif