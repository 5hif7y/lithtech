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
//		PathsToPostScriptUIMac.cpp
//
//	Copyright 1990-1992, Thomas Knoll.
//	All Rights Reserved.
//
//	Description:
//		This file contains the user interface source
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

#include "PIDefines.h"
#include "PathsToPostScript.h"
#include <Navigation.h>

//-------------------------------------------------------------------------------
//	Definitions -- Dialog items
//-------------------------------------------------------------------------------

const uint8 kDPopUp = 1;
		
//-------------------------------------------------------------------------------
//	Prototypes.
//-------------------------------------------------------------------------------

static pascal void NavPutCustomEventProc(const NavEventCallbackMessage callBackSelector,
										 NavCBRecPtr callBackParms,
										 NavCallBackUserData callBackUD);
Boolean BuildPathMenu (GPtr globals,
					  MenuHandle theMenu,
					  short *defaultItem);
void AppendMenuItem (MenuHandle theMenu, Str255 itemText, short style);
void DeleteItemsTo (MenuHandle theMenu, short limit);

//-------------------------------------------------------------------------------
//
//	DoAbout
//
//	Displays the About box.
//
//	Inputs:
//		AboutRecordPtr about	Is actually a pointer to some platform
//								specific data for Windows only.
//		AboutID					#define with ID of String resource to use for text
//
//	Outputs:
//		Displays About box (Dialog).  Press Escape, Enter, or Return to
//		dismiss, or click the mouse.
//
//-------------------------------------------------------------------------------

void DoAbout (AboutRecordPtr /*about*/)
{
	ShowAbout (AboutID);
} // end DoAbout

/*****************************************************************************/

/* Display an alert and set gResult = 1. */

void DisplayStopAlert (GPtr globals, short id)
{
	ShowStop (NULL, AlertID, id, NULL, NULL);
	gResult = 1; // error and return
}

/*****************************************************************************/

void DeleteItemsTo (MenuHandle theMenu, short limit)
{
	short item = CountMenuItems (theMenu);
	
	while (item > limit)
		{
		DeleteMenuItem (theMenu, item);
		--item;
		}
	
}

/*****************************************************************************/

void AppendMenuItem (MenuHandle theMenu, Str255 itemText, short style)
{
	
	AppendMenu (theMenu, itemText);
	
	short item = CountMenuItems (theMenu);
	
	SetMenuItemText (theMenu, item, itemText);
	
	SetItemStyle (theMenu, item, style);
	
}

/*****************************************************************************/

Boolean BuildPathMenu (GPtr globals,
					  MenuHandle theMenu,
					  short *defaultItem)
	{
	int32 pathCount;
	
	if (!GetPathCount (globals, &pathCount) || pathCount < 1)
		{
		DeleteItemsTo (theMenu, 1);
		*defaultItem = 1;
		}
		
	else
	{
		
		int32 pathIndex;
		
		int16 workPathIndex = GetPathIndex (globals, propWorkPathIndex);
		int16 clippingPathIndex = GetPathIndex (globals, propClippingPathIndex);
		int16 targetPathIndex = GetPathIndex (globals, propTargetPathIndex);

		DeleteItemsTo (theMenu, (pathCount == 1) ? (iAllPaths-1) : (iPathStart-1));
		
		for (pathIndex = 0; pathIndex < pathCount; ++pathIndex)
			{
			
			Str255 name;

			short style = normal;
			
			if (!GetPathName (globals, pathIndex, name))
				return FALSE;
				
			if (pathIndex == clippingPathIndex)
				style |= outline;
				
			if (pathIndex == workPathIndex)
				style |= italic;
				
			AppendMenuItem (theMenu, name, style);
			
			}
		
		if (*defaultItem < 1)
		{ // default is still zero so we better set it
			if (pathCount == 1 || targetPathIndex < 0)
				*defaultItem = iAllPaths;
			else
				*defaultItem = iPathStart + targetPathIndex;
		}
	}
		
	return TRUE;
	
	}

/*****************************************************************************/
static short gLastWidth = 0;
static short gLastHeight = 0;
static Handle gNavSvcCustomDitl = NULL;

