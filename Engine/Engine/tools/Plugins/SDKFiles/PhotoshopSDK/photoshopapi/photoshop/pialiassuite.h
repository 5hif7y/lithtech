// (c) Copyright 2003.  Adobe Systems, Incorporated.  All rights reserved.
// PIAliasSuite.h - Initial - John Penn II (2003)

#pragma once

#ifndef __PIAliasSuite__
#define __PIAliasSuite__

//-------------------------------------------------------------------------------
//	Includes.
//-------------------------------------------------------------------------------
#include "pigeneral.h"

//-------------------------------------------------------------------------------
//	C++ wrapper.
//-------------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif

//-------------------------------------------------------------------------------
//	UI Alias suite.
//-------------------------------------------------------------------------------
#define kPSAliasSuite				"Photoshop Alias Utility Suite"


//-------------------------------------------------------------------------------
//	
//-------------------------------------------------------------------------------

typedef	SPAPI SPErr			(*MacNewAliasFromCStringProc) (const char *cStringPathToFile, AliasHandle *alias  );

typedef	SPAPI SPErr			(*MacNewAliasFromFSRefProc)( const void *fsRefToFile, AliasHandle *alias  );
	
typedef	SPAPI SPErr			(*MacNewAliasFromNameAndFSRefProc)( const void *cfStringFilename, const void *fsRefToParentDir, AliasHandle *alias );
	
typedef	SPAPI SPErr			(*WinNewAliasFromWidePathProc)( const uint16 *filePathAsUTF16, AliasHandle *alias );

typedef	SPAPI SPErr			(*WinNewAliasFromPIDLProc)( const void *pidlToFile, AliasHandle *alias );


//-------------------------------------------------------------------------------
//	 Alias suite version 1.
//-------------------------------------------------------------------------------

#define kPSAliasSuiteVersion1		1

typedef struct 
	{
		MacNewAliasFromCStringProc MacNewAliasFromCString;
		MacNewAliasFromFSRefProc MacNewAliasFromFSRef;
		MacNewAliasFromNameAndFSRefProc MacNewAliasFromNameAndFSRef;
		WinNewAliasFromWidePathProc WinNewAliasFromWidePath;
		WinNewAliasFromPIDLProc WinNewAliasFromPIDL;
	} PSAliasSuite;


//-------------------------------------------------------------------------------
//	C++ wrapper.
//-------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif


#endif // __PIAliasSuite__

