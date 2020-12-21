/**
 *
 *  Copyright (C) Manas Kamal Choudhury 2020
 *
 *  Va_list.h -- Varable length parameter definition
 *
 *  /PROJECT - Xnldr v2.0
 *  /AUTHOR  - Manas Kamal Choudhury
 *
 * -------------------------------------------------------------
 */

/* define __VA_LIST_H__ here */
#ifndef   __VA_LIST_H__
#define   __VA_LIST_H__



/* define varable length parameter 
 * here
 */

#ifdef __cplusplus
extern "C"
{
#endif


/**
 *  Check architecture for naming scheme
 *  for X86 (32-bit) use 'va_list' 
 *  for X64 (64-bit) use 'Va_list'
 */


#ifdef __ARCH_X86__

	/* varable length parameter */
	typedef unsigned char *va_list;

#elif __ARCH_X64__

	/* Varable length parameter */
	typedef unsigned char *Va_list;

#endif

/*
 * Now it is defined as suitable for
 * architecture width
 */


#ifdef __cplusplus
}
#endif


/* End of Va_list.h */
#endif