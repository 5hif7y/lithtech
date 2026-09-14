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
//		TriggerFiltersScripting.cpp
//
//	Description:
//		This file contains the source scripting routines for the
//		Actions module TriggerFilters, an example of a module
//		that uses ADM and the Actions suite to execute events
//		from the Filter menu.
//
//	Use:
//		This is a good example of a basic shell to pop a cross-
//		platform UI using ADM and trigger basic events.
//
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
//	Includes
//-------------------------------------------------------------------------------

#include "PhotoshopSDK.h"
#include "TriggerFilters.h"

//-------------------------------------------------------------------------------
//	Prototypes.
//-------------------------------------------------------------------------------
static showDialog_t MapKeyToShowDialog(const DescriptorKeyID key);
static DescriptorKeyID MapShowDialogToKey(const showDialog_t showDialog);
static SPErr RestoreFileSpec(PIActionDescriptor descriptor,const DescriptorKeyID key,SPPlatformFileSpecification*		fileSpec);
static SPErr RecordFileSpec(PIActionDescriptor descriptor, const DescriptorKeyID key,const SPPlatformFileSpecification 	fileSpec);
//-------------------------------------------------------------------------------
//
//	ReadScriptParams
//
//	Checks the parameters against scripting-returned parameters, if any, and
//	updates the globals to match ones provided by the scripting system.
//
//-------------------------------------------------------------------------------

SPErr ReadScriptParams (PIActionParameters* actionParams)
{
	SPErr error = kSPNoError;
	
	if (sPSActionDescriptor != NULL)
		{ // Make sure we have a valid suite before trying this.
		PIActionDescriptor descriptor = actionParams->descriptor;
					
		// If we got a valid descriptor, grab our key out of it:
		if (descriptor != NULL && gGroupList != NULL &&
			gGroupList->eventList != NULL)
			{
			char string[kMaxStr255Len];
			unsigned long size = 0;
			
			error = sPSActionDescriptor->GetStringLength
				(
				descriptor,
				keyGroup,
				&size
				);
				
			// If we don't have space to allocate the whole string,
			// then we can't get it.  Asking it to truncate (by
			// specifying a max less than what it just reported
			// was its length) produces unknown results.
			if (size < kMaxStr255Len)
				{				
				error = sPSActionDescriptor->GetString
					(
					descriptor,
					keyGroup,
					(char*)&string,
					kMaxStr255Len
					);
				
					// Even though we write out both the name
					// and the id, we don't want to allocate
					// yet additional copies for them, so
					// I'll use this temporary string
					// to look it up in our list:
					gLastGroupList = FindNameInGroupList(string);
					
					if (gLastGroupList == NULL)
						{
						// Oops, didn't find it.
						gLastGroupList = gGroupList; // Default.
						}
				
				} // size
			
			// We're going to go and get the name string.  If
			// it's other, then we need not look up anything:
			error = sPSActionDescriptor->GetStringLength
				(
				descriptor,
				keyLastFilter,
				&size
				);
				
			if (size < kMaxStr255Len && error == kSPNoError && gLastGroupList != NULL)
				{
				error = sPSActionDescriptor->GetString
					(
					descriptor,
					keyLastFilter,
					(char*)&string,
					kMaxStr255Len
					);
			
				if (error == kSPNoError)
					{
					if (sSPBasic->IsEqual(string, otherName))
						{
						// First we'll set up the standard "other"
						// eventList:
						gLastGroupList->lastEventList =
							FindIDInEventList
								(
								otherKey, 
								gLastGroupList->eventList
								);
						
						// Now we'll read the key:
						error = sPSActionDescriptor->GetStringLength
							(
							descriptor,
							keyLastID,
							&size
							);
						
						if (size < kMaxStr255Len && error == kSPNoError)
							{
							error = sPSActionDescriptor->GetString
								(
								descriptor,
								keyLastID,
								gOtherEvent,
								kMaxStr255Len
								);
							}
						}
					else
						{
						// Was not "other".  Get ID and look
						// up from that:
						error = sPSActionDescriptor->GetStringLength
							(
							descriptor,
							keyLastID,
							&size
							);
							
						if (size < kMaxStr255Len)
							{
							error = sPSActionDescriptor->GetString
								(
								descriptor,
								keyLastID,
								string,
								kMaxStr255Len
								);
								
							if (error == kSPNoError)
								{
								gLastGroupList->lastEventList = 
									FindIDInEventList
										(
										string,
										gLastGroupList->eventList
										);
								} // error
								
							} // size	
						
						} // else
						
					} // error
				} // size
				
			DescriptorEnumTypeID type = typeDialogOptions;
			DescriptorKeyID key = typeNull;
			error = sPSActionDescriptor->GetEnumerated
				(
				descriptor,
				keyShowDialog,
				&type,
				&key
				);
				
			gShowDialog = MapKeyToShowDialog(key);

			Boolean tempBool = false;
			
			error = sPSActionDescriptor->GetBoolean
				(
				descriptor,
				keyLogToFile,
				&tempBool
				);
			
			if (error == kSPNoError && tempBool)
				{
				// Oooh, we've got a file we're logging
				// to.  Go get its file spec:
				error = RestoreFileSpec
					(
					descriptor,
					keyOurFilename,
					&gFile
					);
				
				// If we read a valid file spec, then
				// all's good in the world.  Otherwise,
				// don't set this flag.
				if (ValidFileSpec(gFile))
					gLogToFile = true;
				}
			
			} // descriptor == NULL
		
		}

	return error;
	
} // end ReadScriptParams
		
