// ADOBE SYSTEMS INCORPORATED
// Copyright  1993 - 2002 Adobe Systems Incorporated
// All Rights Reserved
//
// NOTICE:  Adobe permits you to use, modify, and distribute this 
// file in accordance with the terms of the Adobe license agreement
// accompanying it.  If you have received this file from a source
// other than Adobe, then your use, modification, or distribution
// of it requires the prior written permission of Adobe.
//-------------------------------------------------------------------
//-------------------------------------------------------------------------------
//
//	File:
//		PIRezMacCarbon.h
//
//
//	Description:
//		This file contains the definitions for resource
//		compiling.
//
//	Use:
//		This particular file will define the flags that
//		need to be set for the resource compiler to
//		compile the code according to the right switches.
//
//-------------------------------------------------------------------------------

#ifndef __PIRezMacCarbon_H__	// Has this not been defined yet?
#define __PIRezMacCarbon_H__	// Only include this once by predefining it

#define Macintosh	1
#define MSWindows	0
#define Rez 		1

#ifndef SystemSevenOrLater
#define SystemSevenOrLater 1
#endif

#define TARGET_CARBON 1

#endif // __PIRezMacCarbon_H__