static pascal void NavPutCustomEventProc(const NavEventCallbackMessage callBackSelector,
										 NavCBRecPtr callBackParms,
										 NavCallBackUserData callBackUD)
{
	
	GPtr globals = (GPtr) callBackUD;
	OSType err;
	
	UInt16 firstItem = 0;

	DialogPtr	dialog = GetDialogFromWindow (callBackParms->window);

	switch (callBackSelector)
		{
		case kNavCBEvent:
			{
			switch (callBackParms -> eventData . eventDataParms .event -> what)
				{
				case nullEvent:
				case activateEvt:
				case updateEvt:
					/* Pass updates and activates out to the host */
					if (globals)
						if (gStuff->processEvent && (callBackParms -> eventData . eventDataParms .event->message != ((long) dialog)))
							(*gStuff->processEvent) (callBackParms -> eventData . eventDataParms .event);
					break;

				default:
					break;
				}
			break;
			}

		case kNavCBCustomize:
			{
			
			// negotiate for space as needed
			
			short vSaveCustomWidth = 300;
			short vSaveCustomHeight = 40;
			
			// this is generic code to negotiate with nav services for our space
			
			short neededWidth  = callBackParms -> customRect . left + vSaveCustomWidth;
			short neededHeight = callBackParms -> customRect . top  + vSaveCustomHeight;
			
			if ((callBackParms -> customRect . right  == 0) &&
				(callBackParms -> customRect . bottom == 0))
				{
				
				callBackParms -> customRect . right  = neededWidth;
				callBackParms -> customRect . bottom = neededHeight;
				
				}
				
			else
				{
				
				if (gLastWidth != callBackParms -> customRect . right)
					if (callBackParms -> customRect . right < neededWidth)
						callBackParms -> customRect . right = neededWidth;
					
				if (gLastHeight != callBackParms -> customRect . bottom)
					if (callBackParms -> customRect . bottom < neededHeight)
						callBackParms -> customRect . bottom = neededHeight;
						
				}
			
			gLastWidth  = callBackParms -> customRect . right;
			gLastHeight = callBackParms -> customRect . bottom;
			
			break;
			
			};

		case kNavCBAccept:
			{
			// Read our menu
			};
			break;

		case kNavCBStart:
			{
			gLastWidth = 0;
			gLastHeight = 0;
			gNavSvcCustomDitl = NULL;

			gNavSvcCustomDitl = Get1Resource( 'DITL', 17006 );
			if (( NULL != gNavSvcCustomDitl ) && ( noErr == ResError() ))
				if (( err = NavCustomControl( callBackParms -> context, kNavCtlAddControlList, gNavSvcCustomDitl )) == noErr ) 
					{
					MenuRef pathMenu;
					ControlRef popupControl;
					err = NavCustomControl(callBackParms->context,kNavCtlGetFirstControlID,&firstItem);	
					firstItem ++; // returns zero based, we need it one based
					err = GetDialogItemAsControl( dialog, firstItem, &popupControl);
					pathMenu = GetControlPopupMenuRef(popupControl);
					if (nil == pathMenu) break;
					if (BuildPathMenu(globals, pathMenu, &gPopUpValue))
						SetPopUpMenuValue(dialog, firstItem, gPopUpValue);
					short maxValue = GetControlMaximum ((ControlHandle) popupControl);
					short newValue = CountMenuItems (pathMenu);
					SetControlMaximum ((ControlHandle) popupControl, newValue);
					}
			};
			break;
			
		case kNavCBTerminate:
			{
			err = NavCustomControl(callBackParms->context,kNavCtlGetFirstControlID,&firstItem);
			firstItem ++; // returns zero based, we need it one based PR#19118
	
			gPopUpValue = GetPopUpMenuValue (dialog, firstItem);

			if ( NULL != gNavSvcCustomDitl )
				ReleaseResource( (Handle)gNavSvcCustomDitl );
			gNavSvcCustomDitl = NULL; // always brush before bed, and always init your globals to safe vals
			break;
			};
		};
}								 

/*****************************************************************************/