//-------------------------------------------------------------------------------
//
//	WriteScriptParams
//
//	Takes our parameters from our global variables and writes them out to the
//	scripting system, which hands it all back to the host.
//
//-------------------------------------------------------------------------------

SPErr WriteScriptParams (PIActionParameters* actionParams)
{
	SPErr				error = kSPNoError; // we'll return any error with this

	if (sPSActionDescriptor != NULL && gLastGroupList != NULL &&
		gLastGroupList->lastEventList != NULL)
		{ // Make sure we have a valid suite before doing this.
		PIActionDescriptor	descriptor = NULL;	// token to write our parameters to
		
		error = sPSActionDescriptor->Make(&descriptor);
		
		error = sPSActionDescriptor->PutString
			(
			descriptor,
			keyGroup,
			gLastGroupList->name
			);
		
		error = sPSActionDescriptor->PutString
			(
			descriptor,
			keyLastFilter,
			gLastGroupList->lastEventList->name
			);
		
		char* id = gLastGroupList->lastEventList->id;
			
		if 
			(
			sSPBasic->IsEqual
				(
				id,
				otherKey
				)
			)
			{
			id = gOtherEvent;
			}
			
		error = sPSActionDescriptor->PutString
			(
			descriptor,
			keyLastID,
			id
			);
			
		error = sPSActionDescriptor->PutEnumerated
			(
			descriptor,
			keyShowDialog,
			typeDialogOptions,
			MapShowDialogToKey(gShowDialog)
			);
			
		if (gLogToFile)
			{ // Default is false.  Put only if true.
			error = sPSActionDescriptor->PutBoolean
				(
				descriptor,
				keyLogToFile,
				gLogToFile
				);
			
			error  = RecordFileSpec
				(
				descriptor,
				keyOurFilename,
				gFile
				);
			}			
			
		// Now stuff our return descriptor for return to the host:
		if (error == 0 && actionParams != NULL)
			{
			if (actionParams->descriptor)
				sPSActionDescriptor->Free( actionParams->descriptor );

			actionParams->descriptor = descriptor;
			actionParams->recordInfo = plugInDialogOptional;
			}

		}
	
	return error;
		
} // end WriteScriptParams


