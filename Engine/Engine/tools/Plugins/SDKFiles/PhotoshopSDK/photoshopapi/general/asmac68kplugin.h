#ifndef __ASMac68KPlugin__
#define __ASMac68KPlugin__

/*
 *        Name:	ASMac68KPlugin.h
 *   $Revision: 1 $
 *      Author:	Dave Lazarony
 *        Date:	1/26/95   
 *     Purpose:	Adobe Macintosh 68K Plugin support.
 *
 * Copyright (c) 1986-1996 Adobe Systems Incorporated, All Rights Reserved.
 *
 */

#ifndef __ASConfig__
#include "asConfig.h"
#endif

#ifdef __MWERKS__
#warning *** THIS FILE IS OBSOLETE now that 68K code is no longer supported. ***
#else
#pragma warning // *** THIS FILE IS OBSOLETE now that 68K code is no longer supported. ***
#endif

#if !defined(__BUILD_PLUGIN__)
#if defined(ILLUSTRATOR_H)
#define __BUILD_PLUGIN__ 0
#else
#define __BUILD_PLUGIN__ 1
#endif
#endif

#if __BUILD_PLUGIN__ && defined(MAC68K_ENV)
	#include <A4Stuff.h>
	#define PUSH_GLOBALS	long __oldA4 = SetCurrentA4();
	#define POP_GLOBALS		SetA4(__oldA4);
	#define kPlatformUnloadedSuiteProc	((void *)0x50FF8001L)
#endif


#if !defined(PUSH_GLOBALS)
	#define PUSH_GLOBALS
	#define POP_GLOBALS
	#define kPlatformUnloadedSuiteProc	NULL
#endif

#endif
	


