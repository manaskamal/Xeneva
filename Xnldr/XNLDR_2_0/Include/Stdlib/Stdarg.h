/**
 * 
 *  Copyright (C) Manas Kamal Choudhury 2020
 *
 *  /PROJECT - Xnldr v2.0
 *  /AUTHOR  - Manas Kamal Choudhury
 *
 *  ------------------------------------------------
 */

//! define __STDARG_H__ here
#ifndef __STDARG_H__
#define __STDARG_H__

#include <Include/Stdlib/Va_list.h>

#ifdef __cplusplus
extern "C"
{
#endif

/* width of stack == width of int */
#define	STACKITEM	int

/* round up width of objects pushed on stack. The expression before the
& ensures that we get 0 for objects of size 0. */
#define	VA_SIZE(TYPE)					\
	((sizeof(TYPE) + sizeof(STACKITEM) - 1)	\
		& ~(sizeof(STACKITEM) - 1))

/* &(LASTARG) points to the LEFTMOST argument of the function call
(before the ...) */
#define	va_start(AP, LASTARG)	\
	(AP=((Va_list)&(LASTARG) + VA_SIZE(LASTARG)))

/* nothing for va_end */
#define va_end(AP)

#define va_arg(AP, TYPE)	\
	(AP += VA_SIZE(TYPE), *((TYPE *)(AP - VA_SIZE(TYPE))))

#ifdef __cplusplus
}
#endif


#endif