Boolean DoUI (GPtr globals)
{

	if (!gQueryForParameters) return TRUE;

	Str255 prompt;
	FSRef realRef;
	OSErr err = noErr;
	NavDialogRef dialogRef;
	NavReplyRecord navReply;
	NavDialogCreationOptions navOptions;
	NavUserAction action;
	NavEventUPP eventUPP = NewNavEventUPP(NavPutCustomEventProc);
	int32 nameLength = 0;

	gReply.sfGood = false;

	// this needs to happen first ( setup the dialog creation options )
	err = NavGetDefaultDialogCreationOptions( &navOptions );

	// setup the strings
	PIGetString(kPrompt, prompt);
	navOptions.windowTitle = CFStringCreateWithPascalString(NULL, prompt, kCFStringEncodingMacRoman);
	navOptions.message = CFStringCreateWithPascalString(NULL, "\pPaths To Post Script  to file:", kCFStringEncodingMacRoman);
	navOptions.saveFileName = CFStringCreateWithPascalString(NULL, gStuff->filename, kCFStringEncodingMacRoman);

	// Setup flags for the default supports we want
	navOptions.optionFlags |= kNavNoTypePopup;
	navOptions.preferenceKey =  'p2il';
	navOptions.modality = kWindowModalityAppModal;

	err = NavCreatePutFileDialog( 	&navOptions,
									'????',
									'8BIM',
									eventUPP,
									(void *) globals,
									&dialogRef );
									
	err = NavDialogRun ( dialogRef );

	err = NavDialogGetReply( dialogRef, &navReply );

	// what did the user do?
	action = NavDialogGetUserAction( dialogRef );

	if ( navReply.validRecord && action == kNavUserActionSaveAs )
		{
		FSRef resultFSRef;
		AEKeyword dummyKeyword = 0L;
		DescType  dummyType;
		Size      dummySize;

		// move the data into a real FSSpec
		err = AEGetNthPtr ( &(navReply.selection),
							1,
							typeFSRef,
							&dummyKeyword, 
							&dummyType,	
							&resultFSRef, 
							sizeof(FSRef), 
							&dummySize);

		if ( noErr == err )
			{
			FSRef tempWorkingRef;
			CFRange range;
			CFIndex length;
			UniChar *nameString;
			FSCatalogInfo catInfo;
			FileInfo *info;
			
			memset( &catInfo, 0x00, sizeof(FSCatalogInfo));
			
			// setup the filetype and creator we'll be using for the FSCreate operation
			info = (FileInfo *)&(catInfo.finderInfo);
			info->fileType = '????';
			info->fileCreator = '8BIM';

			// setup the length and buffer for the unicode string
			length = CFStringGetLength( navReply.saveFileName );
			nameString = (UniChar *)NewPtr(length*sizeof(UniChar));
			
			// setup the unicode range of characters to use
			range.location = 0;
			range.length = length;
			
			// Get the unicode name of the file
			CFStringGetCharacters( navReply.saveFileName, range, nameString );
			
			// if we can find the object already, delete it, Nav has already asked
			// us if we really want to do that.
			if ( noErr == FSMakeFSRefUnicode( &resultFSRef, length, nameString,
				kTextEncodingUnknown, &tempWorkingRef ))
				{
				FSDeleteObject( &tempWorkingRef );
				}
			
			// creat the file
			err = FSCreateFileUnicode( 	&resultFSRef,
										length,
										nameString,
										kFSCatInfoFinderInfo,
										&catInfo,
										&realRef,
										&gReply.sfFile );
			
			if ( noErr == err )
				{
				gReply.sfGood = true;
				gReply.sfReplacing = navReply.replacing;
				gReply.sfType = info->fileType;
				gReply.sfIsFolder = false;
				gReply.sfIsVolume = false;
				gReply.sfScript = navReply.keyScript; // need to change back to keyScript when checked in
				}
			// clean up the unicode name	
			DisposePtr( (Ptr)nameString);											
			}
		}


	CFRelease(navOptions.message);
	CFRelease(navOptions.windowTitle);
	CFRelease(navOptions.saveFileName);
	
	navOptions.message = NULL;
	navOptions.windowTitle = NULL;
	navOptions.saveFileName = NULL;
	
	DisposeNavEventUPP(eventUPP);

	// clean up the reply
	err = NavDisposeReply( &navReply );
	
	// clean up the nav dialog
	NavDialogDispose( dialogRef );
	
	return TRUE;
}
/*****************************************************************************/

Boolean CreateExportFile (GPtr globals)
{
	return PIOpenFile(gReply.sfFile, 
					  &gFRefNum,
					  &gResult);
}

/*****************************************************************************/

Boolean CloseExportFile (GPtr globals)
{
	Boolean err = PICloseFileAndMakeAlias (gReply.sfFile,
					   		   			   gFRefNum,
					   		   			   gSameNames,
					   		   			   &gStuff->dirty,
					   		   			   &gAliasHandle,
					   		   			   &gResult);	
	return err;

}
	
/*****************************************************************************/

void SmallFileName (Str255 s, const Str255 file)
{
	PIResetString(s);
	AppendString(s, file, 1, file[0]); // copy string
}

//-------------------------------------------------------------------------------

// end PathsToPostScriptUIMac.cpp

