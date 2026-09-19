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
//		PIUFile.h
//
//	Description:
//		File utilities for alias values acquired from the automation system.
//
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//	Includes
//-------------------------------------------------------------------------------
#ifndef __PIUFile__
#define __PIUFile__

#include "PIDefines.h"
#include "pitypes.h"
#include "pigeneral.h"
#include "SPBasic.h"
#include "pihandlesuite.h"
#include "PIUSuites.h"
#include "PIUtilities.h"
#ifdef __PIMac__
	#include <string.h>
	#include <stdio.h>
	#include <Folders.h>
#endif

void AliasToFullPath(Handle aliasValue, char* fullPath);
void FullPathToAlias(char* fullPath, Handle& aliasValue);
int32 GetFullPathToDesktop(char* fullPath);
int32 FileSpecToFullPath(const FSSpec& fileSpec, StringPtr directoryName, char * fullPath);

#endif