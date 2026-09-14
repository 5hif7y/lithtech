
#ifndef _ASLIB_H_
#define _ASLIB_H_

/*
 *        Name:	ASTypes.h
 *   $Revision: 7 $
 *      Author:	
 *        Date:	10/8/96
 *     Purpose:	Adobe Standard Functions (from stdio, stlib, etc.).
 *
 * Copyright (c) 1986-1996 Adobe Systems Incorporated, All Rights Reserved.
 *
 */

/*
 * Includes
 */

#include <stdlib.h>

#include "asTypes.h"

#ifdef __cplusplus
extern "C"
{
#endif

#pragma PRAGMA_ALIGN_BEGIN
#pragma PRAGMA_IMPORT_BEGIN

/*
 * Constants
 */

#define kASLibSuite				"AS Lib Suite"
#define kASLibSuiteVersion		1
#define kASLibVersion			kASLibSuiteVersion

typedef struct
{
	// functions from stdio.h
	ASCAPI int    (*sprintf) (char *, const char *, ...);

	// functions from stdlib.h

	// functions from string.h

	ASAPI char * (*strchr)  (const char *, int);
	ASAPI void * (*memcpy)  (void *, const void *, size_t);
	ASAPI int    (*memcmp)  (const void *, const void *, size_t);
	ASAPI void * (*memset)  (void *, int, size_t);
	ASAPI void * (*memmove) (void *, const void *, size_t);
	ASAPI char * (*strcpy)  (char *, const char *);
	ASAPI char * (*strcat)  (char *, const char *);
	ASAPI int    (*strcmp)  (const char *, const char *);
	ASAPI size_t (*strlen)  (const char *);
	ASAPI char * (*strncat) (char *, const char *, size_t);
	ASAPI int    (*strncmp) (const char *, const char *, size_t);
	ASAPI char * (*strncpy) (char *, const char *, size_t);
	ASAPI char * (*strrchr) (const char *, int);
	ASAPI char * (*strstr)  (const char *, const char *);

	// functions that "should" be in one of the above (:-)

	ASAPI char * (*strsubst)(char *pszStr, int iMaxLen, const char *pszSearch, const char *pszReplace);

} ASLibSuite;

#pragma PRAGMA_IMPORT_END
#pragma PRAGMA_ALIGN_END

#ifdef __cplusplus
}
#endif

#endif // _ASLIB_H_

