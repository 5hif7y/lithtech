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
//		PathsToPostScriptScripting.cpp
//
//	Copyright 1990-1992, Thomas Knoll.
//	All Rights Reserved.
//
//	Description:
//		This file contains the scripting functions
//		for the Export module PathsToPostScript, a module that
//		creates a file with PostScript information
//		detailing a specific path, or all paths.  It
//		is taken from the source of the "Paths to Illustrator"
//		plug-in that ships with the product.
//
//	Use:
//		This module shows how to convert paths accessed
//		from the properties suite into PostScript and save
//		them to a file.  You access it via
//		File>>Export>>PathsToPostScript.
//
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
//	Includes 
//-------------------------------------------------------------------------------

#include "PhotoshopSDK.h"
#include "PathsToPostScript.h"

//-------------------------------------------------------------------------------
//
//	ReadScriptParams
//
//	Checks the parameters against scripting-returned parameters, if any, and
//	updates the globals to match ones provided by the scripting system.
//
//	Inputs:
//		GPtr globals		Pointer to global structure.
//
//	Outputs:
//		returns TRUE		If you should pop your dialog.
//		returns FALSE		If you should not pop your dialog.
//
//		gResult				Will return any fatal error.
//
//-------------------------------------------------------------------------------

Boolean ReadScriptParams (GPtr globals)
{
	PIReadDescriptor	token = NULL;	// token we'll use to read descriptor
	DescriptorKeyID		key = NULLID;	// the next key
	DescriptorTypeID	type = NULLID;	// the type of the key we read
	int32				flags = 0;		// any flags for the key
	
	// Populate this array if we're expecting any keys,
	// must be NULLID terminated:
	DescriptorKeyIDArray array = { keyIn, keyOurPath, NULLID };
	
	// While we're reading keys, errors will stick around and be reported
	// when we close the token:
	OSErr				err = noErr;
	
	// Assume we want to pop our dialog unless explicitly told not to:
	Boolean				returnValue = true;
	
	if (DescriptorAvailable(NULL))
	{ // descriptor suite is available, go ahead and open descriptor:
	
		// PIUtilities routine to open descriptor handed to us by host:
		token = OpenReader(array);
		
		if (token)
		{ // token was valid, so read keys from it:
			Handle h = NULL; // use for reading
			
			while (PIGetKey(token, &key, &type, &flags))
			{ // got a valid key.  Figure out where to put it:
			
				switch (key)
				{ // match a key to its expected type:case keyAmount:

					case keyIn:
						PIGetAlias(token, &(Handle)gAliasHandle);
						break;
						// ignore all other cases and classes
					
					case keyOurPath:
						PIGetText(token, &h);
						if (h != NULL)
						{ // passed us a handle
							gPopUpValue = Handle2PopUp(globals, h);
							PIDisposeHandle(h); // dispose what we're given
							h = NULL; // just in case
						}
						break;
					
					// ignore all other cases and classes
					// See PIActions.h and PIUtilities.h for
					// routines and macros for scripting functions.
				
				} // key
				
			} // PIGetKey

			// PIUtilities routine that automatically deallocates,
			// closes, and sets token to NULL:
			err = CloseReader(&token);
				
			if (err)
			{ // an error did occur while we were reading keys:
			
				if (err == errMissingParameter) // missedParamErr == -1715
				{ // missing parameter somewhere.  Walk IDarray to find which one.
				}
				else
				{ // serious error.  Return it as a global result:
					gResult = err;
				}
					
			} // stickyError
						
		} // didn't have a valid token
		
		// Whether we had a valid token or not, we were given information
		// as to whether to pop our dialog or not.  PIUtilities has a routine
		// to check that and return TRUE if we should pop it, FALSE if not:	
		returnValue = PlayDialog();
	
	} // descriptor suite unavailable
	
	return returnValue;
	
} // end ReadScriptParams

//-------------------------------------------------------------------------------
//
//	WriteScriptParams
//
//	Takes our parameters from our global variables and writes them out to the
//	scripting system, which hands it all back to the host.
//
//	Inputs:
//		GPtr globals				Pointer to global structure.
//
//	Outputs:
//		returns an OSErr		If there was a serious error.
//		returns noErr			If everything worked fine.
//
//-------------------------------------------------------------------------------

OSErr WriteScriptParams (GPtr globals)
{
	PIWriteDescriptor	token = NULL;	// token to write our parameters to
	OSErr				err = noErr; // we'll return any error with this
			
	if (DescriptorAvailable(NULL))
	{ // Descriptor suite is available.  Open a token:
	
		// PIUtilities has a routine that will create a descriptor to write
		// parameters to:
		token = OpenWriter();
		
		if (token)
		{ // was able to create the token, write our keys:
			
			Handle h = PopUp2Handle(globals, gPopUpValue);
			if (h != NULL)
			{
				PIPutText(token, keyOurPath, h);
				PIDisposeHandle(h);
				h = NULL;
			}
			
			// Store then dispose the handle we created.
			if (gAliasHandle != NULL)
			{
				PIPutAlias(token, keyIn, (Handle)gAliasHandle);
				PIDisposeHandle((Handle)gAliasHandle);
				gAliasHandle = NULL;
			}
			
			// See PIActions.h and PIUtilities.h for
			// routines and macros for scripting functions.

			// PIUtilities has a routine that will close the token, collapse
			// it, and store it in the struct that the host will then grab
			// and store for recording and/or subsequent scripting.  It tells
			// the host that the dialog is optional.  You can override this
			// by setting recordInfo to whatever you need.  It then
			// deallocates token and sets it to NULL:
			err = CloseWriter(&token);
			
		} // wasn't able to create token
	
	} // descriptor suite unavailable
	
	return err;
	
} // end WriteScriptParams

/*****************************************************************************/
// Maps gPopUp to a string then returns the string as a handle

Handle PopUp2Handle (GPtr globals, short popup)
{
	Str255 s = "";
	
	switch (popup)
	{
		case iDocBounds:
			PIGetString(kDocBounds, s);
			break;
		case iAllPaths:
			PIGetString(kAllPaths, s);
			break;
		default:
			if (popup >= iPathStart)
				GetPathName(globals, popup - iPathStart, s);
			break;
	}
	if (s[0] > 0)
		return PIPString2Handle(s);
	else return NULL;
}

/*****************************************************************************/
// Maps string to popup values and returns the index

short Handle2PopUp (GPtr globals, Handle h)
{
	Str255 s = "";
	Str255 s2 = "";
	int32 pathCount = 0;
	int32 pathIndex = 0;
	short retn = 0;
	
	PIHandle2PString(h, s2);
	
	PIGetString(kDocBounds, s);
	if (PISMatch(s, (char *)s2) == gotMatch)
		retn=iDocBounds; // document bounds
	else
	{ // keep checking
		PIGetString(kAllPaths, s);
		if (PISMatch(s, (char *)s2) == gotMatch)
			retn=iAllPaths; // all paths
		else
		{ // keep checking
			GetPathCount(globals, &pathCount);
			if (pathCount < 1)
				retn = 0; // no paths
			else
			{ // keep checking
				do
				{	
					if (GetPathName (globals, pathIndex, s))
					{ // got a name
						if (PISMatch(s, (char *)s2) == gotMatch)
						{
							retn = (short)(iPathStart+pathIndex); // extra menu items
							pathIndex = pathCount; // skip to end
						}
					}
				} while (pathIndex++ < pathCount);
			}
		}
	}
	return retn;
}

// end PathsToPostScriptScripting.cpp