//-------------------------------------------------------------------------------
//
//	MapKeyToShowDialog
//
//	Returns ShowDialog for each key.	
//
//-------------------------------------------------------------------------------

static showDialog_t MapKeyToShowDialog(const DescriptorKeyID key)
	{		
	showDialog_t showDialog = mapKeyToShowDialog[0].showDialog;
	
	unsigned short loop = 0;
	
	while (loop++ < mapKeyToShowDialog_size)
		{
		if (mapKeyToShowDialog[loop].key == key)
			{
			showDialog = mapKeyToShowDialog[loop].showDialog;
			loop = mapKeyToShowDialog_size;	// Done indicator.
			}
		}
	
	return showDialog;
	}

//-------------------------------------------------------------------------------
//
//	MapShowDialogToKey
//
//	Returns key for specific ShowDialog.	
//
//-------------------------------------------------------------------------------
static DescriptorKeyID MapShowDialogToKey(const showDialog_t showDialog)
	{
	DescriptorKeyID key = mapKeyToShowDialog[0].key;
	
	unsigned short loop = 0;
	
	while (loop++ < mapKeyToShowDialog_size)
		{
		if (mapKeyToShowDialog[loop].showDialog == showDialog)
			{
			key = mapKeyToShowDialog[loop].key;
			loop = mapKeyToShowDialog_size;	// Done indicator.
			}
		}
	
	return key;
	}
	
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
//
//	PIURecordFileSpec
//
//	Stores the filename for Mac and Win in an appropriate descriptor.
//
//-------------------------------------------------------------------------------
static SPErr RecordFileSpec
	(
	/* IN/OUT */	PIActionDescriptor 					descriptor,
	/* IN */		const DescriptorKeyID				key,
	/* IN */		const SPPlatformFileSpecification 	fileSpec
	)
	{
	SPErr error = kSPBadParameterError;
	
	if (sPSActionDescriptor != NULL)
		{
		
		#ifdef __PIMac__
		
		// Mac PlatformFileHandles are stored as aliases.  SweetPea creates an
		// SPPlatformFileSpecification from the GetFile dialog that is the
		// equivalent of a Mac FSSpec:
		AliasHandle		alias;
		
		error = NewAlias(nil, (FSSpec *)&fileSpec, &alias);
		
		if (error == noErr)
			{ // Now store it.
		
			error = sPSActionDescriptor->PutAlias
				(
				descriptor,
				key,
				(Handle)alias
				);
			}
			
		#elif defined(__PIWin__)
		
		// Windows PlatformFileHandles are stored as text paths.  SweetPea
		// creates an SPPlatformFileSpecification from the GetFile dialog that
		// is a 300-max character path:
		
		if (sPSHandle != NULL)
			{

			const unsigned short length = strlen(fileSpec.path);

			if (length > 0)
				{
				
				Handle h = sPSHandle->New(length+1); // Extra for null.

				if (h != NULL)
					{
					Ptr pointer = NULL;
					Boolean oldLock = false;
					
					sPSHandle->SetLock
						(
						h,
						true,
						&pointer,
						&oldLock
						);

					if (pointer != NULL)
						{
						strncpy(pointer,fileSpec.path,length);

						if (error == kSPNoError)
							pointer[length] = 0;
						}

					// Whether pointer returned null or not, we'll try to unlock:
					sPSHandle->SetLock
						(
						h,
						oldLock,		// Hopefully false.
						&pointer,
						&oldLock
						);
	
					error = sPSActionDescriptor->PutAlias
						(
						descriptor,
						key,
						h
						);
				
					// Now release the handle, since the host makes a copy:
					sPSHandle->Dispose(h);
					h = NULL;
					} // h
				} // length
			} // handle
			
		#endif	
		
		} // descriptor && handle
		
	return error;
	
	} // PIURecordFilename
		
