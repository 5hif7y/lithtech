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
//		PIUFile.cpp
//
//	Description:
//		File utilities for alias values acquired from the automation system.
//
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//	Includes
//-------------------------------------------------------------------------------
#include "PhotoshopSDK.h"


void AliasToFullPath(Handle aliasValue, char* fullPath)
{
	if (fullPath != NULL)
	{
		*fullPath = 0;
#ifdef __PIMac__
		OSErr err;
		Boolean wasChanged = false;
		FSSpec fileSpec;

		err = ResolveAlias(nil, (AliasRecord**)aliasValue, &fileSpec, &wasChanged);
		if (err != noErr) return;

		Str255 directoryName = "";
		err = FileSpecToFullPath(fileSpec, directoryName, fullPath);
			
#elif defined(__PIWin__)
		int32 length = sPSHandle->GetSize(aliasValue);
			
		Boolean oldLock = FALSE;
		Ptr pointer = NULL;
			
		sPSHandle->SetLock(aliasValue, true, &pointer, &oldLock);
			
		if (pointer != NULL)
		{
			strcpy(fullPath, pointer);
			sPSHandle->SetLock(aliasValue, oldLock, &pointer, &oldLock);
		}
			
		
	#endif
		}
}

void FullPathToAlias(char* fullPath, Handle& aliasValue)
{
	aliasValue = NULL;
#if __PIMac__
	// this could error with fnfErr, file not found error, ignore
	(void) sPSAlias->MacNewAliasFromCString(fullPath, &(AliasHandle)aliasValue);
#else
	Boolean oldLock;
	Ptr address;
    aliasValue = sPSHandle->New(strlen(fullPath)+1);
    if (aliasValue != NULL)
	{
		sPSHandle->SetLock(aliasValue, true, &address, &oldLock);
        strncpy(address, fullPath, strlen(fullPath)+1);
		sPSHandle->SetLock(aliasValue, false, &address, &oldLock);
    }
#endif
}


int32 GetFullPathToDesktop(char * fullPath)
{
	int32 error = 0;
	if (fullPath == NULL) return kSPBadParameterError;
	
	#if __PIMac__

		short myVRef = 0;
		long myDirID = 0;

		error = FindFolder(kOnSystemDisk, 
			 					 kDesktopFolderType, 
								 kDontCreateFolder, 
								 &myVRef, 
								 &myDirID);
		if (error) return error;

		FSSpec fsSpec;
		Str255 directoryName = "";
		error = FSMakeFSSpec(myVRef, myDirID, directoryName, &fsSpec);
		if (error) return error;
		
		error = FileSpecToFullPath(fsSpec, directoryName, fullPath);				
		if (error) return error;

		strcat(fullPath, ":");

	#else
		strcpy(fullPath, "C:\\");
	#endif
	
	return error;
}

int32 FileSpecToFullPath(const FSSpec & fileSpec, StringPtr directoryName, char * fullPath)
{
	int32 error = 0;

	#if __PIMac__

		if (fullPath == 0) return kSPBadParameterError;

		Str255 pFullPath = "\p";
		CInfoPBRec pb;
		DirInfo * block = (DirInfo *)&pb;

		PStrInsert((StringPtr)fileSpec.name, (StringPtr)pFullPath);
				
		block->ioDrParID = fileSpec.parID;
		block->ioNamePtr = directoryName;

		do {
			block->ioVRefNum = fileSpec.vRefNum;
			block->ioFDirIndex = -1;
			block->ioDrDirID = block->ioDrParID;
			error = PBGetCatInfo(&pb, FALSE);
			if (error == noErr)
			{
				PStrCat((StringPtr)"\p:", directoryName);
				PStrInsert(directoryName, pFullPath);
			}
		} while (block->ioDrDirID != 2 && error == noErr);

		if (error == noErr)
		{
			PToCStr(pFullPath, fullPath);
		}
	
	#endif

	return error;
}

