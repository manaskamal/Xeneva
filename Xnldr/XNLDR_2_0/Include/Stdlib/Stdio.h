/**
 *  Copyright (C) Manas Kamal Choudhury 2020
 *
 *  Stdio.h -- Standard Input/Output Stream header
 *
 *  /PROJECT - Xnldr v2.0
 *  /AUTHOR  - Manas Kamal Choudhury
 *
 *  -------------------------------------------------------
 */

//! define __STDIO_H__  here
#ifndef __STDIO_H__
#define __STDIO_H__

#include <Include/Stdlib/Stdarg.h>

extern int vsprintf (char *str, const char *format, Va_list ap);
extern int vsnprintf (char *str, size_t size, const char *format, Va_list ap);
#endif