//-------------------------------------------------------------------------------
//
//	PIURestoreFileSpec
//
//	Reads the file spec from an appropriate descriptor and restores the
//	file spec.
//
//-------------------------------------------------------------------------------
static SPErr RestoreFileSpec
	(
	/* IN */	PIActionDescriptor 					descriptor,
	/* IN */	const DescriptorKeyID				key,
	/* OUT */	SPPlatformFileSpecification*		fileSpec
	)
	{
	SPErr error = kSPBadParameterError;
	
	if (sPSActionDescriptor == NULL)
		return error;
	
	#ifdef __PIMac__
	AliasHandle alias;
	
	error = sPSActionDescriptor->GetAlias
		(
		descriptor,
		key,
		&(Handle)alias
		);
		
	if (error == noErr)
		{
		Boolean wasChanged = false;
		
		error = ResolveAlias(nil, alias, (FSSpec*)fileSpec, &wasChanged);
		
		if (error != noErr || fileSpec->name[0] < 1)
			{
			// Couldn't find it, so make a record with just the name.
			error = GetAliasInfo(alias, asiAliasName, fileSpec->name);
			
			if (error != noErr && fileSpec->name[0] > 0)
				FSMakeFSSpec(0, 0, fileSpec->name, (FSSpec*)fileSpec);
			}
		}
		
	#elif defined(__PIWin__)
		
		if (sPSHandle == NULL)
			return error;
			
		Handle h = NULL;
		
		error = sPSActionDescriptor->GetAlias
			(
			descriptor,
			key,
			&h
			);
			
		if (h != NULL)
			{
			unsigned short length = (unsigned short)sPSHandle->GetSize(h);
			
			Boolean oldLock = FALSE;
			Ptr pointer = NULL;
			
			sPSHandle->SetLock
				(
				h,
				true,
				&pointer,
				&oldLock
				);
			
			if (pointer != NULL)
				{
				if (length > kMaxPathLength)
					length = kMaxPathLength-1;
					
				strncpy(fileSpec->path,pointer,length);
					
				fileSpec->path[length] = 0;
				
				// Ignoring error:
				sPSHandle->SetLock
					(
					h,
					oldLock,
					&pointer,
					&oldLock
					);
				
				} // pointer
			
			// Ignoring error:
			sPSHandle->Dispose(h);
			h = NULL;
			
			} // Handle

		if (error == kSPNoError)
			{
			// We had to have retrieved some form of string.  Lets
			// go find it:

			char newPath[kMaxPathLength];
			
			LPCTSTR	lpPath = fileSpec->path;	// Search path.
			LPCTSTR lpFileName = NULL;			// Filename (part of path)
			LPCTSTR lpExtension = NULL;			// Extension (part of path)
			DWORD	nBufferLength = kMaxPathLength;
			LPTSTR	lpBuffer = newPath;	// Receiving buffer.
			LPTSTR	lpFilePart;					// Trailing / ends.
			DWORD bufferSize = SearchPath
				(
				lpPath,
				lpFileName,
				lpExtension,
				nBufferLength,
				lpBuffer,
				&lpFilePart
				);
 
			if (bufferSize < 1 || bufferSize > kMaxPathLength)
				{
				// We couldn't find it, so lets just get the filename
				// and store it wherever the current directory is
				// set:
				char filename[_MAX_FNAME];   
				char extension[_MAX_EXT];
   
				_splitpath
					(
					fileSpec->path,
					NULL,
					NULL,
					filename,
					extension
					);

				_makepath
					(
					newPath,
					NULL,
					NULL,
					filename,
					extension
					);
				}
			
			unsigned short length = strlen(newPath); 
			
			strncpy(fileSpec->path, newPath, length);
			fileSpec->path[length] = 0;
			
			} // Got no string from scripting system.
		
	#endif
	
	return error;
	
	} // PIURestoreFileSpec
// end TriggerFiltersScripting.cpp
